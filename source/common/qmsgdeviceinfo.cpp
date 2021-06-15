/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qmessagefactory.h"
#include "qmsgdeviceinfo.h"

using namespace QRserver;
using namespace QProfile;
using namespace rserver;

QMessageSelfRegisteration<QMsgDeviceInfo> registerQMsgDeviceInfo(
    rserver::eDeviceInfo);

QMsgDeviceInfo::QMsgDeviceInfo(QObject *parent)
    : QMsgRegisterTarget(parent)
{
    setType(rserver::eDeviceInfo);
}

QMsgDeviceInfo::QMsgDeviceInfo(const QMsgDeviceInfo
                               &message)
    : QMsgRegisterTarget(message)
{
    setType(rserver::eDeviceInfo);
}


QMsgDeviceInfo::~QMsgDeviceInfo() {}
