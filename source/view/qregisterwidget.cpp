#include "qregisterwidget.h"
#include <QVBoxLayout>
#include "qrserverstrtable.h"
#include "qmodelfactory.h"
#include "qgenericcommand.h"
#include "qrservermessagebox.h"
#include <QTcpSocket>
#include "qregisterstatus.h"
#include "qabstractenvironment.h"
#include "qrserverenvironment.h"
#include "qadminsettingprofile.h"
#include <QFileInfo>
#include "qcompatibilityfactory.h"
#include "qrservercompatibility500.h"
#include "qrservercompatibility518.h"
#include "quploadfiletablewidget.h"
#include "qtechinfo.h"

#if defined(BUILD_X86)
#include "qjson/qobjecthelper.h"
#else
#include <QJson/QObjectHelper>
#endif

using namespace QJson;
using namespace QRserver;
using namespace QProfile;

const int QRegisterWidget::m_techInfoLength = 30;
const int QRegisterWidget::m_supervisorIdLength = 30;
const int QRegisterWidget::m_companyLength = 63;

QRegisterWidget::QRegisterWidget(QWidget *parent, const QString &tagName)
    : QRserverContainerWidget(parent, false)
{
    setTagName(tagName);
    setFixedSize(parent->width(), parent->height() - TAB_ROW_HEIGHT);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);
    m_serverAddress = new QParmLineEdit(TRANSLATE_STR_RSERVER_REGISTER_SERVER_ADDR,
                                        tr(""), this);
    connect(m_serverAddress, SIGNAL(textChanged(const QString &)),
            SLOT(hostChanged(const QString &)));
    m_techID = new QParmLineEdit(TRANSLATE_STR_RSERVER_REGISTER_TECH_ID, tr(""),
                                 this);
    m_techID->setMaxLength(m_techInfoLength);
    m_techName = new QParmLineEdit(TRANSLATE_STR_RSERVER_REGISTER_TECH_NAME, tr(""),
                                   this);
    m_techName->setMaxLength(m_techInfoLength);
    m_company = new QParmLineEdit(TRANSLATE_STR_RSERVER_REGISTER_COMPANY, tr(""),
                                  this);
    m_company->setMaxLength(m_companyLength);
    m_supervisorId = new QParmLineEdit(TRANSLATE_STR_RSERVER_REGISTER_SUPERVISOR_ID,
                                       tr(""), this);
    m_supervisorId->setMaxLength(m_supervisorIdLength);
    m_info = new QLabel(this);
    m_info->setObjectName("RegisterInfo");
    m_vconnectInfo = new QLabel(this);
    m_clientInfo = new QLabel(this);
    m_clientInfo->setObjectName("ClientInfo");
    m_region = new QParmLineEdit(TRANSLATE_STR_RSERVER_REGISTER_REGION, tr(""),
                                 this);
    m_district = new QParmLineEdit(TRANSLATE_STR_RSERVER_REGISTER_DISTRICT, tr(""),
                                   this);
    m_system = new QParmLineEdit(TRANSLATE_STR_RSERVER_REGISTER_SYSTEM, tr(""),
                                 this);
    connect(m_techID, SIGNAL(textChanged(const QString &)), this,
            SLOT(onRegisterParamChanged(const QString &)));
    connect(m_techName, SIGNAL(textChanged(const QString &)), this,
            SLOT(onRegisterParamChanged(const QString &)));
    connect(m_company, SIGNAL(textChanged(const QString &)), this,
            SLOT(onRegisterParamChanged(const QString &)));
    connect(m_supervisorId, SIGNAL(textChanged(const QString &)), this,
            SLOT(onRegisterParamChanged(const QString &)));
    layout->addWidget(m_serverAddress);
    layout->addWidget(m_techID);
    layout->addWidget(m_techName);
    layout->addWidget(m_company);
    layout->addWidget(m_supervisorId);
    layout->addSpacing(10);
    layout->addWidget(m_vconnectInfo);
    layout->addSpacing(15);
    layout->addWidget(m_info);
    layout->addSpacing(20);
    layout->addWidget(m_region);
    layout->addWidget(m_district);
    layout->addWidget(m_system);
    layout->addStretch();
    layout->addWidget(m_clientInfo);
    layout->addSpacing(5);
    m_lookupHostTimer.setInterval(500);
    m_lookupHostTimer.setSingleShot(true);
    connect(&m_lookupHostTimer, SIGNAL(timeout()), this, SLOT(onLookupHost()));
    m_updateTechInfoTimer.setInterval(15000);
    connect(&m_updateTechInfoTimer, SIGNAL(timeout()), this,
            SLOT(onCheckTechInfo()));
    setRegionItemsVisible(false);
    setInformation("");
    m_clientInfo->setText(TRANSLATE_STR_RSERVER_REGISTER_CLIENTVERSION.arg(
                              QRserverEnvironment::Version()));
}

