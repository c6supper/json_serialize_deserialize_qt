/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "quploadresult.h"
#include "qobjectfactory.h"
#include "qmodelfactory.h"
#include <QFileInfo>
#include "qrserverresult.h"
#include "quploadcommand.h"
#include "qmsgfolderinfo.h"
#include "qmsgfolderinfolongfilename.h"
#include "qmsgfileblock.h"
#include "qmsgfileblocklongfilename.h"
#include "qmsgfilefinish.h"
#include "qmsgfilefinishlongfilename.h"
#include "qprocessmessagecommand.h"
#include "qgenericbinmessage.h"
#include "qmsgsendcmdsucceed.h"
#include "qtimerstatuswatcher.h"
#include "qabstracttopic.h"
#include "qclientmodel.h"
#include "qprocessmessagestatus.h"
#include "qtimerstatuswatcher.h"
#include "qvconnectmodel.h"
#include "qrserverenvironment.h"
#include "qgetresultstatus.h"
#include "qmsguploadresultack.h"
#include "qrservercompatibility405.h"
#include "qvconnectclientmodel.h"
using namespace QProfile;
using namespace QRserver;

QObjectSelfRegisteration<QUploadResult> registerQUploadResult;

QUploadResult::QUploadResult(QObject *parent): QUploadFile(parent)
{
    subscribeMessage(rserver::eUploadResultAck);
}

bool QUploadResult::initPropertiesMap(const QVariantMap &properties)
{
    if (properties.count() <= 0) {
        setProperty("tagName", "quploadresult");
    }

    return QRserverContainer::initPropertiesMap(properties);
}

void QUploadResult::allocModel()
{
    setModel(m_uploadResultModel = QModelFactory::construct<QUploadResultModel>
                                   (this));
    QUploadFile::allocModel();
    m_vconnectResultModel = QModelFactory::construct<QVConnectResultModel>(this);
}

const QVariant QUploadResult::absolutePathToFileType(const QString
        absoluteFilePath)
{
    return QRserverResult::GetFileType(absoluteFilePath);
}

const QString QUploadResult::getFilterPattern()
{
    return QRserverResult::GetFilterPattern();
}

const QAbstractStatus::CommandStatus QUploadResult::uploadResultInfo(
    const QString resultName, const quint32 fileType, const quint32 count)
{
    const QVConnectModel *pVConnectModel =
        QModelFactory::constModel<QVConnectModel>(this);
    bool needBind = true;

    if (pVConnectModel->rserverMode() == QVConnectModel::ModeVConnect) {
        needBind = false;
    }

    QProcessMessageCommand command(QVariant(), this, needBind);

    if (pVConnectModel->rserverMode() == QVConnectModel::ModeVConnect) {
        command.setTopic(QVconnectClientModel::RserverVconnectCommandTopic);
    }

    QMsgFolderInfo *pFolderInfo = nullptr;

    if (isCompatible(QRserverCompatibility405::FeatureLongFileName)) {
        pFolderInfo = new QMsgFolderInfoLongFileName(this);
    } else {
        pFolderInfo = new QMsgFolderInfo(this);
    }

    if (pFolderInfo == nullptr) {
        return (QAbstractStatus::CommandStatus)QUploadStatus::AllocMemoryFailed;
    }

    pFolderInfo->setFolderName(resultName);
    pFolderInfo->setFolderType(fileType);
    pFolderInfo->setFileCount(count);
    command.appendMessage(0, pFolderInfo);
    QMsgSendCmdSucceed succeedMessage(pFolderInfo->type(), this);
    command.appendMessage(pFolderInfo->id(), &succeedMessage);
    pFolderInfo->deleteLater();

    if (0 == publish(command.topic(), command.toJson())) {
        qprofileDebug(QtWarningMsg) << "publish command failed!";
        return QAbstractStatus::IPCError;
    } else {
        QString statusTopic;

        if (pVConnectModel->rserverMode() == QVConnectModel::ModeVConnect) {
            statusTopic = QVconnectClientModel::RserverVconnectStatusTopic;
        } else {
            statusTopic = QClientModel::RserverClientStatusTopic;
        }

        QTimerStatusWatcher watcher(this, statusTopic,
                                    command.id(), QUploadFileModel::UploadSingleFileTimeout);

        if (watcher.exec() == QTimerStatusWatcher::Timeout) {
            qprofileDebug(QtWarningMsg) << "send upload result info timeout " << resultName;
            return QAbstractStatus::Timeout;
        }

        QProcessMessageStatus status(watcher.status());
        return status.commandStatus();
    }
}

bool QUploadResult::isXmlFile(const QString &resultName,
                              const QString &absoluteFilePath)
{
    QFileInfo fileInfo(absoluteFilePath);
    return (fileInfo.completeSuffix().isEmpty()
            || fileInfo.fileName().compare(resultName) == 0);
}

