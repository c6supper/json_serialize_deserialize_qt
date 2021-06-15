/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "quploadprofile.h"
#include "qobjectfactory.h"
#include "qmodelfactory.h"
#include <QFileInfo>
#include "qrserverprofile.h"
#include "quploadcommand.h"
#include "qprocessmessagecommand.h"
#include "qmsguploadprofilefinish.h"
#include "qmessageseries.h"
#include "qmsgsendcmdsucceed.h"
#include "qgenericbinmessage.h"
#include "qtimerstatuswatcher.h"
#include "qabstracttopic.h"
#include "qclientmodel.h"
#include "qprocessmessagestatus.h"
#include "qtimerstatuswatcher.h"

#include "qmsguploadprofilefinishlongname.h"
#include "qmsguploadprofileblocklongname.h"
#include "qmsguploadprofileack.h"
#include "qmsguploadprofileinfoend.h"
#include "qmsguploadprofileinfoack.h"
#include "quploadprofilemodel.h"
#include "qmsgprofileinfo.h"
#include <QFileInfo>
#include "qfilemodel.h"
#include "qrservercompatibility510.h"
using namespace QProfile;
using namespace QRserver;

QObjectSelfRegisteration<QUploadProfile> registerQUploadProfile;

QUploadProfile::QUploadProfile(QObject *parent): QUploadFile(parent)
{
    subscribeMessage(rserver::eUploadProfileAck);
    subscribeMessage(rserver::eUploadProfileInfoAck);
}

bool QUploadProfile::initPropertiesMap(const QVariantMap &properties)
{
    if (properties.count() <= 0) {
        setProperty("tagName", "quploadprofile");
    }

    return QRserverContainer::initPropertiesMap(properties);
}

void QUploadProfile::allocModel()
{
    setModel(m_uploadProfileModel = QModelFactory::construct<QUploadProfileModel>
                                    (this));
    QUploadFile::allocModel();
}

