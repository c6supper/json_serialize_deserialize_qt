/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "quploadfile.h"
#include "quploadfilemodel.h"
#include "qmodelfactory.h"
#include <QProcess>
#include <QFileInfo>
#include "qrserverenvironment.h"
#include "quploadstatus.h"

using namespace QProfile;
using namespace QRserver;

const QString QUploadFile::m_mlocateDataFile = "mlocate.db";

QUploadFile::QUploadFile(QObject *parent): QRserverContainer(parent, false),
    m_uploadFileModel(nullptr)
{
    subscribeMessage(rserver::eSendCmdSucceed);
}

bool QUploadFile::processCommands(const QString &commandId,
                                  const QString &commandType, QVariant commandVar, QVariant *status)
{
    if (!commandType.isEmpty()) {
        qprofileDebug(QtDebugMsg) << commandType;
        QUploadStatus uploadStatus;

        if (commandType == QUploadFileModel::CommandGetFileList) {
            uploadStatus.fromVariant(getFileList());
        } else if (commandType == QUploadFileModel::CommandUpload) {
            uploadStatus.fromVariant(upload(commandVar, false));
        } else if (commandType == QUploadFileModel::CommandUploadDelete) {
            uploadStatus.fromVariant(upload(commandVar, true));
        } else {
            return true;
        }

        if (status != nullptr) {
            uploadStatus.setId(commandId);
            uploadStatus.setMethod(commandType);
            *status = uploadStatus.toVariant();
        }
    }

    return true;
}

const QVariant QUploadFile::getFileList()
{
    QUploadStatus uploadStatus;
    QStringList arguments = QStringList() << "-U" <<
                            QRserverEnvironment::StorageRoot() << "-o" <<
                            QRserverEnvironment::StorageRserver() + m_mlocateDataFile;

    if (!QRserverEnvironment::TouchRserverFile(m_mlocateDataFile)) {
        uploadStatus.setCommandStatus((QAbstractStatus::CommandStatus)
                                      QUploadStatus::TouchMlocateDataFileFailed);
        qprofileDebug(QtWarningMsg) << "Failed to touch the " << m_mlocateDataFile;
        return uploadStatus.toVariant();
    }

    if (0 > QProcess::execute("updatedb", arguments)) {
        uploadStatus.setCommandStatus((QAbstractStatus::CommandStatus)
                                      QUploadStatus::UpatedbFailed);
        qprofileDebug(QtWarningMsg) << "updatedb failed!";
        return uploadStatus.toVariant();
    }

    QProcess locateProcess;
    arguments.clear();
    arguments << "-bie" << "--regex" << getFilterPattern() << "-d" <<
              QRserverEnvironment::StorageRserver() + m_mlocateDataFile;
    qprofileDebug(QtDebugMsg) << arguments;
    locateProcess.start("locate", arguments);

    if (locateProcess.waitForFinished()) {
        locateProcess.setReadChannel(QProcess::StandardOutput);
        QVariantList fileModelList;

        while (locateProcess.canReadLine()) {
            QString line = QString(locateProcess.readLine());
            QFileInfo file(line.trimmed());

            if (file.exists()) {
                QFileModel fileModel;
                QVariant fileType = absolutePathToFileType(file.absoluteFilePath());

                if (fileType.isValid()) {
                    fileModel.fromVariant(fileType);

                    if (fileModel.displayName().isEmpty()) {
                        fileModel.setDisplayName(file.completeBaseName());
                    }

                    fileModel.setFileName(file.completeBaseName());
                    fileModel.setPathName(file.absoluteFilePath());
                    fileModel.setDateTime(file.lastModified());
                    fileModelList.append(fileModel.toVariant());
                }
            }
        }

        m_uploadFileModel->setFileModelList(fileModelList);
    }

    uploadStatus.setCommandStatus(QAbstractStatus::Succeed);
    return uploadStatus.toVariant();
}

void QUploadFile::processMessage(QAbstractBinMsg *message)
{
    Q_ASSERT(message != nullptr);

    switch (message->type()) {
    case rserver::eSendCmdSucceed:
        break;

    /*
        case rserver::eSendCmdFail:
            break;*/

    default:
        break;
    }
}

void QUploadFile::allocModel()
{
    m_uploadFileModel = qobject_cast<QUploadFileModel *>(model());
    Q_ASSERT(m_uploadFileModel != nullptr);
}

