/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include <QtGlobal>
#include <QEvent>
#include "qprofile_debug.h"
#include "qrservertimeouttransition.h"
#include "qrservertimeoutevent.h"

using namespace QRserver;

QRserverTimeoutTransition::QRserverTimeoutTransition(QState *sourceState)
    : QAbstractTransition(sourceState)
{
    Q_ASSERT(sourceState != nullptr);
}

QRserverTimeoutTransition::~QRserverTimeoutTransition()
{
}

bool QRserverTimeoutTransition::eventTest(QEvent *e)
{
    if (e->type() == QRserverTimeoutEvent::messageEventType()
        && e->type() != QEvent::None) {
        qprofileDebug(QtDebugMsg) << "process message timeout";
        return true;
    }

    return false;
}

