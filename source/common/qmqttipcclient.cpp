/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qmqttipcclient.h"
#include "qmqtt_client.h"
#include "qtopicsubscriber.h"
#include "qmqtt_message.h"
#include "qprofile_debug.h"
#include <QMutexLocker>
#include <QCoreApplication>
#include <QTimer>

using namespace QRserver;
using namespace QProfile;
using namespace QMQTT;

QPointer<QAbstractIpcClient> QAbstractIpcClient::m_instance = nullptr;

QMqttIpcClient::QMqttIpcClient(QObject *parent,
                               const QHostAddress &host, const quint16 port)
    : QAbstractIpcClient(parent), m_mqttClient(new QMQTT::Client(host, port, this)),
      m_mutex(new QMutex()), m_seq(0)
{
#if 0 /* clientId can not be duplicated */
    QString clientId = QString(host.toIPv4Address()) +
                       QCoreApplication::applicationName() + QString::number(
                           QCoreApplication::applicationPid());
    m_mqttClient->setClientId(clientId);
#endif
    m_mqttClient->setAutoReconnect(true);
    m_mqttClient->setAutoReconnectInterval(1000);
    connect(m_mqttClient, SIGNAL(subscribed(const QString &)), this,
            SLOT(onSubscribed(const QString &)));
    connect(m_mqttClient, SIGNAL(subscribed(const QString &)), this,
            SIGNAL(subscribed(const QString &)));
    connect(m_mqttClient, SIGNAL(received(const QMQTT::Message &)), this,
            SLOT(onReceived(const QMQTT::Message &)));
    connect(m_mqttClient, SIGNAL(connected()), this, SIGNAL(connected()));
    connect(m_mqttClient, SIGNAL(disconnected()), this, SIGNAL(disconnected()));
    connect(m_mqttClient, SIGNAL(error(const QMQTT::ClientError)), this,
            SLOT(onError(const QMQTT::ClientError)));
}

QProfile::QAbstractIpcClient *QMqttIpcClient::Create(QObject *parent,
        const QHostAddress &host, const quint16 port)
{
    if (nullptr == QAbstractIpcClient::Instance()) {
        QAbstractIpcClient::setInstance(new QMqttIpcClient(parent, host, port));
    }

    return QAbstractIpcClient::Instance();
}

void QMqttIpcClient::connectToHost()
{
    m_mqttClient->connectToHost();
}

bool QMqttIpcClient::isConnectedToHost()
{
    return m_mqttClient->isConnectedToHost();
}

void QMqttIpcClient::onReceived(const QMQTT::Message &message)
{
    QString topic = message.topic();
    QMutexLocker locker(m_mutex.data());

    if (m_topicSubMap.contains(topic)) {
        m_topicSubMap[topic]->onReceived(message.payload());
    }
}

void QMqttIpcClient::subscribe(const QString &topic, QObject *subscriber,
                               const quint8 qos)
{
    if (m_mqttClient->isConnectedToHost()) {
        QMutexLocker locker(m_mutex.data());
        m_mqttClient->subscribe(topic, qos);

        if (!m_topicSubMap.contains(topic))
            m_topicSubMap[topic] = new QTopicSubscriberList(this, subscriber, topic);
        else
            m_topicSubMap[topic]->registerSubscriber(subscriber);
    }
}

void QMqttIpcClient::unsubscribe(const QString &topic,
                                 QObject *subscriber)
{
    if (m_mqttClient->isConnectedToHost()) {
        Q_ASSERT(subscriber != nullptr);
        QMutexLocker locker(m_mutex.data());
        m_mqttClient->unsubscribe(topic);

        if (m_topicSubMap.contains(topic))
            m_topicSubMap[topic]->unregisterSubscriber(subscriber);
    }
}

quint16 QMqttIpcClient::publish(const QString &topic, const QByteArray &message,
                                const quint8 qos, const bool retain)
{
    if (m_mqttClient->isConnectedToHost()) {
        QMutexLocker locker(m_mutex.data());

        if (m_seq == INVALID_SEQ)
            m_seq++;

        QMQTT::Message qmqttMessage(m_seq++, topic, message, qos, retain);
        return m_mqttClient->publish(qmqttMessage);
    }

    return 0;
}

void QMqttIpcClient::onError(const QMQTT::ClientError error)
{
    qprofileDebug(QtDebugMsg) << "QMqttIpcClient::onError errno = " << error;
}

void QMqttIpcClient::onSubscribed(const QString &topic)
{
    qprofileDebug(QtDebugMsg) << topic << "subscribed!";
}

QMqttIpcClient::~QMqttIpcClient()
{
    m_mqttClient->disconnectFromHost();
    m_topicSubMap.clear();
    m_topicPubMap.clear();
}
