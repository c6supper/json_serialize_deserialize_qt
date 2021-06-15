/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include <QtGlobal>
#include "qmessagefactory.h"
#include "qgenericbinmessage.h"

using namespace QRserver;
using namespace rserver;

QMessageSelfRegisteration<QGenericBinMessage> registereConnect2Server(
    rserver::eConnect2Server);

QMessageSelfRegisteration<QGenericBinMessage> registereConnecPkgUp(
    rserver::eConnectPkgUp);

QMessageSelfRegisteration<QGenericBinMessage> registereDownloadFail(
    rserver::eDownloadFail);

QMessageSelfRegisteration<QGenericBinMessage> registereUpgradeFail(
    rserver::eUpgradeFail);

QMessageSelfRegisteration<QGenericBinMessage> registereSendCmdFail(
    rserver::eSendCmdFail);

QMessageSelfRegisteration<QGenericBinMessage> registereServerClosed(
    rserver::eServerClosed);

QMessageSelfRegisteration<QGenericBinMessage> registereGetSoftwareVer(
    rserver::eGetSoftwareVer);

QMessageSelfRegisteration<QGenericBinMessage> registereTestsetSWStopSend(
    rserver::eTestsetSWStopSend);

QMessageSelfRegisteration<QGenericBinMessage> registereConnectFail(
    rserver::eConnectFail);

QDataStream &QGenericBinMessage::read(QDataStream &in)
{
    return in;
}

const QByteArray QGenericBinMessage::toByteArray() const
{
    return QByteArray();
}

QDataStream &QGenericBinMessage::write(QDataStream &out) const
{
    return out;
}

QGenericBinMessage::QGenericBinMessage(const quint32 type, QObject *parent)
    : QAbstractBinMsg(type, parent)
{
    Q_ASSERT(type != 0);
}

QGenericBinMessage::QGenericBinMessage(const QGenericBinMessage &message)
    : QAbstractBinMsg(message)
{
}

QGenericBinMessage::~QGenericBinMessage() {}
