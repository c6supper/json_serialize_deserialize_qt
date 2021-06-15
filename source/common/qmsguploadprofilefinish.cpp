/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qmessagefactory.h"
#include "qmsguploadprofilefinish.h"

using namespace QRserver;
using namespace QProfile;
using namespace rserver;

QMessageSelfRegisteration<QMsgUploadProfileFinish>
registerQMsgUploadProfileFinish(eUploadProfileFinish);

QMsgUploadProfileFinish::QMsgUploadProfileFinish(QObject *parent)
    : QMsgUploadProfileBlock(parent)
{
    setType(eUploadProfileFinish);
}

QMsgUploadProfileFinish::QMsgUploadProfileFinish(const QMsgUploadProfileFinish
        &message)
    : QMsgUploadProfileBlock(message)
{
    setType(eUploadProfileFinish);
}

quint32 QMsgUploadProfileFinish::blockLength() const
{
    quint32 lastLength = totalLength() % QMsgUploadProfileBlock::blockLength();

    if (lastLength == 0) {
        lastLength = QMsgUploadProfileBlock::blockLength();
    }

    return lastLength;
}

QMsgUploadProfileFinish::~QMsgUploadProfileFinish()
{
}


