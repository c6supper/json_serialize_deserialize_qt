/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qmessagefactory.h"
#include "qmsgunregistresult.h"

using namespace QRserver;
using namespace rserver;

QMessageSelfRegisteration<QMsgUnregistResult> registerQMsgUnregistResult(
    rserver::eUnRegistResult);

QMsgUnregistResult::QMsgUnregistResult(QObject *parent)
    : QMsgRegisterResult(parent)
{
    setType(rserver::eUnRegistResult);
}

QMsgUnregistResult::QMsgUnregistResult(const QMsgUnregistResult &message)
    : QMsgRegisterResult(message)
{
    setType(rserver::eUnRegistResult);
}

QMsgUnregistResult::~QMsgUnregistResult() {}
