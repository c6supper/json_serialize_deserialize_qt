#include "qmessagefactory.h"
#include "qmsgcheckall.h"

using namespace QRserver;
using namespace QProfile;
using namespace rserver;

QMessageSelfRegisteration<QMsgCheckAll> registerQMsgCheckAll(
    rserver::eCheckAll);

QDataStream &QMsgCheckAll::read(QDataStream &in)
{
    st_TestsetAllCheckInfo stMessage;

    if ((qint32)sizeof(stMessage) <= in.readRawData((char *)&stMessage,
            sizeof(stMessage))) {
        m_registerStatus =
            stMessage.RegisterStatus;
        m_serverName->setValue(QVariant(stMessage.ServerInfo.szServerName));
        m_serverLocation->setValue(QVariant(stMessage.ServerInfo.szServerLocation));
#if defined(Q_OS_LINUX)
        struct tm  tmDateTime = {0};
        tmDateTime.tm_sec  = stMessage.ServerInfo.nArrDate[5];
        tmDateTime.tm_min  = stMessage.ServerInfo.nArrDate[4];
        tmDateTime.tm_hour = stMessage.ServerInfo.nArrDate[3];
        tmDateTime.tm_mday = stMessage.ServerInfo.nArrDate[2];
        tmDateTime.tm_mon  = stMessage.ServerInfo.nArrDate[1] - 1;
        tmDateTime.tm_year = stMessage.ServerInfo.nArrDate[0] - 1900;
        tmDateTime.tm_wday  = 0;
        tmDateTime.tm_yday  = 0;
        tmDateTime.tm_isdst = 0;
        time_t time_new = timelocal((struct tm *)&tmDateTime);
        m_serverDateTime->setValue(QVariant(QDateTime::fromTime_t(time_new)));
#else
        m_serverDateTime->setValue(QVariant(QDateTime::currentDateTime()));
#endif
        m_deviceType->setValue(QVariant::fromValue<quint32>
                               (stMessage.DeviceInfo.cDeviceType));
        m_serialNumber->setValue(QVariant(stMessage.DeviceInfo.szSerialNum));
        m_mac->fromChar((char *)stMessage.DeviceInfo.Mac,
                        sizeof(stMessage.DeviceInfo.Mac));
        m_cableMac->fromChar((char *)stMessage.DeviceInfo.CMMac,
                             sizeof(stMessage.DeviceInfo.CMMac));
        m_version->setValue(QVariant(stMessage.DeviceInfo.szVersion));
        m_techId->setValue(QVariant(stMessage.DeviceInfo.szTechID));
        m_techName->setValue(QVariant(stMessage.DeviceInfo.TechnicianName));
        m_company->setValue(QVariant(stMessage.DeviceInfo.szCompany));
        m_supervisorId->setValue(QVariant(stMessage.DeviceInfo.szSupervisorID));
        //
        m_adminStatus->setValue(QVariant::fromValue<quint32>
                                (stMessage.AdminMode.bEnable));
        m_lockDown->setValue(QVariant::fromValue<quint32>
                             (stMessage.AdminMode.bLockDownEnable));
        m_timeLock->setValue(QVariant::fromValue<quint32>
                             (stMessage.AdminMode.TimeLockEnable));
        m_timeBomb->setValue(QVariant::fromValue<quint32>
                             (stMessage.AdminMode.TimeBombEnable));
        m_timeBombMessage->setValue(QVariant(stMessage.AdminMode.strMessage));
        m_timeBombDays->setValue(QVariant::fromValue<quint32>
                                 (stMessage.AdminMode.nDaysCount));
        //
        m_lockRegistration->setValue(QVariant::fromValue<quint32>
                                     (stMessage.LockRegistration));
        //
        m_remoteVersion->setValue(QVariant(stMessage.SWVersion.CurrentVersion));
        m_orgChartStatus->setValue(QVariant::fromValue<quint32>
                                   (stMessage.OrgChartStatus.status));
        m_region->setValue(QVariant(stMessage.OrgChartInfo.szRegion));
        m_district->setValue(QVariant(stMessage.OrgChartInfo.szDistrict));
        m_system->setValue(QVariant(stMessage.OrgChartInfo.szSystem));
        qprofileDebug(QtDebugMsg) << toJson();
    }

    return in;
}

const QByteArray QMsgCheckAll::toByteArray() const
{
    st_RserverDeviceInfoExt device;
    bzero(&device, sizeof(st_RserverDeviceInfoExt));
    QByteArray messageArray((const char *)&device, sizeof(device));
    QDataStream out(&messageArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_6);
    out << *this;
    return messageArray;
}