void QRegisterWidget::onRegisterParamChanged(const QString &text)
{
    QParmLineEdit *pSender = qobject_cast<QParmLineEdit *>(sender());

    if (pSender == m_techID) {
        m_registerModel->setTechId(text);
    } else if (pSender == m_techName) {
        m_registerModel->setTechName(text);
    } else if (pSender == m_company) {
        m_registerModel->setCompany(text);
    } else if (pSender == m_supervisorId) {
        m_registerModel->setSupervisorId(text);
    }
}

void QRegisterWidget::onIpcConnected()
{
    setInformation("");
    QAbstractContainerWidget::onIpcConnected();
}

void QRegisterWidget::onIpcDisconnected()
{
    setInformation(TRANSLATE_STR_RSERVER_REGISTER_NETWORK_ERROR);
    QRserverContainerWidget::onIpcDisconnected();
}

void QRegisterWidget::onNetworkError()
{
    setInformation("");
    setRegisterApproved(false);
    QRserverContainerWidget::onNetworkError();
    showNetworkErrorMessageBox();
}

void QRegisterWidget::showNetworkErrorMessageBox()
{
    QRserverMessageBox::Show(this, TRANSLATE_STR_RSERVER_WARNING,
                             TRANSLATE_STR_RSERVER_REGISTER_NETWORK_ERROR, QMessageBox::Ok);
}

bool QRegisterWidget::loadProfile(const QString filePathName)
{
    bool ret = false;

    if (QFile::exists(filePathName)) {
        ret = m_registerModel->fromFile(filePathName);
    }

    if (ret) {
        qprofileDebug(QtDebugMsg) << m_registerModel->toJson();
        QObjectHelper::qvariant2qobject(m_registerModel->toVariant().toMap(), this);

        if (!m_registerModel->serverUrl().isEmpty()) {
            setServerAddress(m_registerModel->serverUrl());
        }
    }

    return ret;
}

bool QRegisterWidget::loadLastProfile()
{
    bool ret = false;
    QFileInfo lastProfileInfo(QAbstractEnvironment::LastProfile());
    QFile lastProfile(QAbstractEnvironment::LastProfile());
    QFile defaultProfile(QAbstractEnvironment::DefaultProfile());

    if (!defaultProfile.exists()) {
        m_registerModel->toFile(QAbstractEnvironment::DefaultProfile(),
                                QStringList() << QRegisterModel::AdminSettingProfileProperty);
    }

    if (!lastProfileInfo.isSymLink()) {
        lastProfile.remove();
    }

    if (!lastProfileInfo.exists()) {
        defaultProfile.link(QAbstractEnvironment::LastProfile());
    }

    ret = loadProfile(QAbstractEnvironment::LastProfile());
    return ret;
}

void QRegisterWidget::onSubscribeFinished()
{
    if (!loadLastProfile()) {
        qprofileDebug(QtWarningMsg) << "Failed to load last profile!";
    }
}

void QRegisterWidget::saveProfile()
{
    m_registerModel->toFile(QFile::symLinkTarget(
                                QAbstractEnvironment::LastProfile()),
                            QStringList() << QRegisterModel::AdminSettingProfileProperty);
}

void QRegisterWidget::postponeInit()
{
    QRserverContainerWidget::postponeInit();

    if (!m_topic.isNull()) {
        connect(m_topic.data(), SIGNAL(subscribeFinished()), this,
                SLOT(onSubscribeFinished()));
    }
}

QString QRegisterWidget::serverAddress() const
{
    return m_serverAddress->text();
}

void QRegisterWidget::setServerAddress(const QString &serverAddress)
{
    m_serverAddress->setText(serverAddress);
}

QString QRegisterWidget::techId() const
{
    return m_techID->text();
}

