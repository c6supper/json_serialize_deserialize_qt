#include "qrserverwidget.h"
#include "qprofile_debug.h"
#include "qrserverstrtable.h"
#include "qrservermodel.h"
#include "qmodelfactory.h"
#include "qrservermessagebox.h"
#include "qrserverdevicetype.h"
#include "qautocheckevent.h"
#include "qrserverenvironment.h"
#include "qgenericcommand.h"
#include "qgenericstatus.h"
#include <QFileInfo>
#include "qadminsettingprofile.h"
#include "qrservercompatibility518.h"

using namespace QRserver;

#define RIGHT_TOOL_WIDTH (139)

const qint32 QRserverWidget::CheckRegistrationBombInterval = 10 * 60 * 1000;

QRserverWidget::QRserverWidget(QWidget *parent, const QString &tagName)
    : QRserverContainerWidget(parent),
      m_vconnectModel(nullptr),
      m_uploadTabCurrentIdx(QUploadWidget::RserverTabUploadResult)

{
    setObjectName("QRserverWidget");
    setTagName(tagName);
    setFixedSize(parent->width(), parent->height());
    //load admin setting from json file..
    QAdminSettingProfile::Instance()->loadProfile();
    QHBoxLayout *pMainLayout = new QHBoxLayout(this);
    pMainLayout->setMargin(0);
    pMainLayout->setSpacing(0);
    QHBoxLayout *pTabLayout = new QHBoxLayout;
    pTabLayout->setAlignment(Qt::AlignLeft);
    pTabLayout->setMargin(0);
    pTabLayout->setSpacing(0);
    m_ptab = new QTabWidget(this);
    m_ptab->setObjectName("QRserverQTab");
    m_ptab->setFixedSize(width() - RIGHT_TOOL_WIDTH, height());
    m_pRegister = new QRegisterWidget(m_ptab);
    m_pUploadWidget = new QUploadWidget(m_ptab);
    m_pDownloadWidget = new QDownloadWidget(m_ptab);
    m_pSoftwareWidget = new QSoftwareWidget(m_ptab);
    m_ptab->addTab(m_pRegister, TRANSLATE_STR_RSERVER_TAB_REGISTER);
    m_ptab->addTab(m_pUploadWidget, TRANSLATE_STR_RSERVER_TAB_UPLOAD);
    m_ptab->addTab(m_pDownloadWidget, TRANSLATE_STR_RSERVER_TAB_DOWNLOAD);
    m_ptab->addTab(m_pSoftwareWidget, TRANSLATE_STR_RSERVER_TAB_SOFTWARE);
    QWidget *pWidget = new QWidget(this);
    pWidget->setObjectName("ToolboxBtnWidget");
    pWidget->setFixedSize(RIGHT_TOOL_WIDTH, height());
    m_pBtnRegister = new QPushButton(this);
    m_pBtnRegister->setText(TRANSLATE_STR_RSERVER_BTN_REGISTER);
    m_pBtnUnRegister = new QPushButton(this);
    m_pBtnUnRegister->setText(TRANSLATE_STR_RSERVER_BTN_UNREGISTER);
    m_pBtnCheck = new QPushButton(this);
    m_pBtnCheck->setText(TRANSLATE_STR_RSERVER_BTN_CHECK);
    m_pBtnRServerMode = new QPushButton(this);
    m_pBtnRServerMode->setText(TRANSLATE_STR_RSERVER_BTN_MODE_VCONNECT);
    m_pBtnRServerMode->hide();
    m_pBtnUpload = new QPushButton(this);
    m_pBtnUpload->setText(TRANSLATE_STR_RSERVER_BTN_UPLOAD);
    m_pBtnUpload->hide();
    m_pBtnJob = new QPushButton(this);
    m_pBtnJob->setText(TRANSLATE_STR_RSERVER_BTN_JOB);
    m_pBtnJob->hide();
    m_pBtnUploadAndDel = new QPushButton(this);
    m_pBtnUploadAndDel->setText(TRANSLATE_STR_RSERVER_BTN_UPLOAD_DEL);
    m_pBtnUploadAndDel->hide();
    m_pBtnDownload = new QPushButton(this);
    m_pBtnDownload->setText(TRANSLATE_STR_RSERVER_BTN_DOWNLOAD);
    m_pBtnDownload->hide();
    m_pBtnUpgrade = new QPushButton(this);
    m_pBtnUpgrade->setText(TRANSLATE_STR_RSERVER_BTN_UPGRADE);
    m_pBtnUpgrade->hide();
    m_pBtnUpdate = new QPushButton(this);
    m_pBtnUpdate->setText(TRANSLATE_STR_RSERVER_BTN_UPDATE);
    m_pBtnUpdate->hide();
    m_pBtLayout = new QVBoxLayout(pWidget);
    m_pBtLayout->setAlignment(Qt::AlignHCenter);
    m_pBtLayout->setMargin(0);
    m_pBtLayout->addSpacing(30);
    m_pBtLayout->addWidget(m_pBtnRegister);
    m_pBtLayout->addWidget(m_pBtnUnRegister);
    m_pBtLayout->addWidget(m_pBtnCheck);
    m_pBtLayout->addWidget(m_pBtnUpload);
    m_pBtLayout->addWidget(m_pBtnJob);
    m_pBtLayout->addWidget(m_pBtnUploadAndDel);
    m_pBtLayout->addWidget(m_pBtnDownload);
    m_pBtLayout->addWidget(m_pBtnUpgrade);
    m_pBtLayout->addWidget(m_pBtnUpdate);
    m_pBtLayout->addSpacing(30);
    m_pBtLayout->addWidget(m_pBtnRServerMode);
    m_pBtLayout->addStretch();
    pTabLayout->addWidget(m_ptab);
    pMainLayout->addLayout(pTabLayout);
    pMainLayout->addWidget(pWidget);
    connect(m_ptab, SIGNAL(currentChanged(int)), this, SLOT(onTabChange(int)));
    connect(m_pBtnUnRegister, SIGNAL(clicked(bool)), this,
            SLOT(onUnregister()));
    connect(m_pBtnUpload, SIGNAL(clicked(bool)), m_pUploadWidget,
            SIGNAL(onUploadBtnClicked()));
    connect(m_pBtnJob, SIGNAL(clicked(bool)), m_pUploadWidget,
            SIGNAL(onJobBtnClicked()));
    connect(m_pBtnUploadAndDel, SIGNAL(clicked(bool)), m_pUploadWidget,
            SIGNAL(onUploadAndDelBtnClicked()));
    connect(m_pBtnDownload, SIGNAL(clicked(bool)), m_pDownloadWidget,
            SLOT(onDownloadAll()));
    connect(m_pBtnUpgrade, SIGNAL(clicked(bool)), m_pSoftwareWidget,
            SLOT(onUpgrade()));
    connect(this, SIGNAL(registrationBombExpired()), m_pRegister,
            SLOT(onRegistrationBombExpired()));
    connect(m_pRegister, SIGNAL(serverAddressChanged()), this,
            SLOT(onRestoreDefault()));
    connect(m_pRegister, SIGNAL(upateDeviceInfoSucceed()), this,
            SLOT(onUpateDeviceInfoSucceed()));
    connect(m_pBtnUpdate, SIGNAL(clicked(bool)), m_pRegister,
            SLOT(onUpdate()));
    connect(m_pUploadWidget, SIGNAL(currentTabChanged(int)), this,
            SLOT(onUploadWidgetTabChanged(int)));
    connect(this, SIGNAL(lockUploadProfileTab(bool)), m_pUploadWidget,
            SLOT(onSetTab(bool)));
    connect(m_pRegister, SIGNAL(checkDeviceSucceed()), m_pUploadWidget,
            SIGNAL(checkDeviceSucceed()));
    connect(m_pRegister, SIGNAL(checkDeviceSucceed()), this,
            SLOT(onCheckDeviceSucceed()));
    connect(m_pBtnRServerMode, SIGNAL(clicked(bool)), this,
            SLOT(onRServerModeChanged()));
    connect(m_pRegister, SIGNAL(checkDeviceSucceed()), m_pDownloadWidget,
            SLOT(onCheckDeviceSucceed()));
    connect(m_pRegister, SIGNAL(getProfileInfoList()), m_pUploadWidget,
            SIGNAL(getProfileInfoList()));
    //
    QFileInfo vconncetServerConfig(QRserverEnvironment::VConncetServerConfig());
    m_fileSystemWatcher.addPath(vconncetServerConfig.absolutePath());
    connect(&m_fileSystemWatcher, SIGNAL(directoryChanged(const QString)), this,
            SLOT(onDirectoryChanged(const QString)));
    lockTab(true);
    //check registrationBomb
    m_registrationBombTimer.setSingleShot(false);
    m_registrationBombTimer.setInterval(CheckRegistrationBombInterval);
    connect(&m_registrationBombTimer, SIGNAL(timeout()), this,
            SLOT(onCheckRegistrationBomb()));
    m_registrationBombTimer.start();
#ifdef BUILD_X86
    setEnabled(false);
#endif
}