QDataStream &QMsgCheckAll::write(QDataStream &out) const
{
    st_RserverDeviceInfoExt stMessage;
    bzero(&stMessage, sizeof(stMessage));
    stMessage.cDeviceType = (quint8)m_deviceType->value().toUInt();
    m_serialNumber->toChar(stMessage.szSerialNum, sizeof(stMessage.szSerialNum));
    m_mac->toChar((char *)stMessage.Mac);
    m_cableMac->toChar((char *)stMessage.CMMac);
    m_version->toChar(stMessage.szVersion, sizeof(stMessage.szVersion));
    m_techId->toChar(stMessage.szTechID, sizeof(stMessage.szTechID));
    m_techName->toChar(stMessage.TechnicianName, sizeof(stMessage.TechnicianName));
    m_company->toChar(stMessage.szCompany, sizeof(stMessage.szCompany));
    m_supervisorId->toChar(stMessage.szSupervisorID,
                           sizeof(stMessage.szSupervisorID));
    out.writeRawData((const char *)&stMessage, sizeof(stMessage));
    qprofileDebug(QtDebugMsg) << toJson();
    return out;
}

QMsgCheckAll::QMsgCheckAll(QObject *parent)
    : QAbstractBinMsg(rserver::eCheckAll, parent)
{
    initAttributes();
}

QMsgCheckAll::QMsgCheckAll(const QMsgCheckAll &message)
    : QAbstractBinMsg(message)
{
    initAttributes();
}

void QMsgCheckAll::initAttributes()
{
    //
    m_serverName = QSharedPointer<QStringAttribute>(new QStringAttribute(this));
    m_serverLocation = QSharedPointer<QStringAttribute>(new QStringAttribute(this));
    m_serverDateTime = QSharedPointer<QDateTimeAttribute>(new QDateTimeAttribute(
                           this));
    //
    m_deviceType = QSharedPointer<QNumericAttribute>(new QNumericAttribute(0,
                   QVariant::Int, this));
    m_mac = QSharedPointer<QMacAttribute>(new QMacAttribute(this));
    m_cableMac = QSharedPointer<QMacAttribute>(new QMacAttribute(this));
    m_version = QSharedPointer<QStringAttribute>(new QStringAttribute(this));
    m_techId = QSharedPointer<QStringAttribute>(new QStringAttribute(this));
    m_techName = QSharedPointer<QStringAttribute>(new QStringAttribute(this));
    m_company = QSharedPointer<QStringAttribute>(new QStringAttribute(this));
    m_supervisorId = QSharedPointer<QStringAttribute>(new QStringAttribute(this));
    //
    m_adminStatus = QSharedPointer<QNumericAttribute>(new QNumericAttribute(0,
                    QVariant::Int, this));
    m_lockDown = QSharedPointer<QNumericAttribute>(new QNumericAttribute(0,
                 QVariant::Int, this));
    m_timeLock = QSharedPointer<QNumericAttribute>(new QNumericAttribute(0,
                 QVariant::Int, this));
    m_timeBomb = QSharedPointer<QNumericAttribute>(new QNumericAttribute(0,
                 QVariant::Int, this));
    m_timeBombMessage = QSharedPointer<QStringAttribute>(new QStringAttribute(
                            this));
    m_timeBombDays = QSharedPointer<QNumericAttribute>(new QNumericAttribute(0,
                     QVariant::Int, this));
    //
    m_lockRegistration = QSharedPointer<QNumericAttribute>(new QNumericAttribute(0,
                         QVariant::Int, this));
    //
    m_remoteVersion = QSharedPointer<QStringAttribute>(new QStringAttribute(
                          this));
    //
    m_orgChartStatus = QSharedPointer<QNumericAttribute>(new QNumericAttribute(
                           this));
    //
    m_region = QSharedPointer<QStringAttribute>(new QStringAttribute(this));
    m_district = QSharedPointer<QStringAttribute>(new QStringAttribute(this));
    m_system = QSharedPointer<QStringAttribute>(new QStringAttribute(this));
}

QMsgCheckAll::~QMsgCheckAll()
{
}

qint8 QMsgCheckAll::status() const
{
    return m_registerStatus;
}
void QMsgCheckAll::setStatus(qint8 status)
{
    m_registerStatus = status;
}

QString QMsgCheckAll::serverName() const
{
    return m_serverName->value().toString();
}
void QMsgCheckAll::setServerName(QString serverName)
{
    m_serverName->setValue(serverName);
}

QString QMsgCheckAll::serverLocation() const
{
    return m_serverLocation->value().toString();
}
void QMsgCheckAll::setServerLocation(QString serverLocation)
{
    m_serverLocation->setValue(serverLocation);
}

QDateTime QMsgCheckAll::serverDateTime() const
{
    return m_serverDateTime->value().toDateTime();
}
void QMsgCheckAll::setServerDateTime(QDateTime dateTime)
{
    m_serverDateTime->setValue(dateTime);
}

quint32 QMsgCheckAll::deviceType() const
{
    return m_deviceType->value().toUInt();
}
void QMsgCheckAll::setDeviceType(quint32 deviceType)
{
    m_deviceType->setValue(deviceType);
}

