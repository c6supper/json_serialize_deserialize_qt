/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qmessagefactory.h"
#include "qmsgregistertargetext.h"

using namespace QRserver;
using namespace QProfile;
using namespace rserver;

QMessageSelfRegisteration<QMsgRegisterTargetExt> registerQMsgRegisterTargetExt(
    rserver::eRegisterTargetExt);

QDataStream &QMsgRegisterTargetExt::read(QDataStream &in)
{
    st_RserverDeviceInfoExt stMessage;
    QMsgRegisterTarget::read(in);

    if ((qint32)sizeof(stMessage.szSupervisorID) <= in.readRawData((
                char *)&stMessage.szSupervisorID,
            sizeof(stMessage.szSupervisorID))) {
        m_supervisorId->setValue(QVariant(stMessage.szSupervisorID));
    }

    return in;
}

const QByteArray QMsgRegisterTargetExt::toByteArray() const
{
    st_RserverDeviceInfoExt device;
    bzero(&device, sizeof(st_RserverDeviceInfoExt));
    QByteArray messageArray((const char *)&device, sizeof(device));
    QDataStream out(&messageArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_6);
    out << *this;
    return messageArray;
}

QDataStream &QMsgRegisterTargetExt::write(QDataStream &out) const
{
    st_RserverDeviceInfoExt stMessage;
    bzero(&stMessage, sizeof(stMessage));
    QMsgRegisterTarget::write(out);
    m_supervisorId->toChar(stMessage.szSupervisorID,
                           sizeof(stMessage.szSupervisorID));
    out.writeRawData((const char *)&stMessage.szSupervisorID,
                     sizeof(stMessage.szSupervisorID));
    return out;
}

QMsgRegisterTargetExt::QMsgRegisterTargetExt(QObject *parent)
    : QMsgRegisterTarget(parent)
{
    setType(rserver::eRegisterTargetExt);
    initAttributes();
}

QMsgRegisterTargetExt::QMsgRegisterTargetExt(const QMsgRegisterTargetExt
        &message)
    : QMsgRegisterTarget(message)
{
    setType(rserver::eRegisterTargetExt);
    initAttributes();
}

void QMsgRegisterTargetExt::initAttributes()
{
    QMsgRegisterTarget::initAttributes();
    m_supervisorId = QSharedPointer<QStringAttribute>(new QStringAttribute(this));
}

QMsgRegisterTargetExt::~QMsgRegisterTargetExt() {}

QString QMsgRegisterTargetExt::supervisorId() const
{
    return m_supervisorId->value().toString();
}
void QMsgRegisterTargetExt::setSupervisorId(QString supervisorId)
{
    m_supervisorId->setValue(supervisorId);
}

