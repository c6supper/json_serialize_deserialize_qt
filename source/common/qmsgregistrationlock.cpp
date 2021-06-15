/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qmessagefactory.h"
#include "qmsgregistrationlock.h"

using namespace QRserver;
using namespace rserver;

QMessageSelfRegisteration<QMsgRegistrationLock> registerQMsgRegistrationLock(
    rserver::eCheckAllExt);

QMsgRegistrationLock::QMsgRegistrationLock(QObject *parent)
    : QAbstractBinMsg(rserver::eCheckAllExt, parent), m_registrationBomb(false),
      m_lockTechId(false), m_maintenance(false)
{
}

QMsgRegistrationLock::QMsgRegistrationLock(const QMsgRegistrationLock &message)
    : QAbstractBinMsg(message)
{
}

QMsgRegistrationLock::~QMsgRegistrationLock() {}

bool QMsgRegistrationLock::lockTechId() const
{
    return m_lockTechId;
}
void QMsgRegistrationLock::setLockTechId(bool lockTechId)
{
    m_lockTechId = lockTechId;
}

bool QMsgRegistrationLock::registrationBomb() const
{
    return m_registrationBomb;
}
void QMsgRegistrationLock::setRegistrationBomb(bool registrationBomb)
{
    m_registrationBomb = registrationBomb;
}

bool QMsgRegistrationLock::maintenance() const
{
    return m_maintenance;
}
void QMsgRegistrationLock::setMaintenance(bool maintenance)
{
    m_maintenance = maintenance;
}

QDataStream &QMsgRegistrationLock::read(QDataStream &in)
{
    st_AdminExt stMessage;
    bzero(&stMessage, sizeof(st_AdminExt));

    if ((qint32)sizeof(stMessage) > in.readRawData((char *)&stMessage,
            sizeof(stMessage))) {
        m_lockTechId = !(stMessage.lockTechId == 0);
        m_registrationBomb = !(stMessage.registrationTimeBomb == 0);
    } else {
        m_lockTechId = !(stMessage.lockTechId == 0);
        m_registrationBomb = !(stMessage.registrationTimeBomb == 0);
        m_maintenance = !(stMessage.maintenance == 0);
    }

    return in;
}

const QByteArray QMsgRegistrationLock::toByteArray() const
{
    st_AdminExt stMessage;
    bzero(&stMessage, sizeof(st_AdminExt));
    QByteArray messageArray((const char *)&stMessage, sizeof(stMessage));
    QDataStream out(&messageArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_6);
    out << *this;
    return messageArray;
}

QDataStream &QMsgRegistrationLock::write(QDataStream &out) const
{
    return out;
}