const QAbstractStatus::CommandStatus QUploadProfile::uploadSingleFile(
    const QFileModel *fileModel)
{
    QFileInfo fileInfo(fileModel->pathName());
    QFile file(fileModel->pathName());

    if (fileInfo.exists() && fileInfo.isFile() && file.open(QIODevice::ReadOnly)) {
        qint64 length = fileInfo.size();
        qint32 blockIdx = 0;
        QProcessMessageCommand command(QVariant(), this);
        quint32 lastSucceedId = 0;
        QMsgUploadProfileBlock *pProfileBlock = nullptr;

        while (length > QMsgUploadProfileBlock::MaxUploadFileBlock) {
            if (isCompatible(QRserverCompatibility510::FeatureUploadProfileAck)) {
                pProfileBlock = new QMsgUploadProfileBlockLongName(this);
            } else {
                pProfileBlock = new QMsgUploadProfileBlock(this);
            }

            if (pProfileBlock == nullptr) {
                return (QAbstractStatus::CommandStatus) QUploadStatus::AllocMemoryFailed;
            }

            pProfileBlock->setProfileName(fileInfo.baseName());
            pProfileBlock->setProfileType(fileModel->fileType());
            pProfileBlock->setBlockIndex(blockIdx++);
            pProfileBlock->setTotalLength(fileInfo.size());
            pProfileBlock->setBlock(file.read((qint64)
                                              QMsgUploadProfileBlock::MaxUploadFileBlock).toBase64());
            length -= QMsgUploadProfileBlock::MaxUploadFileBlock;
            command.appendMessage(lastSucceedId, pProfileBlock);
            QMsgSendCmdSucceed succeedMessage(pProfileBlock->type(), this);
            command.appendMessage(pProfileBlock->id(), &succeedMessage);
            lastSucceedId = succeedMessage.id();
            pProfileBlock->deleteLater();
        }

        QMsgUploadProfileBlock *pProfileFinish = nullptr;

        if (isCompatible(QRserverCompatibility510::FeatureUploadProfileAck)) {
            pProfileFinish = new QMsgUploadProfileFinishLongName(this);
        } else {
            pProfileFinish = new QMsgUploadProfileFinish(this);
        }

        if (pProfileFinish == nullptr) {
            return (QAbstractStatus::CommandStatus) QUploadStatus::AllocMemoryFailed;
        }

        pProfileFinish->setProfileName(fileInfo.baseName());
        pProfileFinish->setProfileType(fileModel->fileType());
        pProfileFinish->setBlockIndex(blockIdx++);
        pProfileFinish->setTotalLength(fileInfo.size());
        pProfileFinish->setBlock(file.read(length).toBase64());
        command.appendMessage(lastSucceedId, pProfileFinish);

        if (isCompatible(QRserverCompatibility510::FeatureUploadProfileAck)) {
            QMsgUploadProfileAck profileAckMessage(this, true);
            profileAckMessage.setResultName(fileInfo.baseName());
            profileAckMessage.setResultType(fileModel->fileType());
            command.appendMessage(pProfileFinish->id(), &profileAckMessage);
        } else {
            QMsgSendCmdSucceed succeedMessage(pProfileFinish->type(), this);
            command.appendMessage(pProfileFinish->id(), &succeedMessage);
        }

        pProfileFinish->deleteLater();

        if (0 == publish(command.topic(), command.toJson())) {
            qprofileDebug(QtWarningMsg) << "publish command failed!";
            return QAbstractStatus::IPCError;
        } else {
            QTimerStatusWatcher watcher(this, QClientModel::RserverClientStatusTopic,
                                        command.id(), QUploadFileModel::UploadSingleFileTimeout);

            if (watcher.exec() == QTimerStatusWatcher::Timeout) {
                qprofileDebug(QtWarningMsg) << "Upload single file timeout " <<
                                            fileModel->pathName();
                return QAbstractStatus::Timeout;
            }

            QProcessMessageStatus status(watcher.status());
            return status.commandStatus();
        }
    }

    return (QAbstractStatus::CommandStatus)QUploadStatus::FileNotExists;
}

const QVariant QUploadProfile::upload(QVariant argsVar, const bool bDelete)
{
    QVariantList fileList = argsVar.toList();
    QVariantList::const_iterator i;

    for (i = fileList.constBegin(); i != fileList.constEnd(); ++i) {
        QFileModel fileModel;
        fileModel.fromVariant(*i);
        m_uploadStatus.setCommandStatus(QAbstractStatus::Failed);
        m_uploadStatus.setFileModel(*i);
        QAbstractStatus::CommandStatus uploadSingleFileStat = QAbstractStatus::Failed;
        QFileInfo file(fileModel.pathName());
        quint32 retryCount = 0;

        if (file.exists()) {
            while (m_uploadStatus.commandStatus() != QAbstractStatus::Succeed
                   && retryCount++ < QAbstractCommand::RetryCountLimitation) {
                if (isCompatible(QRserverCompatibility510::FeatureUploadProfileAck)) {
                    uploadSingleFileStat = uploadSingleFile(&fileModel);
                } else {
                    m_uploadStatus.setCommandStatus(uploadSingleFile(
                                                        &fileModel));//If processMessage not supported, overwrite the status directly.
                }

                if (isCompatible(QRserverCompatibility510::FeatureUploadProfileAck)) {
                    if (uploadSingleFileStat != QAbstractStatus::Succeed) {
                        //Connection error should always be considered first.
                        m_uploadStatus.setCommandStatus(
                            uploadSingleFileStat); //If connection error occurs, overwrite the status set by process message function.
                    }
                }

                if (m_uploadStatus.commandStatus() != QAbstractStatus::Succeed) {
                    qprofileDebug(QtWarningMsg) << "Failed to upload file "
                                                << fileModel.toVariant()
                                                << m_uploadStatus.commandStatus()
                                                << "Retry count = " << retryCount;

                    if (!m_uploadStatus.canRecover()) {
                        //Connection error or upload error unrecoverable
                        return m_uploadStatus.toVariant();
                    }
                } else {
                    QVariantList relatedFileList = m_uploadProfileModel->getFileModeList(
                                                       fileModel.fileType(),
                                                       file.completeBaseName());//The fileModel got from UI has no fileName
                    //                    foreach (QVariant fileModelVariant, relatedFileList) {
                    //                        QFileModel relatedFile;
                    //                        relatedFile.fromVariant(fileModelVariant);
                    //                        modifyFileDateTime(relatedFile.pathName());
                    //                    }
                    //conflict with uploading profile info list function
                }
            }

            //Uploading a single file finished, now check the status.
            if (m_uploadStatus.commandStatus() != QAbstractStatus::Succeed) {
                qprofileDebug(QtWarningMsg) << "Failed to upload profile, status =  " <<
                                            m_uploadStatus.commandStatus() << ", file path = " << fileModel.pathName();
                break;//This could break for-loop after RetryCountLimitation times retry for a single file.
            }
        } else {
            m_uploadStatus.setCommandStatus((QAbstractStatus::CommandStatus)
                                            QUploadStatus::FileNotExists);
            break;
        }
    }

    return m_uploadStatus.toVariant();
}