void QRserverWidget::onCheckRegistrationBomb()
{
    if (QAdminSettingProfile::Instance()->isRegistrationBombExpired()) {
        m_registrationBombTimer.stop();
        onRestoreDefault();
        m_pBtnRegister->hide();
        m_pBtnUnRegister->hide();
        m_pBtnCheck->hide();
        m_pBtnUpdate->show();
        emit registrationBombExpired();
    }
}

void QRserverWidget::onRestoreDefault()
{
    lockTab();
    QRserverModel *rserverModel = qobject_cast<QRserverModel *>(model());
    rserverModel->initialDeviceStatus();
}

void QRserverWidget::onUpateDeviceInfoSucceed()
{
    m_pBtnRegister->show();
    m_pBtnUnRegister->show();
    m_pBtnCheck->show();
    m_pBtnUpdate->hide();
    m_registrationBombTimer.start();
}

void QRserverWidget::onCheckDeviceSucceed()
{
    lockTab(false);

    if (m_pDownloadWidget != nullptr
        && QAdminSettingProfile::Instance()->isLockDown()) {
        m_ptab->setCurrentIndex(RserverTabDownload);
    }
}

void QRserverWidget::lockTab(bool lock)
{
    for (int tabIdx = 1; tabIdx < m_ptab->count(); tabIdx++) {
        m_ptab->setTabEnabled(tabIdx, !lock);
    }

    emit lockUploadProfileTab((
                                  QAdminSettingProfile::Instance()->isBlockProfileUpload()) || lock);

    if (lock) {
        m_uploadTabCurrentIdx = m_pUploadWidget->currentTabIndex();
    }

    if ((m_vconnectModel != nullptr) && (m_vconnectModel->isValid())
        && (m_vconnectModel->rserverMode() == QVConnectModel::ModeVConnect)) {
        m_ptab->setTabEnabled(m_ptab->indexOf(m_pUploadWidget), true);
    }

    if (lock) {
        m_ptab->setCurrentIndex(0);
    }
}

