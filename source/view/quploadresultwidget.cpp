#include "quploadresultwidget.h"
#include "quploadresultmodel.h"
#include "qmodelfactory.h"
#include "qgenericcommand.h"
#include "quploaddeletecommand.h"
#include "qrservermessagebox.h"
#include "qrserverstrtable.h"
#include "quploadstatus.h"
#include "qresultfilterdialog.h"

using namespace QRserver;

QUploadResultWidget::QUploadResultWidget(QWidget *parent,
        const QString &tagName)
    : QUploadFileWidget(parent, tagName)
{
}

QUploadResultWidget::~QUploadResultWidget()
{
}

void QUploadResultWidget::onUpload()
{
    QVariantList uploadList = fileModelList();

    if (!isVisible() || uploadList.count() <= 0)
        return;

    QUploadCommand uploadCommand(this, QUploadFileModel::CommandUpload,
                                 QVariant(uploadList));
    upload(uploadCommand.toVariant(), uploadList);
}

void QUploadResultWidget::onJob()
{
    //get All jobids
    QStringList jobIdList = getColumnValueList(
                                QUploadFileTableWidget::ColumnIndexJobId);
    //get all locations
    QStringList locationList = getColumnValueList(
                                   QUploadFileTableWidget::ColumnIndexLocation);
    //show filter dialog
    QVariant varFilter = QResultFilterDialog::Exec(this, jobIdList, locationList);
    //get filter key
    QVariantMap varFilterMap = varFilter.toMap();
    qprofileDebug(QtDebugMsg) << "filter key(jobId/location):"
                              << varFilterMap[QFileModel::PropertyJobId].toString()
                              << varFilterMap[QFileModel::PropertyLocation].toString();
    //set filter pattern
    setColumnFilterPattern(QUploadFileTableWidget::ColumnIndexJobId,
                           varFilterMap[QFileModel::PropertyJobId].toString());
    setColumnFilterPattern(QUploadFileTableWidget::ColumnIndexLocation,
                           varFilterMap[QFileModel::PropertyLocation].toString());
}

void QUploadResultWidget::onUploadAndDel()
{
    QVariantList uploadList = fileModelList();

    if (!isVisible() || uploadList.count() <= 0)
        return;

    QUploadDeleteCommand uploadDeleteCommand(this,
            QUploadFileModel::CommandUploadDelete,
            QVariant(uploadList));
    upload(uploadDeleteCommand.toVariant(), uploadList);
    onGetFileList();
}

void QUploadResultWidget::allocModel()
{
    setModel(QModelFactory::construct<QUploadResultModel>(this));
}

void QUploadResultWidget::upload(const QVariant &commandVar,
                                 const QVariantList &uploadList)
{
    QVariant statusVar = issueCommand(commandVar,
                                      QUploadFileModel::UploadSingleFileTimeout * uploadList.count() *
                                      QAbstractCommand::RetryCountLimitation * 2 * 8, "");

    if (!statusVar.isNull()) {
        QUploadStatus status(statusVar);
        QFileModel fileMode;
        fileMode.fromVariant(status.fileModel());

        switch (status.commandStatus()) {
        case QAbstractStatus::Succeed:
            QRserverMessageBox::Information(this, TRANSLATE_STR_RSERVER_INFORMATION,
                                            TRANSLATE_STR_RSERVER_UPLOAD_SUCCESS, QMessageBox::Ok);
            break;

        default:
            QString message = QString("%1\n%2(%3)\n%4(%5)").arg(
                                  TRANSLATE_STR_RSERVER_UPLOAD_FAILED.arg(status.commandStatus())).arg(
                                  TRANSLATE_STR_RSERVER_UPLOAD_TYPE).arg(fileMode.typeName()).arg(
                                  TRANSLATE_STR_RSERVER_UPLOAD_NAME).arg(fileMode.displayName());
            QRserverMessageBox::Information(this, TRANSLATE_STR_RSERVER_WARNING, message,
                                            QMessageBox::Ok);
            break;
        }
    }
}

