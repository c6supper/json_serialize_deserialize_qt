/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qmessagefactory.h"
#include "qmsggetdeviceinfoext.h"

using namespace QRserver;
using namespace QProfile;
using namespace rserver;

QMessageSelfRegisteration<QMsgGetDeviceInfoExt> registerQMsgGetDeviceInfoExt(
    rserver::eGetDevInfoExt);

QMsgGetDeviceInfoExt::QMsgGetDeviceInfoExt(QObject *parent)
    : QMsgRegisterTargetExt(parent)
{
    setType(rserver::eGetDevInfoExt);
}

QMsgGetDeviceInfoExt::QMsgGetDeviceInfoExt(const QMsgGetDeviceInfoExt &message)
    : QMsgRegisterTargetExt(message)
{
    setType(rserver::eGetDevInfoExt);
}

QMsgGetDeviceInfoExt::~QMsgGetDeviceInfoExt() {}



