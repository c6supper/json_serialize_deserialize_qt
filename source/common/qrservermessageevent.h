/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QRSERVERMESSAGEEVENTEVENT_H
#define QRSERVERMESSAGEEVENTEVENT_H

#include <QtGlobal>
#include <QVariant>
#include "qprofile_global.h"
#include "qabstractmessageevent.h"

namespace QRserver
{
    class QRserverMessageEvent : public
        QProfile::QAbstractMessageEvent<const QVariant>
    {
        public:
            QRserverMessageEvent(const QVariant msg):
                QProfile::QAbstractMessageEvent<const QVariant>(msg)
            {
            }
            ~QRserverMessageEvent() {}
    };
}

#endif
