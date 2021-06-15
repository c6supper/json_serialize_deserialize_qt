
#include "qdownloadwidget.h"
#include <QVBoxLayout>
#include "qrserverstrtable.h"
#include <QHeaderView>
#include "qgenericcommand.h"
#include "qdownloadcommand.h"
#include "qmodelfactory.h"
#include "qdownloadfilemodel.h"
#include "qtimerstatuswatcher.h"
#include "qrservermessagebox.h"
#include "qdownloadstatus.h"
#include <QSet>
#include "qlockdowncommand.h"
#include "qadminsettingprofile.h"
#include "qrserverprofile.h"
#include "abstractlockdownsetting.h"
#include "qdetailinfobox.h"

using namespace QRserver;
using namespace QProfile;

QDownloadWidget::QDownloadWidget(QWidget *parent, const QString &tagName)
    : QRserverContainerWidget(parent)
{
    setTagName(tagName);
    setFixedSize(parent->width(), parent->height() - TAB_ROW_HEIGHT);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);
    m_profileTableWidget = new QDownloadProfileTableWidget(this);
    m_profileTableWidget->horizontalHeader()->hide();
    m_profileTableWidget->setColumnCount(2);
    m_profileTableWidget->setShowGrid(true);
    m_profileTableWidget->setMaxVisibleRows(10);
    layout->addWidget(m_profileTableWidget);
    m_pLabLockDown = new QLabel(this);
    m_pLabLockDown->setObjectName("LockDownInfo");
    m_pLabLockDown->setText(TRANSLATE_STR_RSERVER_DOWNLOAD_LOCKINFO);
    m_pLabLockDown->setTextInteractionFlags(Qt::LinksAccessibleByMouse);
    m_pLabLockDown->setVisible(QAdminSettingProfile::Instance()->isLockDown());
    connect(m_pLabLockDown, SIGNAL(linkActivated(const QString &)), this,
            SLOT(onShowLockedTestType()));
    layout->addWidget(m_pLabLockDown);
    layout->addStretch();
    QDetailInfoBox::Initialize(this);
}

void QDownloadWidget::allocModel()
{
    setModel(m_downloadFileModel = QModelFactory::construct<QDownloadFileModel>
                                   (this));
}

void QDownloadWidget::onGetChangeList()
{
    //    m_profileTableWidget->clear();
    //    QGenericCommand genericCommand(this, QDownloadFileModel::CommandGetChangeList);
    //    issueCommand(genericCommand.toVariant());
}

void QDownloadWidget::onDownloadAll()
{
    if (QAdminSettingProfile::Instance()->isMaintenance()) {
        QRserverMessageBox::Information(this, TRANSLATE_STR_RSERVER_INFORMATION,
                                        TRANSLATE_STR_RSERVER_DOWNLOAD_MAINTENANCE, QMessageBox::Ok);
        return;
    }

    QVariantList downloadList = downloadFileModelList();

    if (downloadList.count() < 1) {
        QRserverMessageBox::Show(this, TRANSLATE_STR_RSERVER_INFORMATION,
                                 TRANSLATE_STR_RSERVER_DOWNLOAD_SUCCESS, QMessageBox::Ok);
        return;
    }

    QDownloadCommand downloadCommand(this, QDownloadFileModel::CommandDownloadAll,
                                     QVariant(downloadList));
    QVariant statusVar = issueCommand(downloadCommand.toVariant(),
                                      downloadList.count() *
                                      QDownloadFileModel::DownloadSingleFileTimeout *
                                      QAbstractCommand::RetryCountLimitation * 2, "");

    if (!statusVar.isNull()) {
        QDownloadStatus status(statusVar);
        QFileModel fileMode;
        fileMode.fromVariant(status.fileModel());

        switch (status.commandStatus()) {
        case QAbstractStatus::Succeed:
            QRserverMessageBox::Show(this, TRANSLATE_STR_RSERVER_INFORMATION,
                                     TRANSLATE_STR_RSERVER_DOWNLOAD_SUCCESS, QMessageBox::Ok);
            break;

        default:
            QString message = QString("%1\n%2(%3)\n%4(%5)").arg(
                                  TRANSLATE_STR_RSERVER_DOWNLOAD_FAILED.arg(
                                      status.commandStatus())).arg(
                                  TRANSLATE_STR_RSERVER_UPLOAD_TYPE).arg(fileMode.typeName()).arg(
                                  TRANSLATE_STR_RSERVER_UPLOAD_NAME).arg(fileMode.fileName());
            QRserverMessageBox::Show(this, TRANSLATE_STR_RSERVER_WARNING, message,
                                     QMessageBox::Ok);
            break;
        }
    }
}

