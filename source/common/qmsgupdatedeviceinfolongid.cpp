#include "qmessagefactory.h"
#include "qmsgupdatedeviceinfolongid.h"

using namespace QRserver;
using namespace QProfile;
using namespace rserver;

QMessageSelfRegisteration<QMsgUpdateDeviceInfoLongId>
registerQMsgUpdateDeviceInfoLongId(
    rserver::eUpdateDeviceInfo2);

QMsgUpdateDeviceInfoLongId::QMsgUpdateDeviceInfoLongId(QObject *parent)
    : QMsgUpdateDeviceInfo(parent)
{
    setType(rserver::eUpdateDeviceInfo2);
    initAttributes();
}

QMsgUpdateDeviceInfoLongId::QMsgUpdateDeviceInfoLongId(const
        QMsgUpdateDeviceInfoLongId
        &message)
    : QMsgUpdateDeviceInfo(message)
{
    setType(rserver::eUpdateDeviceInfo2);
    initAttributes();
}

QMsgUpdateDeviceInfoLongId::~QMsgUpdateDeviceInfoLongId()
{
}

QDataStream &QMsgUpdateDeviceInfoLongId::write(QDataStream &out) const
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
    out.writeRawData((const char *)&stMessage,
                     sizeof(stMessage));
    return out;
}

const QByteArray QMsgUpdateDeviceInfoLongId::toByteArray() const
{
    st_RserverDeviceInfoExtLongId device;
    bzero(&device, sizeof(st_RserverDeviceInfoExt));
    QByteArray messageArray((const char *)&device, sizeof(device));
    QDataStream out(&messageArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_6);
    out << *this;
    return messageArray;
}