void QRserverWidget::customEvent(QEvent *event)
{
    if (QAutoCheckEvent::EventType() == event->type()) {
        onLoadVConncetServerConfig();

        if (m_vconnectModel->isVConnectValid()) {
            return setEnabled(true);
        } else {
            return onAutoCheck();
        }
    }

    return QRserverContainerWidget::customEvent(event);
}

void QRserverWidget::onAutoCheck()
{
    if (!isVisible()) {
        qprofileDebug(QtWarningMsg) << "QRserverWidget is not visible";
        return;
    }

    if (QAdminSettingProfile::Instance()->isRegistrationBombExpired()) {
        onCheckRegistrationBomb();
    } else {
        lockTab(true);
        const QRegisterModel *registerModel =
            QModelFactory::constModel<QRegisterModel>(this);

        if (!registerModel->serverUrl().isEmpty()) {
            QTimer::singleShot(0, model(), SLOT(initialDeviceStatus()));
            QTimer::singleShot(0, m_pRegister, SLOT(onCheck()));
        }
    }

    setEnabled(true);
}

void QRserverWidget::onUnregister()
{
    if (QMessageBox::Ok != QRserverMessageBox::Information(this,
            TRANSLATE_STR_RSERVER_INFORMATION,
            TRANSLATE_STR_RSERVER_REGISTER_UNREGISTER_CONFIRM,
            QMessageBox::Ok | QMessageBox::Cancel)) {
        return ;
    }

    //registration lock.
    QAdminSettingProfileProperties profileProperties;
    profileProperties.fromVariant(m_registerModel->adminSettingProfile());

    if (profileProperties.lockRegistration()) {
        QRserverMessageBox::Information(this, TRANSLATE_STR_RSERVER_INFORMATION,
                                        TRANSLATE_STR_RSERVER_REGISTER_REGISTRATION_LOCK, QMessageBox::Ok);
        return ;
    }

    onRestoreDefault();
    QTimer::singleShot(0, m_pRegister, SLOT(onUnregister()));
}

