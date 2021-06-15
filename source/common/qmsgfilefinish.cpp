/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qmessagefactory.h"
#include "qmsgfilefinish.h"

using namespace QRserver;
using namespace QProfile;
using namespace rserver;

QMessageSelfRegisteration<QMsgFileFinish>
registerQMsgFileFinish(eFileFinish);

QMsgFileFinish::QMsgFileFinish(QObject *parent)
    : QMsgFileBlock(parent)
{
    setType(eFileFinish);
}

QMsgFileFinish::QMsgFileFinish(const QMsgFileFinish
                               &message)
    : QMsgFileBlock(message)
{
    setType(eFileFinish);
}

QMsgFileFinish::~QMsgFileFinish()
{
}