const QAbstractStatus::CommandStatus QUploadResult::uploadSingleFile(
    const QString resultName, const QString absoluteFilePath, const bool isLastFile,
    const quint32 fileType)
{
    QFileInfo fileInfo(absoluteFilePath);
    QFile file(absoluteFilePath);

    if (fileInfo.exists() && fileInfo.isFile() && file.open(QIODevice::ReadOnly)) {
        qint64 length = fileInfo.size();
        qint32 blockIdx = 0;
        bool needBind = true;
        const QVConnectModel *pVConnectModel =
            QModelFactory::constModel<QVConnectModel>(this);

        if (pVConnectModel->rserverMode() == QVConnectModel::ModeVConnect) {
            needBind = false;
        }

        QProcessMessageCommand command(QVariant(), this, needBind);

        if (pVConnectModel->rserverMode() == QVConnectModel::ModeVConnect) {
            command.setTopic(QVconnectClientModel::RserverVconnectCommandTopic);
        }

        quint32 lastSucceedId = 0;

        while (length > QMsgFileBlock::MaxFileBlock) {
            QMsgFileBlock *pResultBlock = nullptr;

            if (isCompatible(QRserverCompatibility405::FeatureLongFileName)) {
                pResultBlock = new QMsgFileBlockLongFileName(this);
            } else {
                pResultBlock = new QMsgFileBlock(this);
            }

            if (pResultBlock == nullptr) {
                return (QAbstractStatus::CommandStatus)QUploadStatus::AllocMemoryFailed;
            }

            pResultBlock->setFolderName(resultName);

            //xml file should not have suffix, and must change to xml suffix for r-server
            if (isXmlFile(resultName, absoluteFilePath)) {
                qprofileDebug(QtDebugMsg) << "Uploading xml file for resultName = " <<
                                          resultName << ", File path name = "
                                          << fileInfo.fileName();
                pResultBlock->setFileName(fileInfo.fileName() + ".xml");
            } else {
                pResultBlock->setFileName(fileInfo.fileName());
            }

            pResultBlock->setBlockIndex(blockIdx++);
            pResultBlock->setBlockLength(QMsgFileBlock::MaxFileBlock);
            pResultBlock->setBlock(file.read((qint64)
                                             QMsgFileBlock::MaxFileBlock).toBase64());
            length -= QMsgFileBlock::MaxFileBlock;
            command.appendMessage(lastSucceedId, pResultBlock);
            QMsgSendCmdSucceed succeedMessage(pResultBlock->type(), this);
            command.appendMessage(pResultBlock->id(), &succeedMessage);
            lastSucceedId = succeedMessage.id();
            pResultBlock->deleteLater();
        }

        QMsgFileBlock *pResultFinish = nullptr;

        if (isCompatible(QRserverCompatibility405::FeatureLongFileName)) {
            pResultFinish = new QMsgFileFinishLongFileName(this);
        } else {
            pResultFinish = new QMsgFileFinish(this);
        }

        if (pResultFinish == nullptr) {
            return (QAbstractStatus::CommandStatus)QUploadStatus::AllocMemoryFailed;
        }

        pResultFinish->setFolderName(resultName);

        //xml file should not have suffix, and must change to xml suffix for r-server
        if (isXmlFile(resultName, absoluteFilePath)) {
            qprofileDebug(QtDebugMsg) << "Uploading xml file for resultName = " <<
                                      resultName << ", File path name = "
                                      << fileInfo.fileName();
            pResultFinish->setFileName(fileInfo.fileName() + ".xml");
        } else {
            pResultFinish->setFileName(fileInfo.fileName());
        }

        pResultFinish->setBlockIndex(blockIdx++);
        pResultFinish->setBlockLength(length);
        pResultFinish->setBlock(file.read(length).toBase64());
        command.appendMessage(lastSucceedId, pResultFinish);

        if (isLastFile && isCompatible(QRserverCompatibility405::FeatureLongFileName)) {
            QMsgUploadResultAck resultAckMessage(this, true);
            resultAckMessage.setResultName(resultName);
            resultAckMessage.setResultType(fileType);
            command.appendMessage(pResultFinish->id(), &resultAckMessage);
        } else {
            QMsgSendCmdSucceed succeedMessage(pResultFinish->type(), this);
            command.appendMessage(pResultFinish->id(), &succeedMessage);
        }

        pResultFinish->deleteLater();

        if (0 == publish(command.topic(), command.toJson())) {
            qprofileDebug(QtWarningMsg) << "publish command failed!";
            return QAbstractStatus::IPCError;
        } else {
            QString statusTopic;

            if (pVConnectModel->rserverMode() == QVConnectModel::ModeVConnect) {
                statusTopic = QVconnectClientModel::RserverVconnectStatusTopic;
            } else {
                statusTopic = QClientModel::RserverClientStatusTopic;
            }

            QTimerStatusWatcher watcher(this, statusTopic, command.id(),
                                        QUploadFileModel::UploadSingleFileTimeout);

            if (watcher.exec() == QTimerStatusWatcher::Timeout) {
                qprofileDebug(QtWarningMsg) << "Upload single file timeout " << resultName;
                return QAbstractStatus::Timeout;
            }

            QProcessMessageStatus status(watcher.status());
            return status.commandStatus();
        }
    }

    return (QAbstractStatus::CommandStatus)QUploadStatus::FileNotExists;
}

