/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qdownload.h"
#include "qmodelfactory.h"
#include <QFileInfo>
#include <QDateTime>
#include <QDir>
#include "qmsgprofileinfoend.h"
#include "qrserverprofile.h"
#include "qfilemodel.h"
#include "qmsgprofileblk.h"
#include "qmsgprofileend.h"
#include "qmsggetprofile.h"
#include "qgenericbinmessage.h"
#include "qprocessmessagecommand.h"
#include "qprocessmessagestatus.h"
#include "qtimerstatuswatcher.h"
#include "qclientmodel.h"
#include <QProcess>
#include "qrserverenvironment.h"
#include "abstractlockdownsetting.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <linux/fs.h>

using namespace QProfile;
using namespace QRserver;

#ifdef BROADCAST_PROFILE_DOWNLOAD
#include "qrserverprofileinfo.h"
#endif

QDownload::QDownload(QObject *parent): QRserverContainer(parent, false),
    m_downloadFileModel(nullptr)
{
    subscribeMessage(rserver::eProfileInfo);
    subscribeMessage(rserver::eProfileInfoEnd);
    subscribeMessage(rserver::eProfileFileBlk);
    subscribeMessage(rserver::eProfileFileEnd);
}

bool QDownload::initPropertiesMap(const QVariantMap &properties)
{
    if (properties.count() <= 0) {
        setProperty("tagName", "qdownload");
    }

    return QRserverContainer::initPropertiesMap(properties);
}

bool QDownload::processCommands(const QString &commandId,
                                const QString &commandType, QVariant commandVar, QVariant *status)
{
    if (!commandType.isEmpty()) {
        qprofileDebug(QtDebugMsg) << commandType;
        QDownloadStatus downloadStatus;

        if (commandType == QDownloadFileModel::CommandDownloadAll) {
            if (!deleteLastLockdownProf()) {
                downloadStatus.setCommandStatus((QAbstractStatus::CommandStatus)
                                                QDownloadStatus::DeleteLastLockdownProfFailed);
            } else if (!unlockDownProfiles()) {
                downloadStatus.setCommandStatus((QAbstractStatus::CommandStatus)
                                                QDownloadStatus::UnlockDownFailed);
            } else {
                downloadStatus.fromVariant(download(commandVar));
            }
        } else if (commandType == QDownloadFileModel::CommandLockDown) {
            downloadStatus.fromVariant(lockDown(commandVar));
        }

        if (status != nullptr) {
            downloadStatus.setId(commandId);
            downloadStatus.setMethod(commandType);
            *status = downloadStatus.toVariant();
        }
    }

    return true;
}

bool QDownload::getChangeList()
{
    return false;
}

const QVariant QDownload::download(QVariant argsVar)
{
    QVariantList fileModelList = argsVar.toList();
    QVariantList::const_iterator i;
    QDownloadStatus downloadStatus;
#ifdef BROADCAST_PROFILE_DOWNLOAD
    bool bRecordProfileInfo = false;
    QTextStream out;
    QString path;
    QFile file(QRserverEnvironment::DownloadedProfileMark());
    QRserverProfileInfo profileInfo;

    if (file.open(QFile::WriteOnly | QFile::Truncate)) {
        out.setDevice(&file);
        out << "[";
        bRecordProfileInfo = true;
    } else {
        bRecordProfileInfo = false;
        qprofileDebug(QtWarningMsg) << QString("Open 1% failed.").arg(
                                        QRserverEnvironment::DownloadedProfileMark());
    }

#endif

    for (i = fileModelList.constBegin(); i != fileModelList.constEnd(); ++i) {
        QFileModel fileModel;
        fileModel.fromVariant(*i);
        downloadStatus.setCommandStatus(QAbstractStatus::Failed);
        downloadStatus.setFileModel(*i);
        quint32 retryCount = 0;

        while (downloadStatus.commandStatus() != QAbstractStatus::Succeed
               && retryCount++ < QAbstractCommand::RetryCountLimitation) {
            downloadStatus.setCommandStatus(downloadSingleFile(&fileModel));

            if (downloadStatus.commandStatus() != QAbstractStatus::Succeed) {
                qprofileDebug(QtWarningMsg) << "Failed to download file " <<
                                            fileModel.toVariant() << downloadStatus.commandStatus()
                                            << "Retry count = " << retryCount;

                if (!downloadStatus.canRecover()) {
                    return downloadStatus.toVariant();
                }
            }

#ifdef BROADCAST_PROFILE_DOWNLOAD
            else {
                if (bRecordProfileInfo) {
                    profileInfo.setProfileInfo(fileModel.pathName());
                    out << profileInfo.toJson();

                    if (i < fileModelList.constEnd() - 1) {
                        out << ",";
                    }

                    out.flush();
                }
            }

#endif
        }

        if (downloadStatus.commandStatus() != QAbstractStatus::Succeed) {
            qprofileDebug(QtWarningMsg) << "Failed to download profile, status =  " <<
                                        downloadStatus.commandStatus() << ", file path = " << fileModel.pathName();
            break;
        }
    }

#ifdef BROADCAST_PROFILE_DOWNLOAD

    if (bRecordProfileInfo) {
        out << "]";
    }

#endif
    return downloadStatus.toVariant();
}

