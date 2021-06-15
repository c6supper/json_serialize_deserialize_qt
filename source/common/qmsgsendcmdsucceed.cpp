/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qmessagefactory.h"
#include "qmsgsendcmdsucceed.h"

using namespace QRserver;
using namespace rserver;

QMessageSelfRegisteration<QMsgSendCmdSucceed> registerQMsgSendCmdSucceed(
    rserver::eSendCmdSucceed);

QDataStream &QMsgSendCmdSucceed::read(QDataStream &in)
{
    in.readRawData((char *)&m_sendMessageType, sizeof(m_sendMessageType));
    return in;
}

QDataStream &QMsgSendCmdSucceed::write(QDataStream &out) const
{
    out << m_sendMessageType;
    return out;
}

const QByteArray QMsgSendCmdSucceed::toByteArray() const
{
    QByteArray messageArray(sizeof(m_sendMessageType), m_sendMessageType);
    QDataStream out(&messageArray, QIODevice::ReadWrite);
    out << *this;
    return messageArray;
}

QMsgSendCmdSucceed::QMsgSendCmdSucceed(quint32 sendMessageType, QObject *parent)
    : QAbstractBinMsg(rserver::eSendCmdSucceed, parent),
      m_sendMessageType(sendMessageType)
{
    Q_ASSERT(sendMessageType != 0);
}

QMsgSendCmdSucceed::QMsgSendCmdSucceed(const QMsgSendCmdSucceed &message)
    : QAbstractBinMsg(message), m_sendMessageType(message.sendMessageType())
{
}

QMsgSendCmdSucceed::~QMsgSendCmdSucceed() {}
