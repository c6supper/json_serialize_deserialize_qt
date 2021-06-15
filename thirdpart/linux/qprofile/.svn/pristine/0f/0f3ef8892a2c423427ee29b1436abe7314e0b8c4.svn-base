/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */


#include "qabstractstatus.h"
#include <QMetaProperty>

using namespace QProfile;

const QString QAbstractStatus::PropertyTopic = "topic";
const QString QAbstractStatus::PropertyCommandStatus = "commandStatus";

QAbstractStatus::QAbstractStatus(QObject *parent): QAbstractJsonElement(parent),
    m_id(""), m_from(""), m_to(""), m_topic(""), m_method(""),
    m_commandStatus(Failed)
{
    Q_ASSERT(parent != nullptr);
}

QAbstractStatus::QAbstractStatus(const QVariant &statusVar)
{
    fromVariant(statusVar);
}

QAbstractStatus::QAbstractStatus(const QAbstractStatus &status)
{
    for (int propertyIdx = 0; propertyIdx < metaObject()->propertyCount();
         propertyIdx++) {
        QMetaProperty property = metaObject()->property(propertyIdx);
        int srcIdx = status.metaObject()->indexOfProperty(property.name());

        if (srcIdx > 0) {
            if (!setProperty(property.name(), status.property(property.name())))
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

QAbstractStatus::QAbstractStatus(QObject *parent, const QString &id,
                                 const QString &topic, const quint32 commandStatus)
    : QAbstractJsonElement(parent), m_id(id), m_topic(topic),
      m_commandStatus(commandStatus)
{
    Q_ASSERT(parent != nullptr);
}

QAbstractStatus::~QAbstractStatus()
{
}

bool QAbstractStatus::isValid()
{
    return (!m_id.isEmpty());
}

const QAbstractStatus::CommandStatus QAbstractStatus::commandStatus() const
{
    return (QAbstractStatus::CommandStatus)m_commandStatus;
}

void QAbstractStatus::setCommandStatus(const QAbstractStatus::CommandStatus
                                       commandStatus)
{
    m_commandStatus = (quint32)commandStatus;
}

bool QAbstractStatus::canRecover()
{
    return (m_commandStatus != NetworkError && m_commandStatus != IPCError);
}