bool QDownload::isRsaEncrypedFile(const QString filePathName)
{
    const QString encryptFlag("<encrypted/>");
    QFile file(filePathName);

    if (!file.exists()) {
        qprofileDebug(QtWarningMsg) << filePathName << " is not existed!";
        return false;
    }

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qprofileDebug(QtWarningMsg) << filePathName << " open failed!";
        return false;
    }

    QString line(file.readLine());
    file.close();
    return line.contains(encryptFlag);
}

bool QDownload::decryptFile(const QString filePathName)
{
    QFile file(filePathName);
    QString decryptedFilePathName(filePathName + ".des");

    if (!file.exists()) {
        qprofileDebug(QtWarningMsg) << filePathName << " is not existed!";
        return false;
    }

    QStringList arguments = QStringList() << "-i" << filePathName << "-o" <<
                            decryptedFilePathName;

    if (0 > QProcess::execute(QRserverEnvironment::ProfileDecryptTool(),
                              arguments)) {
        qprofileDebug(QtWarningMsg) << "decrypt failed!";
        return false;
    }

    if (!file.remove()) {
        qprofileDebug(QtWarningMsg) << filePathName << " is removed failed!";
        return false;
    }

    if (!QFile::copy(decryptedFilePathName, filePathName)) {
        qprofileDebug(QtWarningMsg) << "Copy " << decryptedFilePathName << " to " <<
                                    filePathName << " failed!";
        return false;
    }

#ifdef BUILD_X86

    if (!QFile::setPermissions(filePathName,
                               QFile::ReadOwner | QFile::WriteOwner | QFile::ReadUser
                               | QFile::WriteUser | QFile::ReadOther | QFile::WriteOther | QFile::WriteGroup |
                               QFile::ReadGroup | QFile::ExeOwner | QFile::ExeUser | QFile::ExeGroup |
                               QFile::ExeOther)) {
        qprofileDebug(QtWarningMsg) << "Failed to set permission for file " <<
                                    filePathName;
    }

#endif
    return QFile::remove(decryptedFilePathName);
}

const QAbstractStatus::CommandStatus QDownload::downloadSingleFile(
    const QFileModel *fileModel)
{
    QVariantList relatedFileList = m_downloadFileModel->getFileModeList(
                                       fileModel->fileType(), fileModel->fileName());

    foreach (QVariant fileModelVariant, relatedFileList) {
        QFileModel relatedFile;
        relatedFile.fromVariant(fileModelVariant);
        QFile file(relatedFile.pathName());

        if (file.exists()) {
            file.remove();
            file.flush();
        }

        file.close();
    }

    QProcessMessageCommand command(QVariant(), this);
    QMsgGetProfile getProfileMessage;
    getProfileMessage.setProfileName(fileModel->fileName());
    getProfileMessage.setFileType(fileModel->fileType());
    command.appendMessage(0, &getProfileMessage);
    QGenericBinMessage profileEndMessage(rserver::eProfileFileEnd);
    command.appendMessage(getProfileMessage.id(), &profileEndMessage);

    if (0 == publish(command.topic(), command.toJson())) {
        qprofileDebug(QtWarningMsg) << "publish command failed!";
        return QAbstractStatus::IPCError;
    } else {
        QTimerStatusWatcher watcher(this, QClientModel::RserverClientStatusTopic,
                                    command.id(), QDownloadFileModel::DownloadSingleFileTimeout);

        if (watcher.exec() == QTimerStatusWatcher::Timeout) {
            qprofileDebug(QtWarningMsg) << "Download single file timeout " <<
                                        fileModel->pathName();
            return QAbstractStatus::Timeout;
        }

        QProcessMessageStatus status(watcher.status());

        if (status.commandStatus() == QAbstractStatus::Succeed) {
            QVariantList relatedFileList = m_downloadFileModel->getFileModeList(
                                               fileModel->fileType(), fileModel->fileName());

            foreach (QVariant fileModelVariant, relatedFileList) {
                QFileModel relatedFile;
                relatedFile.fromVariant(fileModelVariant);
                QFileInfo fileInfo(relatedFile.pathName());
                // File size will be changed after decrypt,
                // so disable cache for getting timely file info
                fileInfo.setCaching(false);

                if (isRsaEncrypedFile(relatedFile.pathName())) {
                    if (!decryptFile(relatedFile.pathName())) {
                        return (QAbstractStatus::CommandStatus)QDownloadStatus::DecryptFailed;
                    }

                    if (!AbstractLockDownSetting::Instance()->setCryptedMark(
                            relatedFile.pathName().toStdString())) {
                        qprofileDebug(QtWarningMsg) << "Set encrypted mark failed! "
                                                    << relatedFile.pathName();
                    }

                    fileInfo.refresh();
                }

                if (fileModel->fileSize() != fileInfo.size()) {
                    qprofileDebug(QtWarningMsg) << "Download single file failed, file name = " <<
                                                relatedFile.fileName() << ", expected size = " << fileModel->fileSize() <<
                                                ", actual size = " << fileInfo.size();
                    return (QAbstractStatus::CommandStatus)QDownloadStatus::FileSizeMismatch;
                }

                modifyFileDateTime(fileInfo.absoluteFilePath(), fileModel->remoteDateTime());
            }
        }

        return status.commandStatus();
    }
}