void QRegisterWidget::setTechId(const QString &techId)
{
    m_techID->setText(techId);
}

QString QRegisterWidget::techName() const
{
    return m_techName->text();
}

void QRegisterWidget::setTechName(const QString &techName)
{
    m_techName->setText(techName);
}

QString QRegisterWidget::company() const
{
    return m_company->text();
}

void QRegisterWidget::setCompany(const QString &company)
{
    m_company->setText(company);
}

QString QRegisterWidget::supervisorId() const
{
    return m_supervisorId->text();
}

void QRegisterWidget::setSupervisorId(const QString &supervisorId)
{
    m_supervisorId->setText(supervisorId);
}

QString QRegisterWidget::region() const
{
    return m_region->text();
}

void QRegisterWidget::setRegion(const QString &region)
{
    m_region->setText(region);
}

QString QRegisterWidget::district() const
{
    return m_district->text();
}

void QRegisterWidget::setDistrict(const QString &district)
{
    m_district->setText(district);
}

QString QRegisterWidget::system() const
{
    return m_system->text();
}

void QRegisterWidget::setSystem(const QString &system)
{
    m_system->setText(system);
}

void QRegisterWidget::translate(const QFont &font)
{
    m_serverAddress->setName(TRANSLATE_STR_RSERVER_REGISTER_SERVER_ADDR);
    m_techID->setName(TRANSLATE_STR_RSERVER_REGISTER_TECH_ID);
    m_techName->setName(TRANSLATE_STR_RSERVER_REGISTER_TECH_NAME);
    m_company->setName(TRANSLATE_STR_RSERVER_REGISTER_COMPANY);
    m_supervisorId->setName(TRANSLATE_STR_RSERVER_REGISTER_SUPERVISOR_ID);
    m_region->setName(TRANSLATE_STR_RSERVER_REGISTER_REGION);
    m_district->setName(TRANSLATE_STR_RSERVER_REGISTER_DISTRICT);
    m_system->setName(TRANSLATE_STR_RSERVER_REGISTER_SYSTEM);
    //
    m_serverAddress->setFont(font);
    m_techID->setFont(font);
    m_techName->setFont(font);
    m_company->setFont(font);
    m_supervisorId->setFont(font);
    m_info->setFont(font);
    m_region->setFont(font);
    m_district->setFont(font);
    m_system->setFont(font);
    m_vconnectInfo->setFont(font);
    m_clientInfo->setFont(font);
}

void QRegisterWidget::setRegionItemsVisible(bool bVisible)
{
    if (bVisible) {
        if (m_registerModel->orgChartStatus() == QRegisterModel::Enable
            && (!m_registerModel->region().isEmpty()
                || !m_registerModel->district().isEmpty()
                || !m_registerModel->system().isEmpty())) {
            m_region->setText(m_registerModel->region());
            m_district->setText(m_registerModel->district());
            m_system->setText(m_registerModel->system());
            m_region->show();
            m_district->show();
            m_system->show();
        }
    } else {
        m_region->setText("");
        m_region->hide();
        m_district->setText("");
        m_district->hide();
        m_system->setText("");
        m_system->hide();
    }
}

void QRegisterWidget::setInformation(const QString &information)
{
    m_info->setText(information);
}

void QRegisterWidget::setValid(bool bValid)
{
    m_techID->setEnable((bValid == false) ? bValid :
                        (!QAdminSettingProfile::Instance()->isLockTechId()));
    m_techName->setEnable(isTechNameCanBeChanged() || bValid);
    m_company->setEnable(bValid);
    m_supervisorId->setEnable(bValid);
    m_region->setEnable(bValid);
    m_district->setEnable(bValid);
    m_system->setEnable(bValid);
}

void QRegisterWidget::setRegisterApproved(bool bApproved)
{
    if (bApproved) {
        setRegionItemsVisible(true);
        setValid(false);
    } else {
        setRegionItemsVisible(false);
        setValid(true);
    }
}

void QRegisterWidget::allocModel()
{
    setModel(m_registerModel = QModelFactory::construct<QRegisterModel>(this),
             false);
    m_registerModel->setAdminSettingProfile(QAdminSettingProfile::Instance(
                                            )->toVariant());
}

