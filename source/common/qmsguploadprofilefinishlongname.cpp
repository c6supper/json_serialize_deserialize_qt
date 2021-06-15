/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qmessagefactory.h"
#include "qmsguploadprofilefinishlongname.h"

using namespace QRserver;
using namespace QProfile;
using namespace rserver;

QMessageSelfRegisteration<QMsgUploadProfileFinishLongName>
registerQMsgUploadProfileFinishLongName(eUploadProfileFinishLongName);

QMsgUploadProfileFinishLongName::QMsgUploadProfileFinishLongName(
    QObject *parent)
    : QMsgUploadProfileBlockLongName(parent)
{
    setType(eUploadProfileFinishLongName);
}

QMsgUploadProfileFinishLongName::QMsgUploadProfileFinishLongName(const
        QMsgUploadProfileFinishLongName
        &message)
    : QMsgUploadProfileBlockLongName(message)
{
    setType(eUploadProfileFinishLongName);
}

quint32 QMsgUploadProfileFinishLongName::blockLength() const
{
    quint32 lastLength = totalLength() %
                         QMsgUploadProfileBlockLongName::blockLength();

    if (lastLength == 0) {
        lastLength = QMsgUploadProfileBlockLongName::blockLength();
    }

    return lastLength;
}

QMsgUploadProfileFinishLongName::~QMsgUploadProfileFinishLongName()
{
}







