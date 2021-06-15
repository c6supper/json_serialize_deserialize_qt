/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qmessagefactory.h"
#include "qmsgprofileinfoend.h"

using namespace QRserver;
using namespace QProfile;
using namespace rserver;

QMessageSelfRegisteration<QMsgProfileInfoEnd> registerQMsgProfileInfoEnd(
    rserver::eProfileInfoEnd);

QMsgProfileInfoEnd::QMsgProfileInfoEnd(QObject *parent)
    : QMsgProfileInfo(parent)
{
    setType(rserver::eProfileInfoEnd);
}

QMsgProfileInfoEnd::QMsgProfileInfoEnd(const QMsgProfileInfoEnd
                                       &message)
    : QMsgProfileInfo(message)
{
    setType(rserver::eProfileInfoEnd);
}

QMsgProfileInfoEnd::~QMsgProfileInfoEnd() {}