void QRegisterWidget::onRServerModeChanged(qint32 rserverMode)
{
    setInformation("");

    if (QVConnectModel::ModeVConnect == (QVConnectModel::RServerMode)rserverMode) {
        m_vconnectInfo->setText(TRANSLATE_STR_RSERVER_REGISTER_VCONNECT);
    } else {
        m_vconnectInfo->setText("");
    }

    setRegisterApproved(false);
}

bool QRegisterWidget::checkConnective()
{
    if (m_registerModel->serverIp() == QRegisterModel::InvalidIpAddress) {
        showNetworkErrorMessageBox();
        return false;
    }

    return true;
}

void QRegisterWidget::getCompatibility(const QString className)
{
    qint32 metaType = QMetaType::type(className.toStdString().c_str());

    if (metaType == QMetaType::Void) {
        qprofileDebug(QtDebugMsg) << "Invalid compatibility type =  " << metaType;
        return;
    }

    qint32 lastMetaType = QAbstractCompatibility::GetCompatibilityMetaType();

    if (lastMetaType == metaType) {
        qprofileDebug(QtDebugMsg) << "New compatibility is the same = " << metaType;
        return;
    }

    void *compatibility = QMetaType::construct(metaType);

    if (compatibility == nullptr) {
        qprofileDebug(QtDebugMsg) << "Failed to create compatibility = " << metaType;
        return;
    }

    QAbstractCompatibility::SetInstance((QAbstractCompatibility *)compatibility);
}

void QRegisterWidget::onUpdateTechName()
{
    if (!checkConnective()) {
        return;
    }

    QGenericCommand genericCommand(this, QRegisterModel::CommandUpdateTechName, "",
                                   QVariant(toVariant()));
    QVariant statusVar = issueCommand(genericCommand.toVariant(),
                                      QRegisterModel::UpdateTechNameTimeout *
                                      QAbstractCommand::RetryCountLimitation * 2,
                                      "");

    if (!statusVar.isNull()) {
        QRegisterStatus status(statusVar);

        switch (status.commandStatus()) {
        case QAbstractStatus::Succeed: {
            QRserverMessageBox::Information(this, TRANSLATE_STR_RSERVER_WARNING,
                                            TRANSLATE_STR_RSERVER_REGISTER_UPDATE_TECHNAME_SUCCESS, QMessageBox::Ok);
            saveProfile();
            break;
        }

        default:
            QRserverMessageBox::Information(this, TRANSLATE_STR_RSERVER_WARNING,
                                            TRANSLATE_STR_RSERVER_REGISTER_UPDATE_TECHNAME_FAIL.arg(status.commandStatus()),
                                            QMessageBox::Ok);
            break;
        }
    }
}

void QRegisterWidget::onRegister()
{
    setInformation("");
    setRegisterApproved(false);

    if (!checkConnective()) {
        return;
    }

    QGenericCommand genericCommand(this, QRegisterModel::CommandRegister, "",
                                   QVariant(toVariant()));
    int deviceCount = m_registerModel->rserverModel().deviceList().count();
    QVariant statusVar = issueCommand(genericCommand.toVariant(),
                                      QRegisterModel::RegisterSingleDeviceTimeout * deviceCount *
                                      QAbstractCommand::RetryCountLimitation * 2,
                                      "");

    if (!statusVar.isNull()) {
        QRegisterStatus status(statusVar);
        quint32 commandStatus = (quint32)status.commandStatus();

        switch (commandStatus) {
        case QAbstractStatus::Succeed: {
            QRserverModel rserverModel;
            rserverModel.fromVariant(status.rserverModelVar());

            if (QRserverModel::GetDeviceStatus(rserverModel) ==
                QDeviceInfo::RegisterSucceed) {
                setInformation(TRANSLATE_STR_RSERVER_REGISTER_REGISTER_AND_WAIT);
                QRserverMessageBox::Information(this, TRANSLATE_STR_RSERVER_INFORMATION,
                                                TRANSLATE_STR_RSERVER_REGISTER_REGISTER_AND_WAIT, QMessageBox::Ok);
            } else {
                setInformation(TRANSLATE_STR_RSERVER_REGISTER_REJECTED);
                QRserverMessageBox::Information(this, TRANSLATE_STR_RSERVER_WARNING,
                                                TRANSLATE_STR_RSERVER_REGISTER_REGISTER_FAIL, QMessageBox::Ok);
            }

            break;
        }

        case QRegisterStatus::TestsetBeRevoked: {
            QRserverMessageBox::Information(this, TRANSLATE_STR_RSERVER_WARNING,
                                            TRANSLATE_STR_RSERVER_REGISTER_UNREGISTER_REVOKED, QMessageBox::Ok);
            break;
        }

        case QRegisterStatus::TestsetBeStolen: {
            QRserverMessageBox::Information(this, TRANSLATE_STR_RSERVER_WARNING,
                                            TRANSLATE_STR_RSERVER_REGISTER_UNREGISTER_STOLEN, QMessageBox::Ok);
            break;
        }

        default:
            QRserverMessageBox::Information(this, TRANSLATE_STR_RSERVER_WARNING,
                                            TRANSLATE_STR_RSERVER_REGISTER_REGISTER_FAIL, QMessageBox::Ok);
            break;
        }
    }
}