void QUploadProfile::modifyFileDateTime(const QString &filepath)
{
    time_t time_now;
    struct utimbuf uid_time;
    time_now = time(NULL);
    uid_time.actime  = time_now;
    uid_time.modtime = time_now;
    utime(filepath.toLatin1().data(), &uid_time);
}

const QVariant QUploadProfile::absolutePathToFileType(const QString
        absoluteFilePath)
{
    return QRserverProfile::GetFileType(absoluteFilePath);
}

const QString QUploadProfile::getFilterPattern()
{
    return "^[a-zA-z0-9\\_' '\\-]+$";
}

const QVariant QUploadProfile::uploadDelete(QVariant argsVar)
{
#if 0
    bool ok = upload(argsVar);
    QVariantList fileList = argsVar.toList();
    QVariantList::const_iterator i;

    for (i = fileList.constBegin(); i != fileList.constEnd(); ++i) {
        QFileModel fileModel;
        fileModel.fromVariant(*i);
        QFileInfo file(fileModel.pathName());

        if (file.exists() && file.isFile()) {
            if (QFile::remove(fileModel.pathName())) {
                //                m_uploadFileModel.removeFile(fileModel.pathName());
            }
        }
    }

    return ok;
#endif
    QUploadStatus uploadStatus;
    uploadStatus.setCommandStatus(QAbstractStatus::Succeed);
    return uploadStatus.toVariant();
}

void QUploadProfile::processMessage(QAbstractBinMsg
                                    *message)//This function could affect command report status only when uploading single file succeed.
//This is an extension for QUploadProfile::upload to handle report status
{
    Q_ASSERT(message != nullptr);

    switch (message->type()) {
    case rserver::eUploadProfileAck: {
        QMsgUploadProfileAck *pProfileAckMsg = qobject_cast<QMsgUploadProfileAck *>
                                               (message);

        if (pProfileAckMsg != nullptr) {
            qprofileDebug(QtDebugMsg) << "Upload profile status = " <<
                                      pProfileAckMsg->resultStatus();

            switch (pProfileAckMsg->resultStatus()) {
            case QMsgUploadProfileAck::Succeed:
                m_uploadStatus.setCommandStatus(QAbstractStatus::Succeed);
                break;

            case QMsgUploadProfileAck::InvalidEncryptedFile:
                m_uploadStatus.setCommandStatus((QAbstractStatus::CommandStatus)
                                                QUploadStatus::InvalidEncryptedFile);
                break;

            default:
                m_uploadStatus.setCommandStatus((QAbstractStatus::CommandStatus)
                                                QUploadStatus::UploadProfileFailedOtherACK);
                break;
            }
        } else {
            qprofileDebug(QtDebugMsg) << message->toJson();
        }

        break;
    }

    case rserver::eUploadProfileInfoAck: {
        updateProfInfoAckStatus(message);
        break;
    }

    default:
        QUploadFile::processMessage(message);
        break;
    }
}

