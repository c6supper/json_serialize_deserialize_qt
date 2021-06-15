/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qmessagefactory.h"
#include "qmsgprofileend.h"

using namespace QRserver;
using namespace QProfile;
using namespace rserver;

QMessageSelfRegisteration<QMsgProfileEnd> registerQMsgProfileEnd(
    rserver::eProfileFileEnd);

QMsgProfileEnd::QMsgProfileEnd(QObject *parent)
    : QMsgProfileBlk(parent)
{
    setType(rserver::eProfileFileEnd);
}

QMsgProfileEnd::QMsgProfileEnd(const QMsgProfileEnd
                               &message)
    : QMsgProfileBlk(message)
{
    setType(rserver::eProfileFileEnd);
}

quint32 QMsgProfileEnd::blockLength() const
{
    quint32 lastLength = totalLength() % QMsgUploadProfileBlock::blockLength();

    if (lastLength == 0) {
        lastLength = QMsgUploadProfileBlock::blockLength();
    }

    return lastLength;
}

QMsgProfileEnd::~QMsgProfileEnd() {}

