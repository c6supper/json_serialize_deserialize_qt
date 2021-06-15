/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QABSTRACTMESSAGEEVENT_H
#define QABSTRACTMESSAGEEVENT_H

#include <QEvent>
#include <QString>
#include <QtGlobal>
#include <QSharedPointer>
#include "qprofile_global.h"
#include "qprofile_debug.h"

namespace QProfile
{
    template<typename T>
    class QAbstractMessageEvent : public QEvent
    {
        public:
            QAbstractMessageEvent(const T &message): QEvent(registeredType()),
                m_message(message)
            {
            }
            ~QAbstractMessageEvent()
            {
            }

            const T &message()
            {
                return m_message;
            }

            static QEvent::Type messageEventType()
            {
                return m_eventType;
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

        private:
            const T	m_message;
    };
    template<typename T>
    QEvent::Type QAbstractMessageEvent<T>::m_eventType = QEvent::None;
}

#endif