bool QUploadProfile::processCommands(const QString &commandId,
                                     const QString &commandType, QVariant commandVar, QVariant *status)
{
    if (!commandType.isEmpty()) {
        qprofileDebug(QtDebugMsg) << commandType;
        QUploadStatus uploadProfileInfoListStatus;

        if (commandType == QUploadProfileModel::CommandUploadProfileInfoList) {
            uploadProfileInfoListStatus.fromVariant(uploadProfileInfoList());
        } else {
            return QUploadFile::processCommands(commandId, commandType, commandVar,
                                                status);
        }

        if (status != nullptr) {
            uploadProfileInfoListStatus.setId(commandId);
            uploadProfileInfoListStatus.setMethod(commandType);
            *status = uploadProfileInfoListStatus.toVariant();
        }
    }

    return true;
}

const QVariant QUploadProfile::uploadProfileInfoList()
{
    m_uploadStatus.setCommandStatus(QAbstractStatus::Failed);
    QAbstractStatus::CommandStatus uploadProfInfoStatus = QAbstractStatus::Failed;
    quint32 retryCount = 0;

    while (m_uploadStatus.commandStatus() != QAbstractStatus::Succeed
           && retryCount++ < QAbstractCommand::RetryCountLimitation) {
        uploadProfInfoStatus = sendProfileInfoMsg();

        if (uploadProfInfoStatus != QAbstractStatus::Succeed) {
            m_uploadStatus.setCommandStatus(uploadProfInfoStatus);
        }

        if (m_uploadStatus.commandStatus() != QAbstractStatus::Succeed) {
            if (!m_uploadStatus.canRecover()) {
                return m_uploadStatus.toVariant();
            }
        }
    }

    qprofileDebug(QtWarningMsg) << "Upload profile info status = " <<
                                m_uploadStatus.commandStatus();
    return m_uploadStatus.toVariant();
}

const QAbstractStatus::CommandStatus QUploadProfile::sendProfileInfoMsg()
{
    QVariantList profileInfoList = getProfileInfoList();
    QProcessMessageCommand command(QVariant(), this, false);
    quint32 profileInfoCount = profileInfoList.count();
    qprofileDebug(QtDebugMsg) << "Local profile number = " << profileInfoCount;
    qint32 index = 0;
    quint32 lastSucceedId = 0;
    QGenericBinMessage succeedMsg(rserver::eSendCmdSucceed);
    quint32 messageCount = 1;//QMsgUploadProfileInfoEnd should always be sent.

    while (profileInfoCount > 100) {
        QVariantList profileInfoListPerMessage = profileInfoList.mid(
                    index , QUploadProfileModel::MaxCapPerProfInfoMsg);
        QMsgProfileInfo profileInfoMessage;
        profileInfoMessage.setProfileInfoList(profileInfoListPerMessage);
        command.appendMessage(lastSucceedId, &profileInfoMessage);
        command.appendMessage(profileInfoMessage.id(), &succeedMsg);
        lastSucceedId = succeedMsg.id();
        index += 100;
        profileInfoCount -= 100;
        ++messageCount;
    }

    if (profileInfoCount > 0) {
        QVariantList profileInfoListPerMessage = profileInfoList.mid(
                    index , QUploadProfileModel::MaxCapPerProfInfoMsg);
        QMsgProfileInfo profileInfoMessage;
        profileInfoMessage.setProfileInfoList(profileInfoListPerMessage);
        command.appendMessage(lastSucceedId, &profileInfoMessage);
        command.appendMessage(profileInfoMessage.id(), &succeedMsg);
        lastSucceedId = succeedMsg.id();
        ++messageCount;
    }

    QMsgUploadProfileInfoEnd uploadProfileInfoEndMsg;
    uploadProfileInfoEndMsg.setProfileInfoCount(profileInfoList.count());
    command.appendMessage(lastSucceedId, &uploadProfileInfoEndMsg);
    QMsgUploadProfileInfoAck uploadProfileInfoAck(this, true);
    command.appendMessage(uploadProfileInfoEndMsg.id(), &uploadProfileInfoAck);

    if (0 == publish(command.topic(), command.toJson())) {
        qprofileDebug(QtWarningMsg) << "publish command failed!";
        return QAbstractStatus::IPCError;
    } else {
        QTimerStatusWatcher watcher(this, QClientModel::RserverClientStatusTopic,
                                    command.id(), QUploadFileModel::UploadSingleFileTimeout
                                    * /*this time interval is referenced from sending QMsgFolderInfo*/
                                    messageCount);

        if (watcher.exec() == QTimerStatusWatcher::Timeout) {
            return QAbstractStatus::Timeout;
        }

        QProcessMessageStatus status(watcher.status());
        return status.commandStatus();
    }
}

