#include "qmessagefactory.h"
#include "qmsgcheckalllongid.h"

using namespace QRserver;
using namespace QProfile;
using namespace rserver;

QMessageSelfRegisteration<QMsgCheckAllLongId> registerQMsgCheckAllLongId(
    rserver::eCheckAll2);

QDataStream &QMsgCheckAllLongId::read(QDataStream &in)
{
    st_TestsetAllCheckInfoLongId stMessage;

    if ((qint32)sizeof(stMessage) <= in.readRawData((char *)&stMessage,
            sizeof(stMessage))) {
        setStatus(stMessage.RegisterStatus);
        setServerName(QString(stMessage.ServerInfo.szServerName));
        setServerLocation(QString(stMessage.ServerInfo.szServerLocation));
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
        setServerDateTime(QDateTime::fromTime_t(time_new));
#else
        setServerDateTime(QDateTime::currentDateTime());
#endif
        setDeviceType(stMessage.DeviceInfo.cDeviceType);
        setSerialNumber(QString(stMessage.DeviceInfo.szSerialNum));
        setMacFromChar((char *)stMessage.DeviceInfo.Mac,
                       sizeof(stMessage.DeviceInfo.Mac));
        setCableMacFromChar((char *)stMessage.DeviceInfo.CMMac,
                            sizeof(stMessage.DeviceInfo.CMMac));
        setVersion(QString(stMessage.DeviceInfo.szVersion));
        setTechId(QString(stMessage.DeviceInfo.szTechID));
        setTechName(QString(stMessage.DeviceInfo.TechnicianName));
        setCompany(QString(stMessage.DeviceInfo.szCompany));
        setSupervisorId(QString(stMessage.DeviceInfo.szSupervisorID));
        //
        setAdminStatus(stMessage.AdminMode.bEnable);
        setLockDown(stMessage.AdminMode.bLockDownEnable);
        setTimeLock(stMessage.AdminMode.TimeLockEnable);
        setTimeBomb(stMessage.AdminMode.TimeBombEnable);
        setTimeBombMessage(QString(stMessage.AdminMode.strMessage));
        setTimeBombDays(stMessage.AdminMode.nDaysCount);
        //
        setLockRegistration(stMessage.LockRegistration);
        //
        setRemoteVersion(QString(stMessage.SWVersion.CurrentVersion));
        setOrgChartStatus(stMessage.OrgChartStatus.status);
        setRegion(QString(stMessage.OrgChartInfo.szRegion));
        setDistrict(QString(stMessage.OrgChartInfo.szDistrict));
        setSystem(QString(stMessage.OrgChartInfo.szSystem));
        qprofileDebug(QtDebugMsg) << toJson();
    }

    return in;
}

const QByteArray QMsgCheckAllLongId::toByteArray() const
{
    st_RserverDeviceInfoExtLongId device;
    bzero(&device, sizeof(st_RserverDeviceInfoExtLongId));
    QByteArray messageArray((const char *)&device, sizeof(device));
    QDataStream out(&messageArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_6);
    out << *this;
    return messageArray;
}

QDataStream &QMsgCheckAllLongId::write(QDataStream &out) const
{
    st_RserverDeviceInfoExtLongId stMessage;
    bzero(&stMessage, sizeof(stMessage));
    stMessage.cDeviceType = (quint8)deviceType();
    QStringAttribute::StringToChar(serialNumber(), stMessage.szSerialNum,
                                   sizeof(stMessage.szSerialNum));
    getMac((char *)stMessage.Mac);
    getCableMac((char *)stMessage.CMMac);
    QStringAttribute::StringToChar(version(), stMessage.szVersion,
                                   sizeof(stMessage.szVersion));
    QStringAttribute::StringToChar(techId(), stMessage.szTechID,
                                   sizeof(stMessage.szTechID));
    QStringAttribute::StringToChar(techName(), stMessage.TechnicianName,
                                   sizeof(stMessage.TechnicianName));
    QStringAttribute::StringToChar(company(), stMessage.szCompany,
                                   sizeof(stMessage.szCompany));
    QStringAttribute::StringToChar(supervisorId(), stMessage.szSupervisorID,
                                   sizeof(stMessage.szSupervisorID));
    out.writeRawData((const char *)&stMessage, sizeof(stMessage));
    qprofileDebug(QtDebugMsg) << toJson();
    return out;
}

QMsgCheckAllLongId::QMsgCheckAllLongId(QObject *parent)
    : QMsgCheckAll(parent)
{
    setType(rserver::eCheckAll2);
    initAttributes();
}

QMsgCheckAllLongId::QMsgCheckAllLongId(const QMsgCheckAllLongId &message)
    : QMsgCheckAll(message)
{
    setType(rserver::eCheckAll2);
    initAttributes();
}

QMsgCheckAllLongId::~QMsgCheckAllLongId()
{
}
