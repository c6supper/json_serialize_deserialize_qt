/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qprocessmessagestatus.h"
#include "qobjectfactory.h"

using namespace QRserver;
using namespace QProfile;

QObjectSelfRegisteration<QProcessMessageStatus> registerQProcessMessageStatus;

QProcessMessageStatus::QProcessMessageStatus(QObject *parent): QAbstractStatus(
        parent)
{
}

QProcessMessageStatus::QProcessMessageStatus(const QVariant &statusVar) :
    QAbstractStatus(statusVar)
{
    fromVariant(statusVar);
}

QProcessMessageStatus::QProcessMessageStatus(QObject *parent, const QString &id,
        const quint32 status)
    : QAbstractStatus(parent, id, "", status)
{
}

QProcessMessageStatus::~QProcessMessageStatus()
{
}