const QVariant QUploadResult::upload(QVariant argsVar, const bool bDelete)
{
    QVariantList fileList = argsVar.toList();
    QVariantList::const_iterator i;

    for (i = fileList.constBegin(); i != fileList.constEnd(); ++i) {
        QFileModel fileModel;
        fileModel.fromVariant(*i);
        QFileInfo file(fileModel.pathName());
        QRserverFileType fileType;
        fileType.fromVariant(absolutePathToFileType(fileModel.pathName()));
        m_uploadStatus.setCommandStatus(QAbstractStatus::Failed);
        m_uploadStatus.setFileModel(*i);
        quint32 retryCount = 0;

        if (file.exists()) {
            while (m_uploadStatus.commandStatus() != QAbstractStatus::Succeed
                   && retryCount++ < QAbstractCommand::RetryCountLimitation) {
                QStringList relatedList = QRserverResult::GetRelatedFileList(
                                              fileModel.pathName(), fileType.measureMode(), fileType.measureType());

                if (!relatedList.isEmpty()) {
                    qprofileDebug(QtDebugMsg) << relatedList;
                    m_uploadStatus.setCommandStatus(uploadResultInfo(fileModel.displayName(),
                                                    fileModel.fileType(), relatedList.count()));

                    if (m_uploadStatus.commandStatus() != QAbstractStatus::Succeed) {
                        qprofileDebug(QtWarningMsg) << "Failed to send upload file info " <<
                                                    fileModel.toVariant() << m_uploadStatus.commandStatus()
                                                    << "Retry count = " << retryCount;

                        if (!m_uploadStatus.canRecover()) {
                            return m_uploadStatus.toVariant();
                        }

                        continue;
                    }

                    m_uploadStatus.setCommandStatus(QAbstractStatus::Failed);
                    QAbstractStatus::CommandStatus uploadSingleFileStat = QAbstractStatus::Failed;
                    QList<QString>::const_iterator j;

                    for (j = relatedList.constBegin(); j != relatedList.constEnd(); ++j) {
                        uploadSingleFileStat = uploadSingleFile(fileModel.displayName(), *j,
                                                                j == relatedList.constEnd() - 1, fileModel.fileType());

                        if (uploadSingleFileStat != QAbstractStatus::Succeed) {
                            qprofileDebug(QtWarningMsg) << "Failed to upload file " <<
                                                        fileModel.toVariant() << uploadSingleFileStat
                                                        << "Retry count = " << retryCount;
                            break;
                        }
                    }

                    if (!isCompatible(QRserverCompatibility405::FeatureLongFileName) ||
                        uploadSingleFileStat != QAbstractStatus::Succeed) {
                        m_uploadStatus.setCommandStatus(uploadSingleFileStat);
                    }

                    if (bDelete && m_uploadStatus.commandStatus() == QAbstractStatus::Succeed) {
                        QStringList deletedList = QRserverResult::GetFileListNeedToBeDeleted(
                                                      fileModel.pathName(), fileType.measureMode(), fileType.measureType());

                        if (!deleteResultFiles(deletedList)) {
                            m_uploadStatus.setCommandStatus((QAbstractStatus::CommandStatus)
                                                            QUploadStatus::DeleteResultFailed);
                        }
                    }

                    QRserverResult::postUploading(fileModel.pathName(), fileType.measureMode(),
                                                  fileType.measureType(), m_uploadStatus.commandStatus());

                    if (!m_uploadStatus.canRecover()) {
                        return m_uploadStatus.toVariant();
                    }
                } else {
                    qprofileDebug(QtWarningMsg) <<
                                                "Trying to upload result with no files related it. " << fileModel.displayName();
                    m_uploadStatus.setCommandStatus((QAbstractStatus::CommandStatus)
                                                    QUploadStatus::CannotGetRelatedFiles);
                    return m_uploadStatus.toVariant();
                }
            }

            if (m_uploadStatus.commandStatus() != QAbstractStatus::Succeed) {
                qprofileDebug(QtWarningMsg) << "Failed to upload result, status =  " <<
                                            m_uploadStatus.commandStatus() << ", file path = " << fileModel.pathName();
                break;
            }
        } else {
            m_uploadStatus.setCommandStatus((QAbstractStatus::CommandStatus)
                                            QUploadStatus::FileNotExists);
            break;
        }
    }

    return m_uploadStatus.toVariant();
}