void QRegisterWidget::onUnregister()
{
    setInformation("");
    setRegisterApproved(false);

    if (!checkConnective()) {
        return;
    }

    QGenericCommand genericCommand(this, QRegisterModel::CommandUnregister, "",
                                   QVariant(toVariant()));
    int deviceCount = m_registerModel->rserverModel().deviceList().count();
    QVariant statusVar = issueCommand(genericCommand.toVariant(),
                                      QRegisterModel::UnregisterSingleDeviceTimeout * deviceCount *
                                      QAbstractCommand::RetryCountLimitation * 2,
                                      "");

    if (!statusVar.isNull()) {
        QRegisterStatus status(statusVar);
        quint32 commandStatus = (quint32)status.commandStatus();

        switch (commandStatus) {
        case QAbstractStatus::Succeed: {
            QRserverModel rserverModel;
            rserverModel.fromVariant(status.rserverModelVar());

            if (QRserverModel::GetDeviceStatus(rserverModel) ==
                QDeviceInfo::UnregisterSucceed) {
                //QAdminSettingProfile::Instance()->setStatus(QDeviceInfo::Unregistered);  the status should be gotten form checking
                setInformation(TRANSLATE_STR_RSERVER_REGISTER_UNREGISTER_APPROVED);
                QRserverMessageBox::Information(this, TRANSLATE_STR_RSERVER_INFORMATION,
                                                TRANSLATE_STR_RSERVER_REGISTER_UNREGISTER_SUCCESS, QMessageBox::Ok);
            } else {
                setInformation(TRANSLATE_STR_RSERVER_REGISTER_REJECTED);
                QRserverMessageBox::Information(this, TRANSLATE_STR_RSERVER_WARNING,
                                                TRANSLATE_STR_RSERVER_REGISTER_UNREGISTER_FAIL, QMessageBox::Ok);
            }

            break;
        }

        case QRegisterStatus::TestsetBeRevoked: {
            QRserverMessageBox::Information(this, TRANSLATE_STR_RSERVER_WARNING,
                                            TRANSLATE_STR_RSERVER_REGISTER_UNREGISTER_REVOKED, QMessageBox::Ok);
            break;
        }

        case QRegisterStatus::TestsetBeStolen: {
            QRserverMessageBox::Information(this, TRANSLATE_STR_RSERVER_WARNING,
                                            TRANSLATE_STR_RSERVER_REGISTER_UNREGISTER_STOLEN, QMessageBox::Ok);
            break;
        }

        default:
            QRserverMessageBox::Information(this, TRANSLATE_STR_RSERVER_WARNING,
                                            TRANSLATE_STR_RSERVER_REGISTER_UNREGISTER_FAIL, QMessageBox::Ok);
            break;
        }
    }
}

