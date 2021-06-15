/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QGENERICTOPICEVENT_H
#define QGENERICTOPICEVENT_H

#include <QtGlobal>
#include "qprofile_global.h"
#include "qabstracttopicevent.h"

namespace QProfile
{
    class QGenericTopicEvent : public
        QAbstractTopicEvent<const QString, const QByteArray>
    {
        public:
            QGenericTopicEvent(const QString &topic,
                               const QByteArray &msg):
                QAbstractTopicEvent<const QString, const QByteArray>(topic, msg)
            {
            }
            ~QGenericTopicEvent() {}
    };
}

#endif
