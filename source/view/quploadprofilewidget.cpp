#include "quploadprofilewidget.h"
#include "quploadprofilemodel.h"
#include "qmodelfactory.h"
#include "quploadcommand.h"
#include "qgenericcommand.h"
#include "qrservermessagebox.h"
#include "qrserverstrtable.h"
#include "quploadstatus.h"

using namespace QRserver;

QUploadProfileWidget::QUploadProfileWidget(QWidget *parent,
        const QString &tagName)
    : QUploadFileWidget(parent, tagName)
{
}

QUploadProfileWidget::~QUploadProfileWidget()
{
}

void QUploadProfileWidget::onUpload()
{
    QVariantList uploadList = fileModelList();

    if (!isVisible() || uploadList.count() <= 0)
        return;

    QUploadCommand uploadCommand(this, QUploadFileModel::CommandUpload,
                                 QVariant(uploadList));
    QVariant statusVar = issueCommand(uploadCommand.toVariant(),
                                      QUploadFileModel::UploadSingleFileTimeout * uploadList.count() *
                                      QAbstractCommand::RetryCountLimitation * 2, "");

    if (!statusVar.isNull()) {
        QUploadStatus status(statusVar);
        QFileModel fileMode;
        fileMode.fromVariant(status.fileModel());

        switch (status.commandStatus()) {
        case QAbstractStatus::Succeed:
            QRserverMessageBox::Show(this, TRANSLATE_STR_RSERVER_INFORMATION,
                                     TRANSLATE_STR_RSERVER_UPLOAD_SUCCESS, QMessageBox::Ok);
            break;

        default:
            QString message = QString("%1\n%2(%3)\n%4(%5)").arg(
                                  TRANSLATE_STR_RSERVER_UPLOAD_FAILED.arg(status.commandStatus())).arg(
                                  TRANSLATE_STR_RSERVER_UPLOAD_TYPE).arg(fileMode.typeName()).arg(
                                  TRANSLATE_STR_RSERVER_UPLOAD_NAME).arg(fileMode.displayName());
            QRserverMessageBox::Show(this, TRANSLATE_STR_RSERVER_WARNING, message,
                                     QMessageBox::Ok);
            break;
        }
    }
}

void QUploadProfileWidget::allocModel()
{
    setModel(QModelFactory::construct<QUploadProfileModel>(this));
}

void QUploadProfileWidget::onGetProfileInfoList()
{
    QGenericCommand genericCommand(this, QUploadFileModel::CommandGetFileList);
    QVariant statusVar = issueCommand(genericCommand.toVariant(),
                                      QUploadFileModel::GetFileListTimeout, "");

    if (!statusVar.isNull()) {
        QUploadStatus status(statusVar);

        switch (status.commandStatus()) {
        case QAbstractStatus::Succeed:
            uploadProfileInfoList();
            break;

        default:
            qprofileDebug(QtWarningMsg) << "Failed to get profile info list";
            break;
        }
    }
}

void QUploadProfileWidget::uploadProfileInfoList()
{
    const QUploadProfileModel *uploadProfileModel =
        qobject_cast<QUploadProfileModel *>(model());
    quint32 profileCount = uploadProfileModel->fileModelList().count();
    quint32 messageCount = 1;//QMsgUploadProfileInfoEnd should always be sent.
    messageCount += ((profileCount / QUploadProfileModel::MaxCapPerProfInfoMsg)
                     + (profileCount % QUploadProfileModel::MaxCapPerProfInfoMsg == 0 ? 0 : 1));
    QGenericCommand genericCommand(this,
                                   QUploadProfileModel::CommandUploadProfileInfoList);
    QVariant statusVar = issueCommand(genericCommand.toVariant(),
                                      QUploadFileModel::UploadSingleFileTimeout * messageCount *
                                      QAbstractCommand::RetryCountLimitation * 2, "");

    if (!statusVar.isNull()) {
        QUploadStatus status(statusVar);

        switch (status.commandStatus()) {
        case QAbstractStatus::Succeed:
            qprofileDebug(QtWarningMsg) <<
                                        "Uploading profile info list successfully...";
            break;

        default:
            qprofileDebug(QtWarningMsg) << "Failed to upload profile info list";
            break;
        }
    }
}
