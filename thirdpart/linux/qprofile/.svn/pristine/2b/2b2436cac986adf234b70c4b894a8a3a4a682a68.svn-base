/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qgenericmessage.h"
#include "qobjectfactory.h"

using namespace QProfile;

QObjectSelfRegisteration<QGenericMessage> registerQGenericMessage;

QGenericMessage::QGenericMessage(const QGenericMessage &message,
                                 QObject *parent)
    : QAbstractJsonElement(parent)
{
    m_id = message.id();
    m_from = message.from();
    m_to = message.to();
    m_topic = message.topic();
    m_content = message.content();
    m_metadata = message.metadata();
}

QGenericMessage::QGenericMessage(QObject *parent)
    : QAbstractJsonElement(parent), m_id(""), m_from(""), m_to(""),
      m_topic("")
{
}

/*QGenericMessage::QGenericMessage(QObject *parent, const QString &topic,
                                 const QByteArray &message,
                                 const QString &type, const QByteArray &metaData,
                                 const QString &id, const QString &from, const QString &to)
    : QObject(parent), m_id(id), m_from(from), m_to(to), m_type(type),
      m_topic(topic), m_metadata(metaData)
{
    QJson::Parser parser;
    bool ok = false;
    QVariant variant = parser.parse(message, &ok);
    QJson::QObjectHelper::qvariant2qobject(variant.toMap(), this);

    if (!ok) {
        qprofileDebug(QtDebugMsg) << "QGenericMessage qjson parse fail, variant = " <<
                                  variant;
    }
}*/

QGenericMessage::QGenericMessage(QObject *parent, const QString &topic,
                                 const QVariant &messageVar)
    : QAbstractJsonElement(parent), m_topic(topic)
{
    if (messageVar.type() != QVariant::Map) {
        qprofileDebug(QtFatalMsg) << "Only QVariantMap is allowed!";
    }

    fromVariant(messageVar);
}
