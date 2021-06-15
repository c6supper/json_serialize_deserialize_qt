/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qpongstatus.h"
#include "qobjectfactory.h"

using namespace QRserver;
using namespace QProfile;

QObjectSelfRegisteration<QPongStatus> registerQPongStatus;

QPongStatus::QPongStatus(QObject *parent): QAbstractStatus(
        parent)
{
}

QPongStatus::QPongStatus(const QVariant &statusVar) :
    QAbstractStatus(statusVar)
{
    fromVariant(statusVar);
}

QPongStatus::QPongStatus(QObject *parent, const QString &id,
                         const quint32 commandStatus)
    : QAbstractStatus(parent, id, "", commandStatus)
{
}

QPongStatus::~QPongStatus()
{
}

