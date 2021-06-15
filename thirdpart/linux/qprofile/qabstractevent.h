/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QABSTRACTEVENT_H
#define QABSTRACTEVENT_H

#include <QEvent>
#include <QtGlobal>
#include "qprofile_global.h"
#include "qprofile_debug.h"

namespace QProfile
{
    template<typename T>
    class QAbstractEvent : public QEvent
    {
        public:
            QAbstractEvent(const T &t): QEvent(registeredType()), m_objectT(t)
            {
            }
            ~QAbstractEvent() {}

            static QEvent::Type EventType()
            {
                return m_eventType;
            }

            const T &objectT()
            {
                return m_objectT;
            }

        private:
            static QEvent::Type registeredType()
            {
                if (m_eventType == QEvent::None) {
                    m_eventType = static_cast<QEvent::Type>(QEvent::registerEventType());
                }

                return m_eventType;
            }
            static QEvent::Type m_eventType;
            const T				m_objectT;
    };
    template<typename T>
    QEvent::Type QAbstractEvent<T>::m_eventType = QEvent::None;
}

#endif
