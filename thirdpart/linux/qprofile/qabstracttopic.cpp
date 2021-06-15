/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include <QtGlobal>
#include "qabstracttopic.h"
#include "qprofile_global.h"
#include "qabstractipcclient.h"
#include <QStringList>
#include <QTimer>
#include "qprofile_debug.h"

using namespace QProfile;


//RESTFUL TOPIC
const QString QAbstractTopic::DefaultPutTopic = "put";
const QString QAbstractTopic::DefaultGetTopic = "get";
const QString QAbstractTopic::DefaultCommandTopic = "command";
const QString QAbstractTopic::DefaultStatusTopic = "status";
const QString QAbstractTopic::DefaultTriggerTopic = "trigger";
const QString QAbstractTopic::DefaultResponseTopic = "response";

QAbstractTopic::QAbstractTopic(QObject *parent, QAbstractIpcClient *ipcClient,
                               const QString &relativeTopic, RestfulApis api) : QObject(parent),
    m_relativeTopic(relativeTopic),
    m_ipcClient(ipcClient), m_api(api)
{
    Q_ASSERT(parent != nullptr && ipcClient != nullptr);
    connect(m_ipcClient, SIGNAL(connected()), this, SLOT(onConnected()));
    connect(m_ipcClient, SIGNAL(subscribed(const QString &)), this,
            SLOT(onSubscribed(const QString &)));

    if (m_ipcClient->isConnectedToHost()) {
        QTimer::singleShot(0, this, SLOT(onConnected()));
    }
}

const QString QAbstractTopic::relativeTopic(const QString &absoluteTopic)
{
    QString relativeTopic = absoluteTopic.mid(absoluteTopic.lastIndexOf("\\") + 1);
    return relativeTopic;
}

const QString QAbstractTopic::getRestfulTopic(const RestfulApi api)
{
    if (api == QAbstractTopic::get)
        return absoluteTopic(DefaultGetTopic);

    if (api == QAbstractTopic::put)
        return absoluteTopic(DefaultPutTopic);

    if (api == QAbstractTopic::command)
        return absoluteTopic(DefaultCommandTopic);

    if (api == QAbstractTopic::status)
        return absoluteTopic(DefaultStatusTopic);

    if (api == QAbstractTopic::trigger)
        return absoluteTopic(DefaultTriggerTopic);

    if (api == QAbstractTopic::response)
        return absoluteTopic(DefaultResponseTopic);

    return QString();
}

void QAbstractTopic::onSubscribed(const QString &topic)
{
    if (m_subscriberdTopic.contains(topic)) {
        m_subscriberdTopic.removeOne(topic);

        if (m_subscriberdTopic.count() == 0) {
            emit subscribeFinished();
        }
    }
}

void QAbstractTopic::onConnected()
{
    if (m_api.testFlag(get)) {
        m_ipcClient->subscribe(absoluteTopic(DefaultGetTopic), parent());
        m_subscriberdTopic << absoluteTopic(DefaultGetTopic);
    }

    if (m_api.testFlag(put)) {
        m_ipcClient->subscribe(absoluteTopic(DefaultPutTopic), parent());
        m_subscriberdTopic << absoluteTopic(DefaultPutTopic);
    }

    if (m_api.testFlag(command)) {
        m_ipcClient->subscribe(absoluteTopic(DefaultCommandTopic), parent());
        m_subscriberdTopic << absoluteTopic(DefaultCommandTopic);
    }

    if (m_api.testFlag(status)) {
        m_ipcClient->subscribe(absoluteTopic(DefaultStatusTopic), parent());
        m_subscriberdTopic << absoluteTopic(DefaultStatusTopic);
    }

    if (m_api.testFlag(trigger)) {
        m_ipcClient->subscribe(absoluteTopic(DefaultTriggerTopic), parent());
        m_subscriberdTopic << absoluteTopic(DefaultTriggerTopic);
    }

    if (m_api.testFlag(response)) {
        m_ipcClient->subscribe(absoluteTopic(DefaultResponseTopic), parent());
        m_subscriberdTopic << absoluteTopic(DefaultResponseTopic);
    }

    if (!m_relativeTopic.isEmpty()) {
        m_ipcClient->subscribe(absoluteTopic(m_relativeTopic), parent());
        m_subscriberdTopic << absoluteTopic(m_relativeTopic);
    }
}

QAbstractTopic::~QAbstractTopic() {}