#ifdef BUILD_X86
#include "qrserverenvironment.h"
void QDownload::setPermissions(const QString &fileName,
                               QFile::Permissions permissions)
{
    QDir dir(fileName);
    QDir rserverDir(QRserverEnvironment::StorageRoot());
    qprofileDebug(QtDebugMsg) << fileName << QRserverEnvironment::StorageRoot();

    while (dir != rserverDir) {
        QFile storageDir(dir.absolutePath());
        storageDir.setPermissions(QFile::ReadOwner | QFile::WriteOwner | QFile::ReadUser
                                  | QFile::WriteUser | QFile::ReadOther | QFile::WriteOther | QFile::WriteGroup |
                                  QFile::ReadGroup | QFile::ExeOwner | QFile::ExeUser | QFile::ExeGroup |
                                  QFile::ExeOther);
        dir.cdUp();
    }
}
#endif

void QDownload::saveProfile(QAbstractBinMsg *message)
{
    QMsgProfileBlk *profileBlock = (QMsgProfileBlk *)message;
    Q_ASSERT(profileBlock != nullptr);
    qprofileDebug(QtDebugMsg) << message->type() << profileBlock->profileType() <<
                              profileBlock->profileName();
    QVariantList relatedFileList = m_downloadFileModel->getFileModeList(
                                       profileBlock->profileType(), profileBlock->profileName());

    foreach (QVariant fileModelVariant, relatedFileList) {
        qprofileDebug(QtDebugMsg) << fileModelVariant;

        if (fileModelVariant.isValid()) {
            QFileModel fileModel;
            fileModel.fromVariant(fileModelVariant);
            QFileInfo fileInfo(fileModel.pathName());
            QFile file(fileInfo.absoluteFilePath());
            QIODevice::OpenMode openMode = QIODevice::WriteOnly;
            QDir dir(fileInfo.absolutePath());

            if (!dir.exists()) {
                if (!dir.mkpath(fileInfo.absolutePath())) {
                    qprofileDebug(QtWarningMsg) << "Failed to create dir " <<
                                                fileInfo.absolutePath();
                    return;
                }

#ifdef BUILD_X86
                setPermissions(fileInfo.absolutePath(),
                               QFile::ReadOwner | QFile::WriteOwner | QFile::ReadUser
                               | QFile::WriteUser | QFile::ReadOther | QFile::WriteOther | QFile::WriteGroup |
                               QFile::ReadGroup | QFile::ExeOwner | QFile::ExeUser | QFile::ExeGroup |
                               QFile::ExeOther);
#endif
            }

            if (fileInfo.exists()) {
                QDateTime lastModified = fileInfo.lastModified();
                qint64 milliseconds = lastModified.msecsTo(QDateTime::currentDateTime());

                if (milliseconds > 0 && milliseconds < 10 * 1000) {
                    openMode |= QIODevice::Append;
                } else {
                    openMode |= QIODevice::Truncate;
                }
            }

            if (!file.open(openMode)) {
                qprofileDebug(QtWarningMsg) << "Failed to open file " << fileModelVariant;
                return;
            }

#ifdef BUILD_X86
            file.setPermissions(QFile::ReadOwner | QFile::WriteOwner | QFile::ReadUser
                                | QFile::WriteUser | QFile::ReadOther | QFile::WriteOther | QFile::WriteGroup |
                                QFile::ReadGroup | QFile::ExeOwner | QFile::ExeUser | QFile::ExeGroup |
                                QFile::ExeOther);
#endif

            if (profileBlock->blockLength() != file.write(QByteArray::fromBase64(
                        profileBlock->block()), profileBlock->blockLength())) {
                qprofileDebug(QtWarningMsg) << "Failed to write file " << fileModelVariant;
                file.remove();
                return;
            }

            file.flush();

            if (message->type() == rserver::eProfileFileEnd) {
                qprofileDebug(QtWarningMsg) << "Save file successfully, path: " <<
                                            fileInfo.absoluteFilePath();
            }
        }
    }
}

