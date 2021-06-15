/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */


#include "qabstractcommand.h"
#include <QMetaProperty>
#include "qabstractcompatibility.h"

using namespace QProfile;

const quint32 QAbstractCommand::RetryCountLimitation = 3;
const QString QAbstractCommand::PropertyId = "id";

QAbstractCommand::QAbstractCommand(QObject *parent): QAbstractJsonElement(
        parent),
    m_id(QUuid::createUuid()), m_from(""), m_to(""), m_topic(""), m_method("")
{
    Q_ASSERT(parent != nullptr);
}

QAbstractCommand::QAbstractCommand(const QAbstractCommand &command)
{
    for (int propertyIdx = 0; propertyIdx < metaObject()->propertyCount();
         propertyIdx++) {
        QMetaProperty property = metaObject()->property(propertyIdx);
        int srcIdx = command.metaObject()->indexOfProperty(property.name());

        if (srcIdx > 0) {
            if (!setProperty(property.name(), command.property(property.name())))
                qprofileDebug(QtDebugMsg) << "set property " << property.name() <<
                                          "Failed, for class " <<
                                          metaObject()->className();
        } else {
            qprofileDebug(QtDebugMsg) << "Miss property "  << property.name() <<
                                      ", for class" <<
                                      metaObject()->className();
        }
    }
}

QAbstractCommand::QAbstractCommand(QObject *parent, const QString &topic,
                                   const QString &method, const QVariant &resource)
    : QAbstractJsonElement(parent), m_id(QUuid::createUuid()), m_topic(topic),
      m_method(method),
      m_resource(resource)
{
}

bool QAbstractCommand::IsCommand(const QVariant &variant)
{
    QVariantMap varMap = variant.toMap();
    return (varMap.contains("method") && varMap.contains("resource"));
}

QAbstractCommand::QAbstractCommand(QObject *parent, const QString &topic,
                                   const QVariant &command)
    : QAbstractJsonElement(parent), m_id(QUuid::createUuid()), m_topic(topic)
{
    fromVariant(command);
}
QAbstractCommand::~QAbstractCommand()
{
}

const QString QAbstractCommand::GetId(const QVariant &variant)
{
    QVariantMap cmdVariantMap = variant.toMap();
    return cmdVariantMap[QAbstractCommand::PropertyId].toString();
}

bool QAbstractCommand::isValid()
{
    return (!m_type.isEmpty() && !m_id.isNull());
}

bool QAbstractCommand::isCompatible(const QString feature)
{
    QAbstractCompatibility *compatibility = QAbstractCompatibility::Instance();

    if (compatibility != nullptr) {
        return compatibility->isCompatible(feature);
    }

    return false;
}