void QDownloadWidget::onLockDown(bool bLock)
{
    QLockDownCommand lockDownCommand(this, QDownloadFileModel::CommandLockDown,
                                     QVariant(bLock));
    QVariant statusVar = issueCommand(lockDownCommand.toVariant(),
                                      QDownloadFileModel::DownloadSingleFileTimeout *
                                      QAbstractCommand::RetryCountLimitation, "");

    if (!statusVar.isNull()) {
        QDownloadStatus status(statusVar);

        switch (status.commandStatus()) {
        case QAbstractStatus::Succeed:
            qprofileDebug(QtDebugMsg) << "lockDown succeed.";
            //            m_pWgtLockDown->setVisible(bLock);
            break;

        default:
            QString message = QString(TRANSLATE_STR_RSERVER_DOWNLOAD_LOCKDOWNFAILDE).arg(
                                  status.commandStatus());
            QRserverMessageBox::Show(this, TRANSLATE_STR_RSERVER_WARNING, message,
                                     QMessageBox::Ok);
            break;
        }
    }
}

void QDownloadWidget::onShowLockedTestType()
{
    QDetailInfoBox::Exec(this, QRserverProfile::GetDeviceRelatedProfilePathList());
}

void QDownloadWidget::onCheckDeviceSucceed()
{
    if (QAdminSettingProfile::Instance()->isLockDown()) {
        getBackProgress()->setText(TRANSLATE_STR_RSERVER_DOWNLOADING_IN_LOCKDOWN_MODE);
        onDownloadAll();
        getBackProgress()->setText(TRANSLATE_STR_RSERVER_PROGRESS);
    }

    onLockDown(QAdminSettingProfile::Instance()->isLockDown());
    m_pLabLockDown->setVisible(QAdminSettingProfile::Instance()->isLockDown());
}

const QVariantList QDownloadWidget::downloadFileModelList() const
{
    if (QAdminSettingProfile::Instance()->isLockDown()) {
        return m_downloadFileModel->fileModelList();
    }

    QVariantList downloadFileModelList;
    QVariantList fileModelList = m_downloadFileModel->fileModelList();
    QVariantList::const_iterator i, j;
    QSet<QString> uniqueList;

    for (i = fileModelList.constBegin(); i != fileModelList.constEnd(); ++i) {
        QFileModel fileModel;
        fileModel.fromVariant(*i);

        if (fileModel.isStatusNew()) {
            QString uniqueKey = QString("%1_%2").arg(fileModel.fileType()).arg(
                                    fileModel.fileName());

            if (uniqueList.contains(uniqueKey)) {
                continue;
            } else {
                uniqueList.insert(uniqueKey);
                downloadFileModelList.append(fileModel.toVariant());
            }
        }
    }

    return downloadFileModelList;
}

const QVariantList QDownloadWidget::fileModelList() const
{
    return m_downloadFileModel->fileModelList();
}

void QDownloadWidget::setFileModelList(const QVariantList &fileModelList)
{
    QVariantList localFileModelList;
    QVariantMap measureType2StatusMap;
    QVariantList::const_iterator i;

    for (i = fileModelList.constBegin(); i != fileModelList.constEnd(); ++i) {
        QFileModel fileModel;
        fileModel.fromVariant(*i);

        if (measureType2StatusMap.contains(fileModel.measureType())) {
            if (measureType2StatusMap[fileModel.measureType()] ==
                QDownloadFileModel::FileStatusNew)
                continue;
        }

        if (fileModel.isStatusNew()) {
            measureType2StatusMap[fileModel.measureType()] =
                QDownloadFileModel::FileStatusNew;
        } else {
            measureType2StatusMap[fileModel.measureType()] =
                QDownloadFileModel::FileStatusCurrent;
        }
    }

    QVariantMap::const_iterator j;

    for (j = measureType2StatusMap.constBegin();
         j != measureType2StatusMap.constEnd(); ++j) {
        QVariantMap itemMap;
        itemMap.insert(QFileModel::PropertyMeasureType,
                       QString("%1 Profile").arg(j.key()));
        itemMap.insert(QDownloadFileModel::FileStatus, j.value());
        localFileModelList.append(itemMap);
    }

    m_profileTableWidget->clear();
    m_profileTableWidget->addItems(localFileModelList);
}

void QDownloadWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    m_profileTableWidget->setColumnWidth(0, (int)(width() * 0.5));
    m_profileTableWidget->setColumnWidth(1, (int)(width() * 0.5));
    m_profileTableWidget->adjustHeight();
}

void QDownloadWidget::translate(const QFont &font)
{
    m_profileTableWidget->setFont(font);
}

void QDownloadWidget::allocBackProgress()
{
}

QDownloadWidget::~QDownloadWidget()
{
}