void QRserverWidget::allocModel()
{
    setModel(QModelFactory::construct<QRserverModel>(this));
    connect(m_pBtnRegister, SIGNAL(clicked(bool)), this,
            SLOT(onRegisterButtonClicked()));
    connect(this, SIGNAL(registerRserver()), m_pRegister, SLOT(onRegister()));
    connect(this, SIGNAL(updateTechName()), m_pRegister,
            SLOT(onUpdateTechName()));
    connect(m_pBtnCheck, SIGNAL(clicked(bool)), model(),
            SLOT(initialDeviceStatus()));
    connect(m_pBtnCheck, SIGNAL(clicked(bool)), this, SLOT(lockTab()));
    connect(m_pBtnCheck, SIGNAL(clicked(bool)), m_pRegister, SLOT(onCheck()));
    connect(model(), SIGNAL(modelChangedByOthers()), this,
            SLOT(onRserverModelChanged()));
    //
    m_vconnectModel = QModelFactory::construct<QVConnectModel>(this);
    connect(m_vconnectModel, SIGNAL(rserverModeChanged(qint32)), m_pRegister,
            SLOT(onRServerModeChanged(qint32)));
    connect(m_vconnectModel, SIGNAL(propertyChanged()), this,
            SLOT(onVConnectModelChanged()));
    m_registerModel = QModelFactory::constModel<QRegisterModel>(this);
    connect(m_registerModel, SIGNAL(propertyChanged()), this,
            SLOT(onRegisterModelChanged()));
    //
    m_loadVConncetServerConfigTimer.setInterval(1000);
    m_loadVConncetServerConfigTimer.setSingleShot(true);
    connect(&m_loadVConncetServerConfigTimer, SIGNAL(timeout()), this,
            SLOT(onLoadVConncetServerConfig()));
}

void QRserverWidget::onVConnectModelChanged()
{
    QGenericCommand genericCommand(this, QVConnectModel::CommandUpdateVConnectInfo,
                                   "", QVariant(m_vconnectModel->toVariant()));
    QVariant statusVar = issueCommand(genericCommand.toVariant(),
                                      QVConnectModel::UpdateVConnectInfoTimeout * 2, "");

    if (!statusVar.isNull()) {
        QGenericStatus status(statusVar);

        switch (status.commandStatus()) {
        case QAbstractStatus::Succeed:
            qprofileDebug(QtWarningMsg) <<
                                        "update V-Connect info to engine successfully...";
            break;

        default:
            break;
        }
    } else {
        QTimer::singleShot(3000, this, SLOT(onVConnectModelChanged()));
    }
}

