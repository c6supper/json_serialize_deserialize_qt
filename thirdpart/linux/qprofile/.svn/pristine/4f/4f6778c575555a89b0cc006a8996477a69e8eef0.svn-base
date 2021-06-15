/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qprofile_debug.h"
#include "qabstractstatuswatcher.h"
#include <QTimer>
#include "qabstractstatus.h"
#include "qgenerictopicevent.h"
#include "qabstracttopic.h"
#if defined(BUILD_X86)
#include "qjson/parser.h"
#include "qjson/qobjecthelper.h"
#else
#include <QJson/Parser>
#include <QJson/QObjectHelper>
#endif
#include <QMetaObject>
#include <QMetaMethod>

using namespace QProfile;

QAbstractStatusWatcher::QAbstractStatusWatcher(QObject *parent,
        const QString &topic, const QString id, const QString &method,
        const QVariant &expectedStatus)
    : QObject(parent), m_topic(topic), m_id(id), m_notifyMethod(method),
      m_expectedStatus(expectedStatus)
{
    Q_ASSERT(parent != nullptr);
    m_ipcClient = QAbstractIpcClient::Instance();
    connect(m_ipcClient, SIGNAL(connected()), this, SLOT(onConnected()));
    connect(m_ipcClient, SIGNAL(subscribed(const QString &)), this,
            SLOT(onSubscribed(const QString &)));

    if (m_ipcClient->isConnectedToHost()) {
        QTimer::singleShot(0, this, SLOT(onConnected()));
    }
}

QAbstractStatusWatcher::~QAbstractStatusWatcher()
{
}

const QVariant &QAbstractStatusWatcher::status()
{
    return m_status;
}

void QAbstractStatusWatcher::customEvent(QEvent *event)
{
    if (QGenericTopicEvent::topicEventType() == event->type()) {
        QGenericTopicEvent *topicEvent = static_cast<QGenericTopicEvent *>(event);
        QJson::Parser parser;
        bool ok = false;
        QVariant variant = parser.parse(topicEvent->message(), &ok);

        if (!ok) {
            qprofileDebug(QtDebugMsg) << "Get a unknown custom event! " <<
                                      topicEvent->message();
            return QObject::customEvent(event);
        }

        if (m_topic == topicEvent->topic()) {
            QAbstractStatus status(this);
            status.fromVariant(variant);

            if (status.id() != m_id) {
                /*                qprofileDebug(QtDebugMsg) << "Get a unknown status event! receive id = " <<
                                                          status.id() << "expected id = " << m_id;*/
                return QObject::customEvent(event);
            }

            m_status = variant;

            if (!m_notifyMethod.isEmpty()) {
                const QMetaObject *meta = parent()->metaObject();
                QString method = m_notifyMethod + "(QAbstractStatus)";
                int idx = meta->indexOfMethod(method.toLatin1().data());

                if (idx > -1) {
                    QMetaMethod notifyMethod = meta->method(idx);
                    bool ret;

                    if (!notifyMethod.invoke(this, Qt::DirectConnection, Q_RETURN_ARG(bool, ret),
                                             Q_ARG(QAbstractStatus, status))) {
                        qprofileDebug(QtDebugMsg) << "try notifyMethod " << m_notifyMethod <<
                                                  " failed in " << meta->className();
                    }
                } else {
                    qprofileDebug(QtDebugMsg) << "Failed to find the method " << m_notifyMethod <<
                                              " in " << meta->className();
                }
            }

            qprofileDebug(QtDebugMsg) << "Get a matched status event! receive id = " <<
                                      status.id();
            emit statusArrived();
        }
    }

    return QObject::customEvent(event);
}

void QAbstractStatusWatcher::onSubscribed(const QString &topic)
{
}

void QAbstractStatusWatcher::onConnected()
{
    m_ipcClient->subscribe(m_topic, this);
}
