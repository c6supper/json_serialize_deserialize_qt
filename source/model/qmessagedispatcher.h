/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QMESSAGEDISPATCHER_H
#define QMESSAGEDISPATCHER_H

#include "qprofile_global.h"
#include "qprofile_debug.h"
#include <QMap>
#include <QList>
#include <QObject>
#include <QMutexLocker>

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QRserver
{
    class QMessageDispatcher : public QObject
    {
            Q_OBJECT

        private:
            static QMessageDispatcher 				*instance;
            QMap<quint32, QList<QObject *> >		m_type2SubscriberListMap;
            QMutex									m_mutex;

        public:
            static QMessageDispatcher *Instance();
            static void RegisterSubscriber(const quint32 type, QObject *subscriber);
            static void UnregisterSubscriber(const quint32 type, QObject *subscriber);
            static void DispatchMessageEvent(const quint32 type, QVariant message);

        protected:
            void registerSubscriber(const quint32 type, QObject *subscriber);
            void unregisterSubscriber(const quint32 type, QObject *subscriber);
            void unregisterSubscriber(QObject *subscriber);
            void dispatchMessageEvent(const quint32 type, QVariant message);

        protected slots:
            void onSubscriberDestroyed(QObject *subscriber);
    };

}

#endif // QMESSAGEDISPATCHER_H