void QRserverWidget::onRServerModeChanged()
{
    lockTab(true);
    QRserverModel *rserverModel = qobject_cast<QRserverModel *>(model());
    rserverModel->initialDeviceStatus();

    if (m_pBtnRServerMode->text() == TRANSLATE_STR_RSERVER_BTN_MODE_VCONNECT) {
        /* ModeVConnect */
        m_vconnectModel->setRserverMode(QVConnectModel::ModeVConnect);
        m_pBtnRServerMode->setText(TRANSLATE_STR_RSERVER_BTN_MODE_RSERVER);
        m_ptab->setTabEnabled(m_ptab->indexOf(m_pUploadWidget), true);
    } else {
        /* ModeRServer */
        m_vconnectModel->setRserverMode(QVConnectModel::ModeRServer);
        m_pBtnRServerMode->setText(TRANSLATE_STR_RSERVER_BTN_MODE_VCONNECT);
        m_ptab->setTabEnabled(m_ptab->indexOf(m_pUploadWidget),
                              m_ptab->isTabEnabled(m_ptab->indexOf(m_pSoftwareWidget)));
        //when bluetooth disconnected, lockTab was called first but ModeVConnect has not been changed.
    }
}

void QRserverWidget::onRegisterModelChanged()
{
    bool lastStatus = false;
    QVConnectModel::RServerMode mode = m_vconnectModel->rserverMode();

    if (mode == QVConnectModel::ModeRServer) {
        lastStatus = m_vconnectModel->blockNotify(true);
    }

    m_vconnectModel->fromVariant(m_registerModel->toVariant());

    if (mode == QVConnectModel::ModeRServer) {
        m_vconnectModel->blockNotify(lastStatus);
    }
}

void QRserverWidget::onDirectoryChanged(const QString &path)
{
    QFileInfo vconncetServerConfig(QRserverEnvironment::VConncetServerConfig());

    if (vconncetServerConfig.absolutePath() == path) {
        qprofileDebug(QtWarningMsg) << "directory Changed.";
        m_loadVConncetServerConfigTimer.start();
    }
}

void QRserverWidget::onLoadVConncetServerConfig()
{
    QBaseProgress *backProgress = getBackProgress();

    if ((nullptr != backProgress && backProgress->isActive())
        || m_vconnectModel->serialNumber().isEmpty()) {
        m_loadVConncetServerConfigTimer.start();
        return;
    }

    qprofileDebug(QtWarningMsg) << "load V-Conncet Server Config";
    static bool isVConnectValid = false;
    QFileInfo vconncetServerConfig(QRserverEnvironment::VConncetServerConfig());

    if (vconncetServerConfig.exists()) {
        m_vconnectModel->fromFile(vconncetServerConfig.absoluteFilePath());
    } else {
        m_vconnectModel->setVconnectServerIp(QRegisterModel::InvalidIpAddress);
    }

    if (isVConnectValid != m_vconnectModel->isVConnectValid()) {
        // TODO: hide for now
        //        m_pBtnRServerMode->setVisible(m_vconnectModel->isVConnectValid());
        isVConnectValid = m_vconnectModel->isVConnectValid();

        if (isVConnectValid) {
            m_pBtnRServerMode->setText(TRANSLATE_STR_RSERVER_BTN_MODE_VCONNECT);
        } else {
            m_pBtnRServerMode->setText(TRANSLATE_STR_RSERVER_BTN_MODE_RSERVER);
        }

        QTimer::singleShot(0, this, SLOT(onRServerModeChanged()));
    }
}

