/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qrservercontainer.h"
#include "qobjectfactory.h"
#include "qrservertopic.h"
#include "qgenerictopicevent.h"
#include "qabstractipcclient.h"
#include "qrservermessageevent.h"
#include "qmessagefactory.h"
#include "qmessagedispatcher.h"
#include "qabstractbinmsg.h"

using namespace QProfile;
using namespace QRserver;

QObjectSelfRegisteration<QRserverContainer> registerQRserverContainer;

QRserverContainer::QRserverContainer(QObject *parent, bool isRetainMessage)
    : QAbstractContainer(parent, isRetainMessage)
{
}

void QRserverContainer::postponeInit()
{
    return QAbstractContainer::postponeInit();
}

QRserverContainer::QRserverContainer(const QRserverContainer &container)
    : QAbstractContainer(container.parent())
{
}

void QRserverContainer::copyChildren(const QAbstractContainer &container)
{
    Q_UNUSED(container);
}

bool QRserverContainer::deSerialize(const QByteArray &qProfileString,
                                    QAbstractParser *parser)
{
    return false;
}

bool QRserverContainer::initPropertiesMap(const QVariantMap &properties)
{
    Q_UNUSED(properties);
    return true;
}

quint16 QRserverContainer::publish(const QString &topic, const QByteArray &json)
{
    return m_ipcClient->publish(topic, json,
                                QAbstractIpcClient::QOS1);
}

void QRserverContainer::allocTopic()
{
    if (m_topic == nullptr) {
        m_topic = new QRserverTopic(this, m_ipcClient);
    }
}

void QRserverContainer::subscribeMessage(const quint32 type)
{
    QMessageDispatcher::RegisterSubscriber(type, this);
}

void QRserverContainer::processMessage(QAbstractBinMsg *message)
{
    Q_UNUSED(message);
}

void QRserverContainer::customEvent(QEvent *event)
{
    if (QRserverMessageEvent::messageEventType() == event->type()) {
        QRserverMessageEvent *messageEvent = (QRserverMessageEvent *)event;
        QAbstractBinMsg *message = (QAbstractBinMsg *)QRserver::QMessageFactory::create(
                                       messageEvent->message().toMap());

        if (message != nullptr) {
            processMessage(message);
            delete message;
        }
    }

    QAbstractContainer::customEvent(event);
}
