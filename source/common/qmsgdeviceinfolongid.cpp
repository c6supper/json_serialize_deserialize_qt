/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qmessagefactory.h"
#include "qmsgdeviceinfolongid.h"

using namespace QRserver;
using namespace QProfile;
using namespace rserver;

QMessageSelfRegisteration<QMsgDeviceInfoLongId> registerQMsgDeviceInfoLongId(
    rserver::eDeviceInfo2);

QMsgDeviceInfoLongId::QMsgDeviceInfoLongId(QObject *parent)
    : QMsgRegisterTargetExtLongId(parent)
{
    setType(rserver::eDeviceInfo2);
}

QMsgDeviceInfoLongId::QMsgDeviceInfoLongId(const QMsgDeviceInfoLongId
        &message)
    : QMsgRegisterTargetExtLongId(message)
{
    setType(rserver::eDeviceInfo2);
}


QMsgDeviceInfoLongId::~QMsgDeviceInfoLongId() {}
