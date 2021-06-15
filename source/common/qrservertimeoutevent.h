/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QRSERVERTIMEOUTEVENT_H
#define QRSERVERTIMEOUTEVENT_H

#include <QtGlobal>
#include <QVariant>
#include "qprofile_global.h"
#include "qabstractmessageevent.h"

namespace QRserver
{
    class QRserverTimeoutEvent : public
        QProfile::QAbstractMessageEvent<const quint32>
    {
        public:
            static quint32 TimeoutInterval;
        public:
            QRserverTimeoutEvent(const quint32 interval):
                QProfile::QAbstractMessageEvent<const quint32>(interval)
            {
            }
            ~QRserverTimeoutEvent() {}
    };
}

#endif
