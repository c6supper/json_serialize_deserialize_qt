/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */


#include "qregistermodel.h"
#include "qobjectfactory.h"
#include "qmodelfactory.h"

using namespace QProfile;
using namespace QRserver;

QObjectSelfRegisteration<QRegisterModel> registerQRegisterModel;

const QString QRegisterModel::CommandRegister = "register";
const QString QRegisterModel::CommandUnregister = "unregister";
const QString QRegisterModel::CommandCheck = "check";
const QString QRegisterModel::CommandUpdate = "update";
const QString QRegisterModel::CommandUpdateTechName = "updateTechName";

const quint32 QRegisterModel::RegisterSingleDeviceTimeout = 20 * 1000;
const quint32 QRegisterModel::UnregisterSingleDeviceTimeout = 20 * 1000;
const quint32 QRegisterModel::CheckSingleDeviceTimeout = 20 * 1000;
const quint32 QRegisterModel::UpdateDeviceInfoTimeout = 20 * 1000;
const quint32 QRegisterModel::GetServerVersionTimeout = 20 * 1000;
const quint32 QRegisterModel::UpdateTechNameTimeout = 20 * 1000;


const QString QRegisterModel::ApiVersion = "1.0";
const QString QRegisterModel::InvalidIpAddress = "0.0.0.0";
const QString QRegisterModel::AdminSettingProfileProperty =
    "adminSettingProfile";

QRegisterModel::QRegisterModel(QObject *parent, const QByteArray &content)
    : QAbstractModel(parent, content)
{
    m_rserverModel = QSharedPointer<const QRserverModel>
                     (QModelFactory::constModel<QRserverModel>(this));
}

QRegisterModel::QRegisterModel(const QRegisterModel &model)
    : QAbstractModel(model)
{
    m_rserverModel = QSharedPointer<const QRserverModel>
                     (QModelFactory::constModel<QRserverModel>(this));
}

QRegisterModel::QRegisterModel(QObject *parent) : QAbstractModel(parent)
{
    Q_ASSERT(parent != nullptr);
    m_rserverModel = QSharedPointer<const QRserverModel>
                     (QModelFactory::constModel<QRserverModel>(this));
}

void QRegisterModel::initAttributes()
{
    m_techId = QSharedPointer<QStringAttribute>(new QStringAttribute(this));
    m_techName = QSharedPointer<QStringAttribute>(new QStringAttribute(this));
    m_company = QSharedPointer<QStringAttribute>(new QStringAttribute(this));
    m_serverUrl = QSharedPointer<QStringAttribute>(new QStringAttribute(this));
    m_serverIp = QSharedPointer<QIpAttribute>(new QIpAttribute(this));
    m_networkDevice =  QSharedPointer<QStringAttribute>(new QStringAttribute(this));
    m_serverPort = QSharedPointer<QNumericAttribute>(new QNumericAttribute(QVariant(
                       33001), QVariant::UInt, this));
    m_supervisorId = QSharedPointer<QStringAttribute>(new QStringAttribute(this));
    m_region = QSharedPointer<QStringAttribute>(new QStringAttribute(this));
    m_district = QSharedPointer<QStringAttribute>(new QStringAttribute(this));
    m_system = QSharedPointer<QStringAttribute>(new QStringAttribute(this));
    m_serverName = QSharedPointer<QStringAttribute>(new QStringAttribute(this));
    m_orgChartStatus = QSharedPointer<QNumericAttribute>(new QNumericAttribute((
                           quint32)QRegisterModel::Disable, QVariant::UInt, this));
}

void QRegisterModel::initial()
{
    m_region->setValue("");
    m_district->setValue("");
    m_system->setValue("");
    m_serverName->setValue("");
    m_orgChartStatus->setValue(QRegisterModel::Disable);
    asynPropertyChangeNotify();
}

QString QRegisterModel::serverUrl() const
{
    return m_serverUrl->value().toString();
}
void QRegisterModel::setServerUrl(QString serverUrl)
{
    m_serverUrl->setValue(serverUrl);
}

QString QRegisterModel::serverIp() const
{
    return m_serverIp->value().toString();
}
void QRegisterModel::setServerIp(QString serverIp)
{
    if (m_serverIp->value().toString() != serverIp) {
        m_serverIp->setValue(serverIp);
        emit serverIpChanged();
    }
}

QString QRegisterModel::networkDevice() const
{
    return m_networkDevice->value().toString();
}
void QRegisterModel::setNetworkDevice(QString networkDevice)
{
    m_networkDevice->setValue(networkDevice);
}

QString QRegisterModel::techId() const
{
    return m_techId->value().toString();
}
void QRegisterModel::setTechId(QString techId)
{
    m_techId->setValue(techId);
}

QString QRegisterModel::techName() const
{
    return m_techName->value().toString();
}
void QRegisterModel::setTechName(QString techName)
{
    m_techName->setValue(techName);
}

QString QRegisterModel::company() const
{
    return m_company->value().toString();
}
void QRegisterModel::setCompany(QString company)
{
    m_company->setValue(company);
}

QString QRegisterModel::supervisorId() const
{
    return m_supervisorId->value().toString();
}
void QRegisterModel::setSupervisorId(QString supervisorId)
{
    m_supervisorId->setValue(supervisorId);
}

quint16 QRegisterModel::serverPort() const
{
    return (quint16)(m_serverPort->value().toUInt());
}
void QRegisterModel::setServerPort(quint16 port)
{
    m_serverPort->setValue(port);
}

QString QRegisterModel::region() const
{
    return m_region->value().toString();
}

void QRegisterModel::setRegion(QString region)
{
    m_region->setValue(region);
}

QString QRegisterModel::district() const
{
    return m_district->value().toString();
}

void QRegisterModel::setDistrict(QString district)
{
    m_district->setValue(district);
}

QString QRegisterModel::system() const
{
    return m_system->value().toString();
}

void QRegisterModel::setSystem(QString system)
{
    m_system->setValue(system);
}

QString QRegisterModel::serverName() const
{
    return m_serverName->value().toString();
}
void QRegisterModel::setServerName(QString serverName)
{
    m_serverName->setValue(serverName);
}

quint32 QRegisterModel::orgChartStatus() const
{
    return (quint32)(m_orgChartStatus->value().toUInt());
}
void QRegisterModel::setOrgChartStatus(quint32 status)
{
    m_orgChartStatus->setValue(status);
}

QString QRegisterModel::apiVersion() const
{
    return m_apiVersion;
}
void QRegisterModel::setApiVersion(QString apiVersion)
{
    m_apiVersion = apiVersion;
}

const QVariant QRegisterModel::adminSettingProfile() const
{
    return m_adminSettingProfileProperties.toVariant();
}

void QRegisterModel::setAdminSettingProfile(const QVariant
        adminSettingProfileVar)
{
    m_adminSettingProfileProperties.fromVariant(adminSettingProfileVar);
    /*    QDateTime serverDateTime = m_adminSettingProfileProperties.serverDateTime();

        if (m_adminSettingProfileProperties.isRegistrationBomb() &&
            !m_adminSettingProfileProperties.registrationExpiredDateTime().isValid() &&
            serverDateTime.isValid()) {
            m_adminSettingProfileProperties.setRegistrationExpiredDateTime(
                serverDateTime.addDays(QAbstractAdminSettingProfile::RegistrationLockDays));
        }

        if (!m_adminSettingProfileProperties.isRegistrationBomb()) {
            m_adminSettingProfileProperties.setRegistrationExpiredDateTime(QDateTime());
        }*/
}

