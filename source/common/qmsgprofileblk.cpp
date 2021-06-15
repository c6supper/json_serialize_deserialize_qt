/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qmessagefactory.h"
#include "qmsgprofileblk.h"

using namespace QRserver;
using namespace QProfile;
using namespace rserver;

QMessageSelfRegisteration<QMsgProfileBlk> registerQMsgProfileBlk(
    rserver::eProfileFileBlk);

QMsgProfileBlk::QMsgProfileBlk(QObject *parent)
    : QMsgUploadProfileBlock(parent)
{
    setType(rserver::eProfileFileBlk);
}

QMsgProfileBlk::QMsgProfileBlk(const QMsgProfileBlk
                               &message)
    : QMsgUploadProfileBlock(message)
{
    setType(rserver::eProfileFileBlk);
}

QMsgProfileBlk::~QMsgProfileBlk()
{
}