const QVariant QUploadResult::uploadDelete(QVariant argsVar)
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

bool QUploadResult::processCommands(const QString &commandId,
                                    const QString &commandType, QVariant commandVar, QVariant *status)
{
    if (!commandType.isEmpty()) {
        qprofileDebug(QtDebugMsg) << commandType;
        QGetResultStatus getResultStatus;

        if (commandType == QVConnectResultModel::CommandGetResultList) {
            getResultStatus.fromVariant(getResultList());
        } else {
            return QUploadFile::processCommands(commandId, commandType, commandVar,
                                                status);
        }

        if (status != nullptr) {
            getResultStatus.setId(commandId);
            getResultStatus.setMethod(commandType);
            getResultStatus.setVConnectResultModel(m_vconnectResultModel->toVariant());
            qprofileDebug(QtDebugMsg) << m_vconnectResultModel->toJson() <<
                                      getResultStatus.toVariant();
            *status = getResultStatus.toVariant();
        }
    }

    return true;
}

void QUploadResult::processMessage(QAbstractBinMsg *message)
{
    Q_ASSERT(message != nullptr);

    switch (message->type()) {
    case rserver::eUploadResultAck: {
        QMsgUploadResultAck *pResultAckMsg = qobject_cast<QMsgUploadResultAck *>
                                             (message);

        if (pResultAckMsg != nullptr) {
            qprofileDebug(QtDebugMsg) << "Upload result status = " <<
                                      pResultAckMsg->resultStatus();

            switch (pResultAckMsg->resultStatus()) {
            case QMsgUploadResultAck::Succeed:
                m_uploadStatus.setCommandStatus(QAbstractStatus::Succeed);
                break;

            case QMsgUploadResultAck::XmlError:
                m_uploadStatus.setCommandStatus((QAbstractStatus::CommandStatus)
                                                QUploadStatus::UploadXmlError);
                break;

            default:
                m_uploadStatus.setCommandStatus((QAbstractStatus::CommandStatus)
                                                QUploadStatus::UploadResultFailed);
                break;
            }
        } else {
            qprofileDebug(QtDebugMsg) << message->toJson();
        }

        break;
    }

    default:
        QUploadFile::processMessage(message);
        break;
    }
}

const QVariant QUploadResult::getResultList()
{
    quint32 retryCount = 0;
    QGetResultStatus getResultStatus;

    while (getResultStatus.commandStatus() != QAbstractStatus::Succeed
           && retryCount++ < QAbstractCommand::RetryCountLimitation) {
        getResultStatus.fromVariant(QUploadFile::getFileList());
    }

    if (getResultStatus.commandStatus() != QAbstractStatus::Succeed) {
        m_vconnectResultModel->clear();
    } else {
        cpUploadFileModel2VConnectResultModel();
    }

    return getResultStatus.toVariant();
}

void QUploadResult::cpUploadFileModel2VConnectResultModel()
{
    QVariantList resultModelList;
    QUploadFileModel *uploadFileModel = qobject_cast<QUploadFileModel *>
                                        (QUploadFile::model());
    QVariantList fileModelList = uploadFileModel->fileModelList();
    QVariantList::const_iterator i;

    for (i = fileModelList.constBegin(); i != fileModelList.constEnd(); ++i) {
        QFileModel fileModel;
        fileModel.fromVariant(*i);
        QStringList relatedList = QRserverResult::GetRelatedFileList(
                                      fileModel.pathName(), fileModel.measureMode(), fileModel.measureType());

        if (!relatedList.isEmpty()) {
            QResultModel resultModel;
            resultModel.fromVariant(fileModel.toVariant());
            resultModel.setRelatedFileList(relatedList);
            resultModelList.append(resultModel.toVariant());
        }
    }

    m_vconnectResultModel->setResultModelFileList(resultModelList);
}

bool QUploadResult::deleteResultFiles(const QStringList &fileList)
{
    QList<QString>::const_iterator j;

    for (j = fileList.constBegin(); j != fileList.constEnd(); ++j) {
        if (QFile::exists(*j)) {
            quint32 removeRetry = 0;

            while (removeRetry++ < QAbstractCommand::RetryCountLimitation) {
                if (QFile::remove(*j)) {
                    break;
                }
            }

            if (removeRetry > QAbstractCommand::RetryCountLimitation) {
                return false;
            }
        }
    }

    return true;
}

