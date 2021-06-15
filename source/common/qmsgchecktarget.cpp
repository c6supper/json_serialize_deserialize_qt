/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qmessagefactory.h"
#include "qmsgchecktarget.h"

using namespace QRserver;
using namespace QProfile;
using namespace rserver;

QMessageSelfRegisteration<QMsgCheckTarget> registerQMsgCheckTarget(
    rserver::eCheckTarget);


QMsgCheckTarget::QMsgCheckTarget(QObject *parent)
    : QMsgRegisterTarget(parent)
{
    setType(rserver::eCheckTarget);
}

QMsgCheckTarget::QMsgCheckTarget(const QMsgCheckTarget
                                 &message)
    : QMsgRegisterTarget(message)
{
    setType(rserver::eCheckTarget);
}

QMsgCheckTarget::~QMsgCheckTarget() {}
