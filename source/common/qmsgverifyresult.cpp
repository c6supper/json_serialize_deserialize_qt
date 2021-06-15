/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qmessagefactory.h"
#include "qmsgverifyresult.h"

using namespace QRserver;
using namespace rserver;

QMessageSelfRegisteration<QMsgVerifyResult> registerQMsgVerifyResult(
    rserver::eVerifyResult);

QMsgVerifyResult::QMsgVerifyResult(QObject *parent)
    : QMsgRegisterResult(parent)
{
    setType(rserver::eVerifyResult);
}

QMsgVerifyResult::QMsgVerifyResult(const QMsgVerifyResult &message)
    : QMsgRegisterResult(message)
{
    setType(rserver::eVerifyResult);
}

QMsgVerifyResult::~QMsgVerifyResult() {}
