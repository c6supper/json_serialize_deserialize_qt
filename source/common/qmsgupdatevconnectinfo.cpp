#include "qmessagefactory.h"
#include "qmsgupdatevconnectinfo.h"

using namespace QRserver;
using namespace QProfile;
using namespace rserver;

QMessageSelfRegisteration<QMsgUpdateVConnectInfo>
registerQMsgUpdateVConnectInfo(
    rserver::eUpdateVConnectInfo);

QDataStream &QMsgUpdateVConnectInfo::read(QDataStream &in)
{
    return in;
}

const QByteArray QMsgUpdateVConnectInfo::toByteArray() const
{
    return toJson();
}

QDataStream &QMsgUpdateVConnectInfo::write(QDataStream &out) const
{
    const QByteArray info = toJson();
    out.writeRawData(info.constData(), info.size());
    return out;
}

QMsgUpdateVConnectInfo::QMsgUpdateVConnectInfo(QObject *parent)
    : QAbstractBinMsg(rserver::eUpdateVConnectInfo, parent)
{
    initAttributes();
}

QMsgUpdateVConnectInfo::QMsgUpdateVConnectInfo(const QMsgUpdateVConnectInfo
        &message)
    : QAbstractBinMsg(message)
{
    initAttributes();
}

void QMsgUpdateVConnectInfo::initAttributes()
{
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
}

QString QMsgUpdateVConnectInfo::serverUrl() const
{
    return m_serverUrl->value().toString();
}
void QMsgUpdateVConnectInfo::setServerUrl(QString serverUrl)
{
    m_serverUrl->setValue(serverUrl);
}

QString QMsgUpdateVConnectInfo::techId() const
{
    return m_techId->value().toString();
}
void QMsgUpdateVConnectInfo::setTechId(QString techId)
{
    m_techId->setValue(techId);
}

QString QMsgUpdateVConnectInfo::techName() const
{
    return m_techName->value().toString();
}
void QMsgUpdateVConnectInfo::setTechName(QString techName)
{
    m_techName->setValue(techName);
}

QString QMsgUpdateVConnectInfo::company() const
{
    return m_company->value().toString();
}
void QMsgUpdateVConnectInfo::setCompany(QString company)
{
    m_company->setValue(company);
}

QString QMsgUpdateVConnectInfo::supervisorId() const
{
    return m_supervisorId->value().toString();
}
void QMsgUpdateVConnectInfo::setSupervisorId(QString supervisorId)
{
    m_supervisorId->setValue(supervisorId);
}

quint32 QMsgUpdateVConnectInfo::deviceType() const
{
    return m_deviceType->value().toUInt();
}
void QMsgUpdateVConnectInfo::setDeviceType(quint32 deviceType)
{
    m_deviceType->setValue(deviceType);
}

QString QMsgUpdateVConnectInfo::serialNumber() const
{
    return m_serialNumber->value().toString();
}
void QMsgUpdateVConnectInfo::setSerialNumber(QString serialNumber)
{
    m_serialNumber->setValue(serialNumber);
}

QString QMsgUpdateVConnectInfo::mac() const
{
    return m_mac->value().toString();
}
void QMsgUpdateVConnectInfo::setMac(QString mac)
{
    m_mac->setValue(mac);
}

QString QMsgUpdateVConnectInfo::cableMac() const
{
    return m_cableMac->value().toString();
}
void QMsgUpdateVConnectInfo::setCableMac(QString cableMac)
{
    m_cableMac->setValue(cableMac);
}

QString QMsgUpdateVConnectInfo::version() const
{
    return m_version->value().toString();
}
void QMsgUpdateVConnectInfo::setVersion(QString version)
{
    m_version->setValue(version);
}

QString QMsgUpdateVConnectInfo::deviceName() const
{
    return m_deviceName->value().toString();
}
void QMsgUpdateVConnectInfo::setDeviceName(QString deviceName)
{
    m_deviceName->setValue(deviceName);
}

QMsgUpdateVConnectInfo::~QMsgUpdateVConnectInfo()
{
}

