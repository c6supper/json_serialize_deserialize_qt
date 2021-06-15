
#include "qvconnectmodel.h"
#include "qobjectfactory.h"
#include "qmodelfactory.h"

using namespace QProfile;
using namespace QRserver;

QObjectSelfRegisteration<QVConnectModel> registerQVConnectModel;

const QString QVConnectModel::DefaultVConnectServerConfig =
    "vconnectserver.json";
const QString QVConnectModel::DefaultVConnectClientConfig =
    "vconnectclient.json";
const QString QVConnectModel::CommandUpdateVConnectInfo = "UpdateVConnectInfo";
const quint32 QVConnectModel::UpdateVConnectInfoTimeout = 5 * 1000;

QVConnectModel::QVConnectModel(QObject *parent, const QByteArray &content)
    : QAbstractModel(parent, content)
{
    m_registerModel = QModelFactory::constModel<QRegisterModel>(this);
}

QVConnectModel::QVConnectModel(const QVConnectModel &model)
    : QAbstractModel(model)
{
    m_registerModel = QModelFactory::constModel<QRegisterModel>(this);
}

QVConnectModel::QVConnectModel(QObject *parent) : QAbstractModel(parent)
{
    Q_ASSERT(parent != nullptr);
    m_registerModel = QModelFactory::constModel<QRegisterModel>(this);
}

void QVConnectModel::initAttributes()
{
    m_rserverMode = QSharedPointer<QNumericAttribute>(new QNumericAttribute((
                        qint32)QVConnectModel::ModeRServer, QVariant::UInt, this));
    m_vconnectServerIp = QSharedPointer<QIpAttribute>(new QIpAttribute(
                             QRegisterModel::InvalidIpAddress, this));
    m_serverUrl = QSharedPointer<QStringAttribute>(new QStringAttribute(this));
    m_techId = QSharedPointer<QStringAttribute>(new QStringAttribute(this));
    m_techName = QSharedPointer<QStringAttribute>(new QStringAttribute(this));
    m_company = QSharedPointer<QStringAttribute>(new QStringAttribute(this));
    m_supervisorId = QSharedPointer<QStringAttribute>(new QStringAttribute(this));
    m_deviceType = QSharedPointer<QNumericAttribute>(new QNumericAttribute(0,
                   QVariant::UInt, this));
    m_serialNumber = QSharedPointer<QStringAttribute>(new QStringAttribute(this));
    m_mac = QSharedPointer<QMacAttribute>(new QMacAttribute(this));
    m_cableMac = QSharedPointer<QMacAttribute>(new QMacAttribute(this));
    m_version = QSharedPointer<QStringAttribute>(new QStringAttribute(this));
    m_deviceName = QSharedPointer<QStringAttribute>(new QStringAttribute(this));
    m_isValid = true;
}

void QVConnectModel::initial()
{
}

bool QVConnectModel::isVConnectValid()
{
    return (QRegisterModel::InvalidIpAddress != vconnectServerIp());
}

QVConnectModel::RServerMode QVConnectModel::rserverMode() const
{
    return (QVConnectModel::RServerMode)m_rserverMode->value().toInt();
}
void QVConnectModel::setRserverMode(QVConnectModel::RServerMode rserverMode)
{
    if (this->rserverMode() == rserverMode) {
        return;
    }

    m_rserverMode->setValue((qint32)rserverMode);
    emit rserverModeChanged((qint32)this->rserverMode());
}

QString QVConnectModel::vconnectServerIp() const
{
    return m_vconnectServerIp->value().toString();
}
void QVConnectModel::setVconnectServerIp(QString vconnectServerIp)
{
    if (this->vconnectServerIp() == vconnectServerIp) {
        return;
    }

    if (!m_vconnectServerIp->setValue(vconnectServerIp)) {
        m_vconnectServerIp->setValue(QRegisterModel::InvalidIpAddress);
    }
}

QString QVConnectModel::serverUrl() const
{
    return m_serverUrl->value().toString();
}
void QVConnectModel::setServerUrl(QString serverUrl)
{
    if (this->serverUrl() == serverUrl) {
        return;
    }

    m_serverUrl->setValue(serverUrl);
}

QString QVConnectModel::techId() const
{
    return m_techId->value().toString();
}
void QVConnectModel::setTechId(QString techId)
{
    if (this->techId() == techId) {
        return;
    }

    m_techId->setValue(techId);
}

QString QVConnectModel::techName() const
{
    return m_techName->value().toString();
}
void QVConnectModel::setTechName(QString techName)
{
    if (this->techName() == techName) {
        return;
    }

    m_techName->setValue(techName);
}

QString QVConnectModel::company() const
{
    return m_company->value().toString();
}
void QVConnectModel::setCompany(QString company)
{
    if (this->company() == company) {
        return;
    }

    m_company->setValue(company);
}

QString QVConnectModel::supervisorId() const
{
    return m_supervisorId->value().toString();
}
void QVConnectModel::setSupervisorId(QString supervisorId)
{
    if (this->supervisorId() == supervisorId) {
        return;
    }

    m_supervisorId->setValue(supervisorId);
}

quint32 QVConnectModel::deviceType() const
{
    return m_deviceType->value().toUInt();
}
void QVConnectModel::setDeviceType(quint32 deviceType)
{
    m_deviceType->setValue(deviceType);
}

QString QVConnectModel::serialNumber() const
{
    return m_serialNumber->value().toString();
}
void QVConnectModel::setSerialNumber(QString serialNumber)
{
    m_serialNumber->setValue(serialNumber);
}

QString QVConnectModel::mac() const
{
    return m_mac->value().toString();
}
void QVConnectModel::setMac(QString mac)
{
    m_mac->setValue(mac);
}

QString QVConnectModel::cableMac() const
{
    return m_cableMac->value().toString();
}
void QVConnectModel::setCableMac(QString cableMac)
{
    m_cableMac->setValue(cableMac);
}

QString QVConnectModel::version() const
{
    return m_version->value().toString();
}
void QVConnectModel::setVersion(QString version)
{
    m_version->setValue(version);
}

QString QVConnectModel::deviceName() const
{
    return m_deviceName->value().toString();
}
void QVConnectModel::setDeviceName(QString deviceName)
{
    m_deviceName->setValue(deviceName);
}
