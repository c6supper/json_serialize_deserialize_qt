/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QTOPICSUBSCRIBER_H
#define QTOPICSUBSCRIBER_H

#include <QObject>
#include <QPointer>
#include <QList>
#include <QMutexLocker>
#include <QCoreApplication>
#include "qrservertopic.h"
#include "qprofile_debug.h"
#include "qgenerictopicevent.h"
#include <QtGlobal>

namespace QRserver
{
    class QTopicSubscriberList : public QObject
    {
            Q_OBJECT

        public:
            QTopicSubscriberList(QObject *parent, QObject *subscriber,
                                 const QString &topic): QObject(parent),
                m_subscribed(false), m_topic(topic)
            {
                Q_ASSERT(!topic.isEmpty());
                registerSubscriber(subscriber);
            }
            ~QTopicSubscriberList()
            {
                m_subscriberList.clear();
            }

            bool isSubscribed()
            {
                return m_subscribed;
            }
            void setSubscribed(bool subscribed)
            {
                m_subscribed = subscribed;
            }

            void registerSubscriber(QObject *subscriber)
            {
                QMutexLocker locker(&m_mutex);

                if (!m_subscriberList.contains(subscriber)) {
                    m_subscriberList.append(subscriber);
                    connect(subscriber, SIGNAL(destroyed(QObject *)), this,
                            SLOT(subDestroyed(QObject *)));

                    if (m_retainMsg.length() > 0) {
                        QCoreApplication::postEvent(subscriber,
                                                    new QProfile::QGenericTopicEvent(m_topic, m_retainMsg));
                    }
                }
            }

            void unregisterSubscriber(QObject *subscriber)
            {
                Q_ASSERT(subscriber != nullptr);
                QMutexLocker locker(&m_mutex);

                if (m_subscriberList.contains(subscriber)) {
                    subscriber->disconnect(this);
                    m_subscriberList.removeAll(subscriber);
                }
            }

            void onReceived(const QByteArray &message)
            {
                QMutexLocker locker(&m_mutex);

                for (int idx = 0; idx < m_subscriberList.count(); idx++) {
                    QCoreApplication::postEvent(m_subscriberList.at(idx),
                                                new QProfile::QGenericTopicEvent(m_topic, message));
                }

                m_retainMsg = message;
            }

        protected slots:
            void subDestroyed(QObject *subscriber)
            {
                QMutexLocker locker(&m_mutex);
                m_subscriberList.removeAll(subscriber);
            }

        protected:
            void setRetainMsg(const QByteArray &msg)
            {
                m_retainMsg = msg;
            }
            const QByteArray &getRetainMsg()
            {
                return m_retainMsg;
            }

        private:
            QList<QObject * >			m_subscriberList;
            QByteArray					m_retainMsg;
            bool						m_subscribed;
            QMutex						m_mutex;
            const QString				m_topic;
    };
}

#endif
