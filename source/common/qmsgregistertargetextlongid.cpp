/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qmessagefactory.h"
#include "qmsgregistertargetextlongid.h"

using namespace QRserver;
using namespace QProfile;
using namespace rserver;

QMessageSelfRegisteration<QMsgRegisterTargetExtLongId>
registerQMsgRegisterTargetExtLongId(
    rserver::eRegisterTargetExt2);

QDataStream &QMsgRegisterTargetExtLongId::read(QDataStream &in)
{
    st_RserverDeviceInfoExtLongId stMessage;

    if ((qint32)sizeof(stMessage) <= in.readRawData((char *)&stMessage,
            sizeof(stMessage))) {
        setDeviceType(stMessage.cDeviceType);
        setSerialNumber(QString(stMessage.szSerialNum));
        setMacFromChar((char *)stMessage.Mac, sizeof(stMessage.Mac));
        setCableMacFromChar((char *)stMessage.CMMac, sizeof(stMessage.CMMac));
        setVersion(QString(stMessage.szVersion));
        setTechId(QString(stMessage.szTechID));
        setTechName(QString(stMessage.TechnicianName));
        setCompany(QString(stMessage.szCompany));
        setSupervisorId(QString(stMessage.szSupervisorID));
    }

    return in;
}

const QByteArray QMsgRegisterTargetExtLongId::toByteArray() const
{
    st_RserverDeviceInfoExtLongId device;
    bzero(&device, sizeof(st_RserverDeviceInfoExtLongId));
    QByteArray messageArray((const char *)&device, sizeof(device));
    QDataStream out(&messageArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_6);
    out << *this;
    return messageArray;
}

QDataStream &QMsgRegisterTargetExtLongId::write(QDataStream &out) const
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

QMsgRegisterTargetExtLongId::QMsgRegisterTargetExtLongId(QObject *parent)
    : QMsgRegisterTargetExt(parent)
{
    setType(rserver::eRegisterTargetExt2);
}

QMsgRegisterTargetExtLongId::QMsgRegisterTargetExtLongId(
    const QMsgRegisterTargetExtLongId
    &message)
    : QMsgRegisterTargetExt(message)
{
    setType(rserver::eRegisterTargetExt2);
}

QMsgRegisterTargetExtLongId::~QMsgRegisterTargetExtLongId()
{
}