void QDownload::modifyFileDateTime(const QString &filepath,
                                   const QDateTime &dateTime)
{
    struct utimbuf uid_time;
    uid_time.actime  = dateTime.toTime_t();
    uid_time.modtime = dateTime.toTime_t();
    utime(filepath.toLatin1().data(), &uid_time);
}

void QDownload::appendProfileInfo(QAbstractBinMsg *message)
{
    static quint32 lastMessageType = 0;
    static QVariantList profileList;

    if (lastMessageType == rserver::eProfileInfoEnd) {
        m_downloadFileModel->clear();
        profileList.clear();
    }

    QMsgProfileInfo *profileInfo = qobject_cast<QMsgProfileInfo *>(message);
    Q_ASSERT(profileInfo != nullptr);
    QVariantList list = profileInfo->profileInfoList();
    QVariantList::const_iterator i;

    for (i = list.constBegin(); i != list.constEnd(); ++i) {
        QFileModel fileModel;
        fileModel.fromVariant(*i);
        QVariantList relatedFileList = QRserverProfile::GetDeviceRelatedFileType((
                                           qint32)fileModel.fileType());

        foreach (QVariant relatedFile, relatedFileList) {
            if (relatedFile.isValid()) {
                fileModel.fromVariant(relatedFile);
                fileModel.fromVariant(*i);
                fileModel.setPathName(QString("%1/%2").arg(fileModel.pathName()).arg(
                                          fileModel.fileName()));
                QFileInfo fileInfo(fileModel.pathName());
                QDateTime lastModified;

                if (fileInfo.exists()) {
                    lastModified = fileInfo.lastModified();
                } else {
                    lastModified.setTime_t(0);
                    lastModified = lastModified.toUTC();
                }

                fileModel.setDateTime(lastModified);
                fileModel.setLocalSize(fileInfo.size());
                profileList.append(fileModel.toVariant());
            }
        }
    }

    lastMessageType = message->type();

    if (lastMessageType == rserver::eProfileInfoEnd) {
        m_downloadFileModel->setFileModelList(profileList);
    }
}

void QDownload::processMessage(QAbstractBinMsg *message)
{
    Q_ASSERT(message != nullptr);

    switch (message->type()) {
    case rserver::eProfileInfo:
    case rserver::eProfileInfoEnd: {
        appendProfileInfo(message);
    }
    break;

    case rserver::eProfileFileBlk:
    case rserver::eProfileFileEnd: {
        saveProfile(message);
    }
    break;

    default:
        break;
    }
}

void QDownload::allocModel()
{
    setModel(m_downloadFileModel = QModelFactory::construct<QDownloadFileModel>
                                   (this));
}

const QVariant QDownload::lockDown(QVariant argsVar)
{
    QDownloadStatus status;
    status.setCommandStatus(QAbstractStatus::Succeed);
#ifdef LOCK_LOCAL_PROFILES

    if (argsVar.toBool()) {
        if (!lockDownProfiles()) {
            status.setCommandStatus((QAbstractStatus::CommandStatus)
                                    QDownloadStatus::LockDownFailed);
        }
    } else {
        if (!unlockDownProfiles()) {
            status.setCommandStatus((QAbstractStatus::CommandStatus)
                                    QDownloadStatus::UnlockDownFailed);
        }
    }

#else

    if (!unlockDownProfiles()) {
        status.setCommandStatus((QAbstractStatus::CommandStatus)
                                QDownloadStatus::UnlockDownFailed);
        return status.toVariant();
    }

    if (argsVar.toBool()) {
        if (!lockDownProfiles()) {
            status.setCommandStatus((QAbstractStatus::CommandStatus)
                                    QDownloadStatus::LockDownFailed);
        }
    }

#endif
    return status.toVariant();
}

