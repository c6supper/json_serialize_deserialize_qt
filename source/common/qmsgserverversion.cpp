/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qmessagefactory.h"
#include "qmsgserverversion.h"

using namespace QRserver;
using namespace QProfile;
using namespace rserver;

QMessageSelfRegisteration<QMsgServerVersion> registerQMsgServerVersion(
    rserver::eServerVersion);

QMsgServerVersion::QMsgServerVersion(QObject *parent)
    : QMsgGetSoftwareVersion(parent)
{
    setType(rserver::eServerVersion);
}

QMsgServerVersion::QMsgServerVersion(const QMsgServerVersion
                                     &message)
    : QMsgGetSoftwareVersion(message)
{
}

QMsgServerVersion::~QMsgServerVersion() {}

QString QMsgServerVersion::serverVersion() const
{
    return softwareVersion();
}
void QMsgServerVersion::setServerVersion(QString serverVersion)
{
    setSoftwareVersion(serverVersion);
}
