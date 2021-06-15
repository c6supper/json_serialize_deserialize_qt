/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qprofile_debug.h"
#include "qrserverstatuswatcher.h"
#include <QTimer>

using namespace QProfile;
using namespace QRserver;

QRserverStatusWatcher::QRserverStatusWatcher(QObject *parent,
        const QString &topic, const QString id, const QString &method,
        const QVariant &expectedStatus)
    : QAbstractStatusWatcher(parent, topic, id, method, expectedStatus)
{
}

QRserverStatusWatcher::~QRserverStatusWatcher()
{
}

