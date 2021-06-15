/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QABSTRACTIPCCLIENT_H
#define QABSTRACTIPCCLIENT_H

#include <QObject>
#include <QPointer>
#include "qprofile_global.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QProfile
{
    class QAbstractIpcClient : public QObject
    {
            Q_OBJECT

        public:

            enum MessageQos {QOS0 = 0, QOS1 = 1, QOS2 = 2};
            Q_ENUMS(MessageQos)

            QAbstractIpcClient(QObject *parent): QObject(parent) {}
            ~QAbstractIpcClient() {};
            static QAbstractIpcClient *Instance()
            {
                return m_instance;
            }
            static QAbstractIpcClient *setInstance(QAbstractIpcClient *instance)
            {
                QAbstractIpcClient *previous = instance;
                m_instance = instance;
                return previous;
            }

            virtual void subscribe(const QString &topic, QObject *subscriber,
                                   const quint8 qos = 1) = 0;
            virtual void unsubscribe(const QString &topic, QObject *subscriber) = 0;
            virtual quint16 publish(const QString &topic, const QByteArray &message,
                                    const quint8 qos = QOS1, const bool retain = false) = 0;
            virtual void connectToHost() = 0;
            virtual bool isConnectedToHost() = 0;

        signals:
            void connected();
            void disconnected();
            void subscribed(const QString &);

        private:
            static QPointer<QAbstractIpcClient> m_instance;
    };
}

#endif // QABSTRACTIPCCLIENT_H