QString QMsgCheckAll::mac() const
{
    return m_mac->value().toString();
}
void QMsgCheckAll::setMac(QString mac)
{
    m_mac->setValue(mac);
}

QString QMsgCheckAll::cableMac() const
{
    return m_cableMac->value().toString();
}
void QMsgCheckAll::setCableMac(QString cableMac)
{
    m_cableMac->setValue(cableMac);
}

QString QMsgCheckAll::version() const
{
    return m_version->value().toString();
}
void QMsgCheckAll::setVersion(QString version)
{
    m_version->setValue(version);
}

QString QMsgCheckAll::techId() const
{
    return m_techId->value().toString();
}
void QMsgCheckAll::setTechId(QString techId)
{
    m_techId->setValue(techId);
}

QString QMsgCheckAll::techName() const
{
    return m_techName->value().toString();
}
void QMsgCheckAll::setTechName(QString techName)
{
    m_techName->setValue(techName);
}

QString QMsgCheckAll::company() const
{
    return m_company->value().toString();
}
void QMsgCheckAll::setCompany(QString company)
{
    m_company->setValue(company);
}

QString QMsgCheckAll::supervisorId() const
{
    return m_supervisorId->value().toString();
}
void QMsgCheckAll::setSupervisorId(QString supervisorId)
{
    m_supervisorId->setValue(supervisorId);
}

quint32 QMsgCheckAll::adminStatus() const
{
    return m_adminStatus->value().toUInt();
}
void QMsgCheckAll::setAdminStatus(quint32 adminStatus)
{
    m_adminStatus->setValue(adminStatus);
}

quint32 QMsgCheckAll::lockDown() const
{
    return m_lockDown->value().toUInt();
}
void QMsgCheckAll::setLockDown(quint32 lockDown)
{
    m_lockDown->setValue(lockDown);
}

quint32 QMsgCheckAll::timeLock() const
{
    return m_timeLock->value().toUInt();
}
void QMsgCheckAll::setTimeLock(quint32 timeLock)
{
    m_timeLock->setValue(timeLock);
}

quint32 QMsgCheckAll::timeBomb() const
{
    return m_timeBomb->value().toUInt();
}
void QMsgCheckAll::setTimeBomb(quint32 timeBomb)
{
    m_timeBomb->setValue(timeBomb);
}

QString QMsgCheckAll::timeBombMessage() const
{
    return m_timeBombMessage->value().toString();
}
void QMsgCheckAll::setTimeBombMessage(QString timeBombMessage)
{
    m_timeBombMessage->setValue(timeBombMessage);
}

quint32 QMsgCheckAll::timeBombDays() const
{
    return m_timeBombDays->value().toUInt();
}
void QMsgCheckAll::setTimeBombDays(quint32 timeBombDays)
{
    m_timeBombDays->setValue(timeBombDays);
}

QDateTime QMsgCheckAll::expiredDateTime() const
{
    QDateTime currentDT = m_serverDateTime->value().toDateTime();
    return currentDT.addDays(m_timeBombDays->value().toUInt());
}

quint32 QMsgCheckAll::lockRegistration() const
{
    return m_lockRegistration->value().toUInt();
}
void QMsgCheckAll::setLockRegistration(quint32 lockRegistration)
{
    m_lockRegistration->setValue(lockRegistration);
}

QString QMsgCheckAll::remoteVersion() const
{
    return m_remoteVersion->value().toString();
}
void QMsgCheckAll::setRemoteVersion(QString remoteVersion)
{
    m_remoteVersion->setValue(remoteVersion);
}

quint32 QMsgCheckAll::orgChartStatus() const
{
    return m_orgChartStatus->value().toUInt();
}
void QMsgCheckAll::setOrgChartStatus(quint32 status)
{
    m_orgChartStatus->setValue(status);
}

QString QMsgCheckAll::region() const
{
    return m_region->value().toString();
}
void QMsgCheckAll::setRegion(QString region)
{
    m_region->setValue(region);
}

QString QMsgCheckAll::district() const
{
    return m_district->value().toString();
}
void QMsgCheckAll::setDistrict(QString district)
{
    m_district->setValue(district);
}

QString QMsgCheckAll::system() const
{
    return m_system->value().toString();
}
void QMsgCheckAll::setSystem(QString system)
{
    m_system->setValue(system);
}

void QMsgCheckAll::getMac(char *mac) const
{
    m_mac->toChar(mac);
}
void QMsgCheckAll::setMacFromChar(char *mac, int length)
{
    m_mac->fromChar(mac, length);
}

void QMsgCheckAll::getCableMac(char *mac) const
{
    m_cableMac->toChar(mac);
}
void QMsgCheckAll::setCableMacFromChar(char *mac, int length)
{
    m_cableMac->fromChar(mac, length);
}


