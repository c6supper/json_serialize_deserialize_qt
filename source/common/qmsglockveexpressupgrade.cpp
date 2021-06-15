/*
 * qmsglockveexpressupgrade.cpp
 *
 *  Created on: Jul 23, 2020
 *      Author: duyi
 */
#include "qmessagefactory.h"
#include "qmsglockveexpressupgrade.h"

using namespace QRserver;
using namespace rserver;

QMessageSelfRegisteration<QMsgLockVeexpressUpgrade>
registerQMsgLockVeexpressUpgrade(
    rserver::eCheckAllExt2);

QDataStream &QMsgLockVeexpressUpgrade::read(QDataStream &in)
{
    in >> m_lockVeexpressUpgrade;
    return in;
}

const QByteArray QMsgLockVeexpressUpgrade::toByteArray() const
{
    QByteArray messageArray(sizeof(m_lockVeexpressUpgrade), m_lockVeexpressUpgrade);
    QDataStream out(&messageArray, QIODevice::ReadWrite);
    out << *this;
    return messageArray;
}

QDataStream &QMsgLockVeexpressUpgrade::write(QDataStream &out) const
{
    out << m_lockVeexpressUpgrade;
    return out;
}

QMsgLockVeexpressUpgrade::QMsgLockVeexpressUpgrade(QObject *parent)
    : QAbstractBinMsg(rserver::eCheckAllExt2, parent),
      m_lockVeexpressUpgrade(false)
{
}

QMsgLockVeexpressUpgrade::QMsgLockVeexpressUpgrade(const
        QMsgLockVeexpressUpgrade &message)
    : QAbstractBinMsg(message),
      m_lockVeexpressUpgrade(message.lockVeexpressUpgrade())
{
}

QMsgLockVeexpressUpgrade::~QMsgLockVeexpressUpgrade() {}