void QRegisterWidget::onCheck()
{
    m_registerModel->initial();
    setInformation("");
    setRegisterApproved(false);

    if (!checkConnective()) {
        return;
    }

    QGenericCommand genericCommand(this, QRegisterModel::CommandCheck, "",
                                   QVariant(toVariant()));
    int deviceCount = m_registerModel->rserverModel().deviceList().count();
    QVariant statusVar = issueCommand(genericCommand.toVariant(),
                                      QRegisterModel::CheckSingleDeviceTimeout * deviceCount *
                                      QAbstractCommand::RetryCountLimitation * 2,
                                      "");

    if (!statusVar.isNull()) {
        QRegisterStatus status(statusVar);

        switch (status.commandStatus()) {
        case QAbstractStatus::Succeed: {
            QRserverModel rserverModel;
            rserverModel.fromVariant(status.rserverModelVar());
            m_registerModel->fromVariant(status.registerModelVar());
            qprofileDebug(QtDebugMsg) << status.toJson();
            getCompatibility(status.compatibilityClassName());

            if (isCompatible(QRserverCompatibility405::FeatureLongFileName)) {
                QUploadFileTableWidget::FileNameLengthLimitation =
                    QRserverCompatibility405::MaxLongFileName;
            } else {
                QUploadFileTableWidget::FileNameLengthLimitation =
                    QRserverCompatibility405::MaxFileName;
            }

            QDeviceInfo::DeviceStatus deviceStatus = QRserverModel::GetDeviceStatus(
                        rserverModel);
            QDeviceInfo::TestsetStatus testsetStatus = QRserverModel::GetTestsetStatus(
                        rserverModel);
            qprofileDebug(QtDebugMsg) << "testsetStatus = " << testsetStatus;
            QAdminSettingProfile::Instance()->fromVariant(
                m_registerModel->adminSettingProfile());

            if (deviceStatus == QDeviceInfo::Unregistered) {
                QString strInfo;

                switch (testsetStatus) {
                case QDeviceInfo::Missing:
                    strInfo = TRANSLATE_STR_RSERVER_REGISTER_TESTSET_STATUS_MISSING;
                    break;

                case QDeviceInfo::Broken:
                    strInfo = TRANSLATE_STR_RSERVER_REGISTER_TESTSET_STATUS_Broken;
                    break;

                case QDeviceInfo::Calibration:
                    strInfo = TRANSLATE_STR_RSERVER_REGISTER_TESTSET_STATUS_Calibration;
                    break;

                case QDeviceInfo::Sold:
                    strInfo = TRANSLATE_STR_RSERVER_REGISTER_TESTSET_STATUS_SOLD;
                    break;

                case QDeviceInfo::Retired:
                    strInfo = TRANSLATE_STR_RSERVER_REGISTER_TESTSET_STATUS_RETIRED;
                    break;

                default:
                    strInfo = TRANSLATE_STR_RSERVER_REGISTER_NOT_REG;
                    break;
                }

                setInformation(strInfo);
                QRserverMessageBox::Information(this, TRANSLATE_STR_RSERVER_WARNING,
                                                TRANSLATE_STR_RSERVER_REGISTER_NOT_REG, QMessageBox::Ok);
            } else if (deviceStatus == QDeviceInfo::RegisteredUnauthorized) {
                setInformation(TRANSLATE_STR_RSERVER_REGISTER_REGISTER_NOT_AUTH);
                QRserverMessageBox::Information(this, TRANSLATE_STR_RSERVER_WARNING,
                                                TRANSLATE_STR_RSERVER_REGISTER_REGISTER_NOT_AUTH, QMessageBox::Ok);
            } else if (deviceStatus == QDeviceInfo::RegisteredAuthorized) {
                setTechId(m_registerModel->techId());
                setTechName(m_registerModel->techName());
                setCompany(m_registerModel->company());
                setSupervisorId(m_registerModel->supervisorId());
                setRegisterApproved(true);
                QString strInfo;

                if (testsetStatus == QDeviceInfo::Loaned) {
                    strInfo = QString(TRANSLATE_STR_RSERVER_REGISTER_REGISTER_AND_AUTH).arg(
                                  TRANSLATE_STR_RSERVER_REGISTER_TESTSET_STATUS_LOANED) +
                              m_registerModel->serverName();
                } else {
                    strInfo = QString(TRANSLATE_STR_RSERVER_REGISTER_REGISTER_AND_AUTH).arg(
                                  TRANSLATE_STR_RSERVER_REGISTER_TESTSET_STATUS_APPROVED) +
                              m_registerModel->serverName();
                }

                setInformation(strInfo);
                saveProfile();

                if (m_registerModel->rserverModel().supportProfile()
                    && !(QAdminSettingProfile::Instance()->isLockDown())
                    && isCompatible(QRserverCompatibility518::FeatureUploadProfileInfoList)) {
                    emit getProfileInfoList();
                }

                emit checkDeviceSucceed();

                if (!m_registerModel->rserverModel().supportProfile()
                    || !QAdminSettingProfile::Instance()->isLockDown()) {
                    QRserverMessageBox::Information(this, TRANSLATE_STR_RSERVER_INFORMATION,
                                                    TRANSLATE_STR_RSERVER_REGISTER_REGISTER_AUTHORIZED, QMessageBox::Ok);
                }
            } else {
                QRserverMessageBox::Information(this, TRANSLATE_STR_RSERVER_WARNING,
                                                TRANSLATE_STR_RSERVER_REGISTER_CHECK_FAIL, QMessageBox::Ok);
            }

            break;
        }

        default:
            QRserverMessageBox::Information(this, TRANSLATE_STR_RSERVER_WARNING,
                                            TRANSLATE_STR_RSERVER_REGISTER_CHECK_FAIL, QMessageBox::Ok);
            break;
        }
    }
}