bool QDownload::lockDownProfiles()
{
    QVariantList downloadFileModelList = m_downloadFileModel->fileModelList();
    QFileModel fileModel;
    QString pathName;

    foreach (QVariant var, downloadFileModelList) {
        fileModel.fromVariant(var);
        pathName = fileModel.pathName();

        if (QFile::exists(pathName)) {
            if (!AbstractLockDownSetting::Instance()->setImmutable(pathName.toStdString(),
                    true)) {
                qprofileDebug(QtWarningMsg) << "Set immutable failed! " << pathName;
                return false;
            }
        }
    }

    QStringList devRelatedProfPathList =
        QRserverProfile::GetDeviceRelatedProfilePathList();
    QDir profPathDir;

    foreach (QString profPath, devRelatedProfPathList) {
        profPathDir.setPath(profPath);

        if (profPathDir.exists()) {
#ifdef LOCK_LOCAL_PROFILES
            QFileInfoList fileList = profPathDir.entryInfoList(QDir::Files);
            QString profPathName;
            bool bImmutable = false;

            foreach (QFileInfo fileInfo, fileList) {
                profPathName = fileInfo.absoluteFilePath();

                if (!AbstractLockDownSetting::Instance()->getImmutable(
                        profPathName.toStdString(), bImmutable)) {
                    qprofileDebug(QtWarningMsg) << "Get immutable failed! " << profPathName;
                    return false;
                } else {
                    if (!bImmutable) {
                        if (!QFile::remove(profPathName)) {
                            qprofileDebug(QtWarningMsg) << "Remove profile failed! " << profPathName;
                            return false;
                        }
                    }
                }
            }

            if (!profPathDir.entryList(QDir::Files).isEmpty()
                && !AbstractLockDownSetting::Instance()->setImmutable(profPath.toStdString(),
                        true)) {
                qprofileDebug(QtWarningMsg) << "Set immutable failed! " << profPath;
                return false;
            }

#endif
        } else {
            if (!profPathDir.mkpath(profPath)) {
                qprofileDebug(QtWarningMsg) << "Make path failed! " << profPath;
                return false;
            }
        }
    }

    return true;
}

bool QDownload::unlockDownProfiles()
{
    QStringList devRelatedProfPathList =
        QRserverProfile::GetDeviceRelatedProfilePathList();
    QDir profPathDir;

    foreach (QString profPath, devRelatedProfPathList) {
        profPathDir.setPath(profPath);

        if (profPathDir.exists()) {
            QFileInfoList fileList = profPathDir.entryInfoList(QDir::Files);

            foreach (QFileInfo fileInfo, fileList) {
                if (!AbstractLockDownSetting::Instance()->setImmutable(
                        fileInfo.absoluteFilePath().toStdString(), false)) {
                    qprofileDebug(QtWarningMsg) << "Set immutable failed! " <<
                                                fileInfo.absoluteFilePath();
                    return false;
                }
            }

            if (!AbstractLockDownSetting::Instance()->setImmutable(profPath.toStdString(),
                    false)) {
                qprofileDebug(QtWarningMsg) << "Set immutable failed! " << profPath;
                return false;
            }
        }
    }

    return true;
}

bool QDownload::deleteLastLockdownProf()
{
    QStringList devRelatedProfPathList =
        QRserverProfile::GetDeviceRelatedProfilePathList();
    QDir profPathDir;

    foreach (QString profPath, devRelatedProfPathList) {
        profPathDir.setPath(profPath);

        if (profPathDir.exists()) {
            if (!AbstractLockDownSetting::Instance()->setImmutable(profPath.toStdString(),
                    false)) {
                qprofileDebug(QtWarningMsg) << "Set immutable failed! " << profPath;
                return false;
            }

            QFileInfoList fileList = profPathDir.entryInfoList(QDir::Files);
            QString profPathName;
            bool bImmutable = false;

            foreach (QFileInfo fileInfo, fileList) {
                profPathName = fileInfo.absoluteFilePath();

                if (!AbstractLockDownSetting::Instance()->getImmutable(
                        profPathName.toStdString(), bImmutable)) {
                    qprofileDebug(QtWarningMsg) << "Get immutable failed! " << profPathName;
                    return false;
                } else {
                    if (bImmutable) {
                        if (!AbstractLockDownSetting::Instance()->setImmutable(
                                profPathName.toStdString(), false)) {
                            qprofileDebug(QtWarningMsg) << "Set immutable failed! " << profPathName;
                            return false;
                        }

                        if (!QFile::remove(profPathName)) {
                            qprofileDebug(QtWarningMsg) << "Remove profile failed! " << profPathName;
                            return false;
                        }
                    }
                }
            }
        }
    }

    return true;
}