void QRserverWidget::onUploadWidgetTabChanged(int idx)
{
    m_pBtnJob->setVisible(idx == QUploadWidget::RserverTabUploadResult);
    m_pBtnUploadAndDel->setVisible(idx == QUploadWidget::RserverTabUploadResult);
    m_uploadTabCurrentIdx = idx;
}

void QRserverWidget::onTabChange(int idx)
{
    m_pBtnRegister->hide();
    m_pBtnUnRegister->hide();
    m_pBtnCheck->hide();
    m_pBtnRServerMode->hide();
    m_pBtnUpload->hide();
    m_pBtnJob->hide();
    m_pBtnUploadAndDel->hide();
    m_pBtnDownload->hide();
    m_pBtnUpgrade->hide();
    m_pBtnUpdate->hide();
    QRserverContainerWidget *tabWidget = dynamic_cast<QRserverContainerWidget *>
                                         (m_ptab->widget(idx));

    if (tabWidget == m_pRegister) {
        m_pBtnRegister->show();
        m_pBtnUnRegister->show();
        m_pBtnCheck->show();
        // TODO: hide for now
        //m_pBtnRServerMode->setVisible(m_vconnectModel->isVConnectValid());
    } else if (tabWidget == m_pUploadWidget) {
        m_pBtnUpload->show();
        const QRserverModel *rserverModel = QModelFactory::constModel<QRserverModel>
                                            (this);

        if (rserverModel->getChassis().isValid() && !rserverModel->supportProfile()) {
            m_pBtnJob->setVisible(true);
            m_pBtnUploadAndDel->setVisible(true);
        } else {
            m_pBtnJob->setVisible(m_uploadTabCurrentIdx ==
                                  QUploadWidget::RserverTabUploadResult);
            m_pBtnUploadAndDel->setVisible(m_uploadTabCurrentIdx ==
                                           QUploadWidget::RserverTabUploadResult);
        }
    } else if (tabWidget == m_pDownloadWidget) {
        m_pBtnDownload->show();
    } else if (tabWidget == m_pSoftwareWidget) {
        m_pBtnUpgrade->show();
    }
}

void QRserverWidget::onIpcDisconnected()
{
    lockTab(true);
    QRserverMessageBox::Show(this, TRANSLATE_STR_RSERVER_WARNING,
                             TRANSLATE_STR_RSERVER_REGISTER_NETWORK_ERROR, QMessageBox::Ok);
    QRserverContainerWidget::onIpcDisconnected();
}

void QRserverWidget::onNetworkError()
{
    lockTab(true);
    QRserverContainerWidget::onNetworkError();
    QRserverModel *rserverModel = qobject_cast<QRserverModel *>(model());
    rserverModel->initialDeviceStatus();
}

