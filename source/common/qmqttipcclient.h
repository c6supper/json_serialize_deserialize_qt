/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QMQTTIPCCLIENT_H
#define QMQTTIPCCLIENT_H

#include <QScopedPointer>
#include <QPointer>
#include <QHostAddress>
#include <QMap>
#include <QList>
#include <QByteArray>
#include "qabstractipcclient.h"
#include "qmqtt_client.h"

QT_BEGIN_NAMESPACE
class QMutex;
namespace QRserver
{
    class QTopicSubscriberList;
    class QTopicPublisherList;
}
QT_END_NAMESPACE

namespace QRserver
{
    class QMqttIpcClient : public QProfile::QAbstractIpcClient
    {
            Q_OBJECT
        public:
            static const quint16 INVALID_SEQ = -1;

        public:
            QMqttIpcClient(QObject *parent,
                           const QHostAddress &host = QHostAddress::LocalHost, const quint16 port = 1883);
            ~QMqttIpcClient();

            virtual void subscribe(const QString &topic, QObject *subscriber,
                                   const quint8 qos = 1);
            virtual void unsubscribe(const QString &topic, QObject *subscriber);
            virtual quint16 publish(const QString &topic, const QByteArray &message,
                                    const quint8 qos = QAbstractIpcClient::QOS1, const bool retain = false);
            virtual void connectToHost();
            virtual bool isConnectedToHost();
            static QProfile::QAbstractIpcClient *Create(QObject *parent,
                    const QHostAddress &host = QHostAddress::LocalHost, const quint16 port = 1883);

        protected slots:
            void onReceived(const QMQTT::Message &);
            void onError(const QMQTT::ClientError);
            void onSubscribed(const QString &);

        signals:
            void error(const QMQTT::ClientError);

        private:
            QPointer<QMQTT::Client> 							m_mqttClient;
            QScopedPointer<QMutex> 								m_mutex;
            QMap<QString, QPointer<QTopicSubscriberList> > 		m_topicSubMap;
            QMap<QString, QPointer<QTopicPublisherList> >		m_topicPubMap;
            quint16 											m_seq;
    };
}

#endif // QMQTTIPCCLIENT_H
