/* This file is part of r-server
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QAUTOCHECKEVENT_H
#define QAUTOCHECKEVENT_H

#include "qabstractevent.h"

namespace QProfile
{
    struct QFakeAutoCheckEvent {
    };
    class QAutoCheckEvent : public QAbstractEvent<QFakeAutoCheckEvent>
    {
        private:
            QFakeAutoCheckEvent		m_fakeAutoCheckEvent;
        public:
            QAutoCheckEvent(): QAbstractEvent<QFakeAutoCheckEvent>(m_fakeAutoCheckEvent)
            {
            }
            ~QAutoCheckEvent() {}
    };
}

#endif