QVariantList QUploadProfile::getProfileInfoList()
{
    QVariantList fileModelList =  m_uploadProfileModel->fileModelList();
    QVariantList::const_iterator iter;
    QVariantList profileInfoList;
    QVariantMap profileInfoMap;
    QVariantMap::const_iterator i;

    for (iter = fileModelList.begin(); iter != fileModelList.end(); ++iter) {
        QFileModel fileModel;
        fileModel.fromVariant(*iter);
        QString uniqueKey =  QString("%1_%2").arg(fileModel.fileType()).arg(
                                 fileModel.fileName());

        if (profileInfoMap.contains(uniqueKey)) {
            QFileModel preFileModel;
            preFileModel.fromVariant(profileInfoMap.value(uniqueKey));

            if (preFileModel.dateTime() < fileModel.dateTime()) {
                profileInfoMap.insert(uniqueKey, fileModel.toVariant());
            }
        } else {
            profileInfoMap.insert(uniqueKey, fileModel.toVariant());
        }
    }

    for (i = profileInfoMap.begin(); i != profileInfoMap.end(); ++i) {
        QFileModel fileModel;
        fileModel.fromVariant(i.value());
        QVariantMap profileInfo;
        profileInfo[QFileModel::PropertyFileType] = fileModel.fileType();
        profileInfo[QFileModel::PropertyFileName] = fileModel.fileName();
        profileInfo[QFileModel::PropertyDateTime] = fileModel.dateTime();
        QFileInfo fileInfo(fileModel.pathName());
        profileInfo[QFileModel::PropertyLocalSize] = QVariant(
                    fileInfo.size());//If the file does not exist or cannot be fetched, 0 is returned.
        profileInfoList.append(QVariant(profileInfo));
    }

    return profileInfoList;
}

void QUploadProfile::updateProfInfoAckStatus(QAbstractBinMsg *message)
{
    QMsgUploadProfileInfoAck *pUploadProfileInfoAckMsg =
        qobject_cast<QMsgUploadProfileInfoAck *>
        (message);

    if (pUploadProfileInfoAckMsg == nullptr) {
        qprofileDebug(QtDebugMsg) << message->toJson();
        return;
    } else {
        switch (pUploadProfileInfoAckMsg->uploadProfileInfoStatus()) {
        case QMsgUploadProfileInfoAck::Succeed :
            m_uploadStatus.setCommandStatus(QAbstractStatus::Succeed);
            break;

        case QMsgUploadProfileInfoAck::WrongNumber :
            m_uploadStatus.setCommandStatus((QAbstractStatus::CommandStatus)
                                            QUploadStatus::UploadProfInfoWrongNumber);
            break;

        default:
            m_uploadStatus.setCommandStatus((QAbstractStatus::CommandStatus)
                                            QUploadStatus::UploadProfInfoOtherFailedACK);
            break;
        }
    }
}