void QRegisterWidget::onRegistrationBombExpired()
{
    onRestoreDefault();
    setInformation(TRANSLATE_STR_RSERVER_REGISTER_REGISTRATION_BOMB_EXPIRED);

    if (!QFile::exists(QTechInfo::TechInfoFilePath)) {
        return ;
    }

    //load tech info from file.
    QTechInfo techInfo;
    techInfo.fromFile(QTechInfo::TechInfoFilePath);
    m_techID->setText(techInfo.techId());
    m_techName->setText(techInfo.techName());
    m_company->setText(techInfo.company());
    m_supervisorId->setText(techInfo.supervisorId());

    if (!isVisible()) {
        //RServer is hidden. auto update tech info.
        onUpdate();
    }
}

void QRegisterWidget::onRestoreDefault()
{
    if (!QAdminSettingProfile::Instance()->isLockTechId()) {
        setTechId("");
    }

    setTechName("");
    setCompany("");
    setSupervisorId("");
    setInformation("");
    setRegisterApproved(false);
}

void QRegisterWidget::onUpdate()
{
    //eUpdateDeviceInfo
    if ((QAdminSettingProfile::Instance()->isLockTechId()
         && m_techName->text().isEmpty() && m_company->text().isEmpty()
         && m_supervisorId->text().isEmpty())
        || (!QAdminSettingProfile::Instance()->isLockTechId()
            && m_techID->text().isEmpty() && m_techName->text().isEmpty()
            && m_company->text().isEmpty() && m_supervisorId->text().isEmpty())) {
        QRserverMessageBox::Show(this, TRANSLATE_STR_RSERVER_WARNING,
                                 TRANSLATE_STR_RSERVER_REGISTER_TECHINFO_EMPTY, QMessageBox::Ok);
        return;
    }

    //
    QTechInfo localTechInfo;
    localTechInfo.fromFile(QAbstractEnvironment::LastProfile());

    if (m_techID->text() == localTechInfo.techId()
        && m_techName->text() == localTechInfo.techName()
        && m_company->text() == localTechInfo.company()
        &&  m_supervisorId->text() == localTechInfo.supervisorId()) {
        QRserverMessageBox::Show(this, TRANSLATE_STR_RSERVER_WARNING,
                                 TRANSLATE_STR_RSERVER_REGISTER_CHECK_TECHINFO, QMessageBox::Ok);
        return;
    }

    if (!checkConnective()) {
        return;
    }

    QGenericCommand genericCommand(this, QRegisterModel::CommandUpdate, "",
                                   QVariant(toVariant()));
    QVariant statusVar = issueCommand(genericCommand.toVariant(),
                                      QRegisterModel::UpdateDeviceInfoTimeout *
                                      QAbstractCommand::RetryCountLimitation * 2, "");

    if (!statusVar.isNull()) {
        QRegisterStatus status(statusVar);

        switch (status.commandStatus()) {
        case QAbstractStatus::Succeed: {
            setInformation("");
            QAdminSettingProfile::Instance()->setRegistrationExpiredDateTime(QDateTime());

            if (QFile::exists(QTechInfo::TechInfoFilePath)) {
                QFile::remove(QTechInfo::TechInfoFilePath);
            }

            emit upateDeviceInfoSucceed();
            QRserverMessageBox::Information(this, TRANSLATE_STR_RSERVER_WARNING,
                                            TRANSLATE_STR_RSERVER_REGISTER_UPDATE_DEVICEINFO_SCCESS, QMessageBox::Ok);
            break;
        }

        default:
            QRserverMessageBox::Information(this, TRANSLATE_STR_RSERVER_WARNING,
                                            TRANSLATE_STR_RSERVER_REGISTER_UPDATE_DEVICEINFO_FAILED, QMessageBox::Ok);
            break;
        }
    }
}

