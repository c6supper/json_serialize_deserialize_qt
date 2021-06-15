/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */


#include "qgenericstatus.h"
#include "qobjectfactory.h"

using namespace QProfile;

QObjectSelfRegisteration<QGenericStatus> registerQGenericStatus;

QGenericStatus::QGenericStatus(QObject *parent): QAbstractStatus(parent)
{
}

QGenericStatus::QGenericStatus(QObject *parent, const QString &id,
                               const QString &topic, const quint32 commandStatus)
    : QAbstractStatus(parent, id, topic, commandStatus)
{
}

QGenericStatus::QGenericStatus(const QVariant &statusVar)
    : QAbstractStatus(statusVar)
{
    fromVariant(statusVar);
}

QGenericStatus::~QGenericStatus()
{
}

bool QGenericStatus::isValid()
{
    return (!method().isEmpty() && QAbstractStatus::isValid());
}
