/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qmessagefactory.h"
#include "qmsgregistresult.h"

using namespace QRserver;
using namespace rserver;

QMessageSelfRegisteration<QMsgRegisterResult> registerQMsgRegisterResult(
    rserver::eRegistResult);

QDataStream &QMsgRegisterResult::read(QDataStream &in)
{
    in >> m_status;
    return in;
}

const QByteArray QMsgRegisterResult::toByteArray() const
{
    QByteArray messageArray(sizeof(m_status), m_status);
    QDataStream out(&messageArray, QIODevice::ReadWrite);
    out << *this;
    return messageArray;
}

QDataStream &QMsgRegisterResult::write(QDataStream &out) const
{
    out << m_status;
    return out;
}

QMsgRegisterResult::QMsgRegisterResult(QObject *parent)
    : QAbstractBinMsg(rserver::eRegistResult, parent),
      m_status(-1)
{
}

QMsgRegisterResult::QMsgRegisterResult(const QMsgRegisterResult &message)
    : QAbstractBinMsg(message), m_status(message.status())
{
}

QMsgRegisterResult::~QMsgRegisterResult() {}
