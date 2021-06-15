/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qmessagefactory.h"
#include "qmsgunregisttarget.h"

using namespace QRserver;
using namespace QProfile;
using namespace rserver;

QMessageSelfRegisteration<QMsgUnregistTarget> registerQMsgUnregistTarget(
    rserver::eUnRegistTarget);

QMsgUnregistTarget::QMsgUnregistTarget(QObject *parent)
    : QMsgRegisterTarget(parent)
{
    setType(rserver::eUnRegistTarget);
}

QMsgUnregistTarget::QMsgUnregistTarget(const QMsgUnregistTarget
                                       &message)
    : QMsgRegisterTarget(message)
{
    setType(rserver::eUnRegistTarget);
}