void QRserverWidget::translate(const QFont &font)
{
    m_ptab->setTabText(m_ptab->indexOf(m_pRegister),
                       TRANSLATE_STR_RSERVER_TAB_REGISTER);
    m_ptab->setTabText(m_ptab->indexOf(m_pUploadWidget),
                       TRANSLATE_STR_RSERVER_TAB_UPLOAD);
    m_ptab->setTabText(m_ptab->indexOf(m_pSoftwareWidget),
                       TRANSLATE_STR_RSERVER_TAB_SOFTWARE);
    m_pBtnRegister->setText(TRANSLATE_STR_RSERVER_BTN_REGISTER);
    m_pBtnUnRegister->setText(TRANSLATE_STR_RSERVER_BTN_UNREGISTER);
    m_pBtnCheck->setText(TRANSLATE_STR_RSERVER_BTN_CHECK);

    if (m_vconnectModel->rserverMode() == QVConnectModel::ModeRServer) {
        m_pBtnRServerMode->setText(TRANSLATE_STR_RSERVER_BTN_MODE_VCONNECT);
    } else {
        m_pBtnRServerMode->setText(TRANSLATE_STR_RSERVER_BTN_MODE_RSERVER);
    }

    m_pBtnUpload->setText(TRANSLATE_STR_RSERVER_BTN_UPLOAD);
    m_pBtnJob->setText(TRANSLATE_STR_RSERVER_BTN_JOB);
    m_pBtnUploadAndDel->setText(TRANSLATE_STR_RSERVER_BTN_UPLOAD_DEL);
    m_pBtnDownload->setText(TRANSLATE_STR_RSERVER_BTN_DOWNLOAD);
    m_pBtnUpgrade->setText(TRANSLATE_STR_RSERVER_BTN_UPGRADE);
    m_pBtnUpdate->setText(TRANSLATE_STR_RSERVER_BTN_UPDATE);
    m_pRegister->translate(font);
    m_pUploadWidget->translate(font);
    m_pSoftwareWidget->translate(font);

    if (m_pDownloadWidget != nullptr) {
        m_ptab->setTabText(m_ptab->indexOf(m_pDownloadWidget),
                           TRANSLATE_STR_RSERVER_TAB_DOWNLOAD);
        m_pDownloadWidget->translate(font);
    }

    //
    m_ptab->setFont(font);
    m_pBtnRegister->setFont(font);
    m_pBtnUnRegister->setFont(font);
    m_pBtnCheck->setFont(font);
    m_pBtnUpload->setFont(font);
    m_pBtnJob->setFont(font);
    m_pBtnUploadAndDel->setFont(font);
    m_pBtnDownload->setFont(font);
    m_pBtnUpgrade->setFont(font);
    m_pBtnUpdate->setFont(font);
}

void QRserverWidget::onRserverModelChanged()
{
    QRserverModel *rserverModel = qobject_cast<QRserverModel *>(model());

    if (rserverModel->getChassis().isValid()) {
        if (!rserverModel->supportProfile()) {
            m_ptab->removeTab(m_ptab->indexOf(m_pDownloadWidget));
            delete m_pDownloadWidget;
            m_pDownloadWidget = nullptr;
        }

        m_ptab->setStyleSheet(QString("QTabBar::tab{width:%1px;}").arg(
                                  m_ptab->width() / m_ptab->count()));
        QDeviceInfo::Style style = rserverModel->getStyle();
        static QDeviceInfo::Style lastStyle = QDeviceInfo::DefaultStyle;

        if ((QDeviceInfo::FX150LStyle == style) && (lastStyle != style)) {
            m_pBtLayout->insertSpacing(0, 50);
        }

        lastStyle = style;
        disconnect(rserverModel, SIGNAL(propertyChanged()), this,
                   SLOT(onRserverModelChanged()));
        //
        bool lastStatus = m_vconnectModel->blockNotify(true);
        m_vconnectModel->fromVariant(rserverModel->getChassis());
        m_vconnectModel->blockNotify(lastStatus);
        //
        QFileInfo vconncetServerConfig(QRserverEnvironment::VConncetServerConfig());

        if (vconncetServerConfig.exists()
            && !m_loadVConncetServerConfigTimer.isActive()) {
            m_loadVConncetServerConfigTimer.start();
        }
    }
}

bool QRserverWidget::isTopWidget()
{
    return true;
}

void QRserverWidget::onRegisterButtonClicked()
{
    if (isTechNameCanBeChanged()) {
        emit updateTechName();
    } else {
        lockTab();
        QRserverModel *rserverModel = qobject_cast<QRserverModel *>(model());
        rserverModel->initialDeviceStatus();
        emit registerRserver();
    }
}

bool QRserverWidget::isTechNameCanBeChanged()
{
    QRserverModel *rserverModel = qobject_cast<QRserverModel *>(model());
    return (isCompatible(QRserverCompatibility518::FeatureUpdateTechName) &&
            QAdminSettingProfile::Instance()->isTechNameCanBeChanged() &&
            (QDeviceInfo::RegisteredAuthorized ==
             QRserverModel::GetDeviceStatus(*rserverModel))
           );
}

QRserverWidget::~QRserverWidget()
{
}

