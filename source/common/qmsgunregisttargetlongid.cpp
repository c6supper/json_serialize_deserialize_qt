/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qmessagefactory.h"
#include "qmsgunregisttargetlongid.h"

using namespace QRserver;
using namespace QProfile;
using namespace rserver;

QMessageSelfRegisteration<QMsgUnregistTargetLongId>
registerQMsgUnregistTargetLongId(
    rserver::eUnRegistTarget2);

QMsgUnregistTargetLongId::QMsgUnregistTargetLongId(QObject *parent)
    : QMsgRegisterTargetExtLongId(parent)
{
    setType(rserver::eUnRegistTarget2);
}

QMsgUnregistTargetLongId::QMsgUnregistTargetLongId(const
        QMsgUnregistTargetLongId
        &message)
    : QMsgRegisterTargetExtLongId(message)
{
    setType(rserver::eUnRegistTarget2);
}