QString QRegisterWidget::serverIp() const
{
    return m_registerModel->serverIp();
}

void QRegisterWidget::setServerIp(QString serverIp)
{
    if (m_registerModel->serverIp() == serverIp) {
        return;
    }

    m_registerModel->setServerIp(serverIp);
    getNetworkChecker()->onNetworkParamChanged(serverIp,
            m_registerModel->serverPort());
}

void QRegisterWidget::onCheckTechInfo()
{
}

void QRegisterWidget::onLookupHost()
{
    QHostInfo::lookupHost(m_registerModel->serverUrl(), this,
                          SLOT(lookUpHost(QHostInfo)));
}

#define IP_REGEXP "((([1-9]?\\d|1\\d\\d|2[0-4]\\d|25[0-5])\\.){3}([1-9]?\\d|1\\d\\d|2[0-4]\\d|25[0-5]))"
#define URL_REGEXP "^(http:\\/\\/www\\.|https:\\/\\/www\\.|http:\\/\\/|https:\\/\\/)?[a-z0-9\\-_]+([\\-\\._]{1}[a-z0-9\\-_]+)*\\.[a-z]{2,5}(:[0-9]{1,5})?(\\/.*)?$"

void QRegisterWidget::hostChanged(const QString &hostname)
{
    //clear info.
    emit serverAddressChanged();
    onRestoreDefault();
    //find ip
    QRegExp hostRegExp(IP_REGEXP);
    QRegExp urlRegExp(URL_REGEXP);
    m_registerModel->setServerUrl("");
    setServerIp(QRegisterModel::InvalidIpAddress);

    if (hostname.isEmpty()) {
        QRserverMessageBox::Information(this, TRANSLATE_STR_RSERVER_WARNING,
                                        TRANSLATE_STR_RSERVER_REGISTER_ENTER_SERVER_ADDR, QMessageBox::Ok);
        qprofileDebug(QtWarningMsg) << "Please enter the server address or URL!";
        return;
    }

    if (hostRegExp.exactMatch(hostname)) {
        m_registerModel->setServerUrl(hostname);
        setServerIp(hostname);
    } else {
        if (urlRegExp.exactMatch(hostname)) {
            m_registerModel->setServerUrl(hostname);
            const QVConnectModel *vconnectModel = QModelFactory::constModel<QVConnectModel>
                                                  (this);
            // todo: fix ping pong
            QTimer::singleShot(1000, this, SLOT(onLookupHost()));
        } else {
            QRserverMessageBox::Information(this, TRANSLATE_STR_RSERVER_WARNING,
                                            TRANSLATE_STR_RSERVER_REGISTER_INVALID_SERVER_ADDR, QMessageBox::Ok);
        }
    }
}

void QRegisterWidget::lookUpHost(const QHostInfo &host)
{
    QString strIP = "";
    QRegExp hostRegExp(IP_REGEXP);

    if (host.error() != QHostInfo::NoError) {
        m_lookupHostTimer.start();
        return;
    }

    foreach (const QHostAddress &address, host.addresses()) {
        strIP = address.toString();

        if (!address.isNull()
            && (QAbstractSocket::IPv4Protocol == address.protocol())
            && address != QHostAddress::LocalHost
            && hostRegExp.exactMatch(strIP)) {
            setServerIp(strIP);
            qprofileDebug(QtDebugMsg) << "The IP address of server is : " << strIP;
            return;
        }
    }

    m_lookupHostTimer.start();
}

bool QRegisterWidget::isTechNameCanBeChanged()
{
    return (isCompatible(QRserverCompatibility518::FeatureUpdateTechName) &&
            QAdminSettingProfile::Instance()->isTechNameCanBeChanged() &&
            (QDeviceInfo::RegisteredAuthorized ==
             QRserverModel::GetDeviceStatus(m_registerModel->rserverModel()))
           );
}

QRegisterWidget::~QRegisterWidget()
{
}

