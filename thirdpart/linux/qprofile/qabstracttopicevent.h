/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QABSTRACTTOPICEVENT_H
#define QABSTRACTTOPICEVENT_H

#include <QEvent>
#include <QString>
#include <QtGlobal>
#include "qprofile_global.h"
#include "qprofile_debug.h"

namespace QProfile
{
    template<typename T, typename M>
    class QAbstractTopicEvent : public QEvent
    {
        public:
            QAbstractTopicEvent(const T &topic, const M &msg): QEvent(registeredType()),
                m_topic(topic), m_message(msg)
            {
                Q_ASSERT(topic != nullptr && msg != nullptr);
            }
            ~QAbstractTopicEvent() {}

            const T &topic()
            {
                return m_topic;
            }

            const M &message()
            {
                return m_message;
            }

            static QEvent::Type topicEventType()
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
            const T 	m_topic;
            const M		m_message;
    };
    template<typename T, typename M>
    QEvent::Type QAbstractTopicEvent<T, M>::m_eventType = QEvent::None;
}

#endif
