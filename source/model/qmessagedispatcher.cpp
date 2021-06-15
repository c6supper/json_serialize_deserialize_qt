/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qmessagedispatcher.h"
#include "qrservermessageevent.h"
#include <QCoreApplication>

using namespace QRserver;

QMessageDispatcher *QMessageDispatcher::instance = nullptr;

QMessageDispatcher *QMessageDispatcher::Instance()
{
    if (QMessageDispatcher::instance == nullptr)
        QMessageDispatcher::instance = new QMessageDispatcher();

    return instance;
}

void QMessageDispatcher::DispatchMessageEvent(const quint32 type,
        QVariant message)
{
    Instance()->dispatchMessageEvent(type, message);
}

void QMessageDispatcher::RegisterSubscriber(const quint32 type,
        QObject *subscriber)
{
    Instance()->registerSubscriber(type, subscriber);
}

void QMessageDispatcher::UnregisterSubscriber(const quint32 type,
        QObject *subscriber)
{
    Instance()->unregisterSubscriber(type, subscriber);
}

void QMessageDispatcher::registerSubscriber(const quint32 type,
        QObject *subscriber)
{
    QMutexLocker locker(&m_mutex);
    Q_ASSERT(subscriber != nullptr);

    if (!m_type2SubscriberListMap.contains(type)) {
        m_type2SubscriberListMap[type] = QList<QObject *>();
    }

    m_type2SubscriberListMap[type].append(subscriber);
    connect(subscriber, SIGNAL(destroyed(QObject *)), this,
            SLOT(onSubscriberDestroyed(QObject *)));
}

void QMessageDispatcher::unregisterSubscriber(const quint32 type,
        QObject *subscriber)
{
    QMutexLocker locker(&m_mutex);
    Q_ASSERT(subscriber != nullptr);

    if (m_type2SubscriberListMap.contains(type)) {
        m_type2SubscriberListMap[type].removeAll(subscriber);
        subscriber->disconnect(this);
    }
}
void QMessageDispatcher::unregisterSubscriber(QObject *subscriber)
{
    QMutexLocker locker(&m_mutex);
    Q_ASSERT(subscriber != nullptr);
    QMap<quint32, QList<QObject *> >::iterator iter;

    for (iter = m_type2SubscriberListMap.begin();
         iter != m_type2SubscriberListMap.end(); ++iter) {
        iter->removeAll(subscriber);
    }
}

void QMessageDispatcher::onSubscriberDestroyed(QObject *subscriber)
{
    unregisterSubscriber(subscriber);
}

void QMessageDispatcher::dispatchMessageEvent(const quint32 type,
        QVariant message)
{
    QMutexLocker locker(&m_mutex);

    if (m_type2SubscriberListMap.contains(type)) {
        QList<QObject *>::const_iterator i;

        for (i = m_type2SubscriberListMap[type].constBegin();
             i != m_type2SubscriberListMap[type].constEnd(); ++i) {
            QCoreApplication::postEvent(*i, new QRserverMessageEvent(message));
        }
    }
}

