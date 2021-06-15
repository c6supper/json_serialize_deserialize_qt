/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qabstractcontainer.h"
#include "qobjectfactory.h"
#include "qabstracttopic.h"
#include <QMetaClassInfo>
#include <QTimer>
#include "qabstractipcclient.h"
#include "qprofile_debug.h"
#include "qgenerictopicevent.h"
#include "qgenericmessage.h"
#include "qgenericcommand.h"
#include <QTimer>
#if defined(BUILD_X86)
#include "qjson/parser.h"
#include "qjson/serializer.h"
#include "qjson/qobjecthelper.h"
#else
#include <QJson/Parser>
#include <QJson/Serializer>
#include <QJson/QObjectHelper>
#endif
#include "qgenericstatus.h"
#include "qabstractcompatibility.h"
#include "qabstracttrigger.h"

using namespace QProfile;

QObjectFactory *QObjectFactory::instance = nullptr;


QAbstractContainer::QAbstractContainer(QObject *parent, bool isRetainMessage)
    : QObject(parent), m_ipcClient(nullptr), m_topic(nullptr),
      m_mutex(new QMutex()), m_triggerMutex(new QMutex()),
      m_isRetainMessage(isRetainMessage)
{
    Q_ASSERT(parent != nullptr);
    m_ipcClient = QAbstractIpcClient::Instance();
    QTimer::singleShot(0, this, SLOT(postponeInit()));
}

void QAbstractContainer::postponeInit()
{
    initPropertiesMap();
    allocModel();
    allocTopic();
}

QAbstractContainer::QAbstractContainer(const QAbstractContainer &container)
    : QObject(container.parent())
{
    *this = container;
    m_ipcClient = QAbstractIpcClient::Instance();
}

void QAbstractContainer::copyProperty(const QAbstractContainer &container)
{
    for (int propertyIdx = 0; propertyIdx < metaObject()->propertyCount();
         propertyIdx++) {
        QMetaProperty property = metaObject()->property(propertyIdx);
        int srcIdx = container.metaObject()->indexOfProperty(property.name());

        if (srcIdx > 0) {
            if (!setProperty(property.name(), container.property(property.name())))
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

const QString QAbstractContainer::tagName(const QString &key)
{
    QVariant propertyVariant = property(key.toLower().toLatin1().constData());

    if (propertyVariant.isValid()) {
        QVariantMap tagMap = propertyVariant.toMap();

        if (tagMap.contains("tagName"))
            return tagMap["tagName"].toString();
    }

    return QString();
}

QVariantMap QAbstractContainer::serialize(QAbstractSerializer *serializer,
        bool *ok)
{
    Q_ASSERT(serializer != nullptr);
    bool ret = false;
    QVariantMap serializerMap = property(
                                    serializer->tagKey().toLatin1().constData()).toMap();
    QVariantMap propertiesMap = serializerMap["propertiesMap"].toMap();
    QString tagName = serializerMap["tagName"].toString();
    qprofileDebug(QtDebugMsg) << propertiesMap << tagName;
    QVariantMap serializeRetMap, serializedProperties;

    for (int propertyIdx = 0; propertyIdx < metaObject()->propertyCount();
         propertyIdx++) {
        QMetaProperty property = metaObject()->property(propertyIdx);

        if (propertiesMap.keys().contains(QString(property.name()))) {
            serializedProperties[propertiesMap[property.name()].toString().trimmed().toLower()]
                = property.read(this);
        }
    }

    if (!tagName.isEmpty()) {
        serializeRetMap[tagName.trimmed().toLower()] = serializedProperties;
    } else {
        serializeRetMap = serializedProperties;
    }

    ret = true;

    if (ok != nullptr)
        *ok = ret;

    return serializeRetMap;
}

QAbstractContainer &QAbstractContainer::operator=(const QAbstractContainer
        &container)
{
    copyProperty(container);

    foreach (QObject *child, children()) {
        delete child;
    }

    copyChildren(container);
    return *this;
}

QAbstractContainer::~QAbstractContainer()
{
    delete m_mutex;
    delete m_triggerMutex;
}

void QAbstractContainer::customEvent(QEvent *event)
{
    if (QGenericTopicEvent::topicEventType() == event->type()) {
        QGenericTopicEvent *topicEvent = static_cast<QGenericTopicEvent *>(event);
        QJson::Parser parser;
        bool ok = false;
        QVariant variant = parser.parse(topicEvent->message(), &ok);

        if (!ok) {
            qprofileDebug(QtWarningMsg) << "Get a unknown custom event! " <<
                                        topicEvent->message();
            return QObject::customEvent(event);
        }

        if (m_topic->getRestfulTopic(QAbstractTopic::command) ==
            topicEvent->topic()) {
            if (QAbstractCommand::IsCommand(variant)) {
                model()->appendCommand(variant);
            } else {
                qprofileDebug(QtWarningMsg) << "Get a unknown command event! " <<
                                            topicEvent->message();
            }
        } else if (m_topic->getRestfulTopic(QAbstractTopic::trigger) ==
                   topicEvent->topic()) {
            if (QAbstractTrigger::IsTrigger(variant)) {
                model()->appendTrigger(variant);
            } else {
                qprofileDebug(QtWarningMsg) << "Get a unknown trigger event! " <<
                                            topicEvent->message();
            }
        } else if (m_topic->getRestfulTopic(QAbstractTopic::put) ==
                   topicEvent->topic()) {
            QGenericMessage message(this, topicEvent->topic(), variant);
            QString relativeTopic = QAbstractTopic::relativeTopic(topicEvent->topic());
            const QMetaObject *meta = metaObject();
            QString method = relativeTopic + "(QGenericMessage)";
            int idx = meta->indexOfMethod(method.toLatin1().data());

            if (idx > -1) {
                QMetaMethod restfulMethod = meta->method(idx);
                QByteArray ret;

                if (!restfulMethod.invoke(this, Qt::DirectConnection, Q_RETURN_ARG(QByteArray,
                                          ret),
                                          Q_ARG(QGenericMessage, message))) {
                    qprofileDebug(QtDebugMsg) << "try restfulMethod.invoke " << relativeTopic <<
                                              " failed in "
                                              << meta->className();
                } else {
                    //                qprofileDebug(QtDebugMsg) << m_model.data() << m_model->toJson();
                }
            } else {
                qprofileDebug(QtDebugMsg) << "Failed to find the method " << relativeTopic <<
                                          " in " <<
                                          meta->className();
            }
        }
    }

    return QObject::customEvent(event);
}

bool QAbstractContainer::isCompatible(const QString feature)
{
    QAbstractCompatibility *compatibility = QAbstractCompatibility::Instance();

    if (compatibility != nullptr) {
        return compatibility->isCompatible(feature);
    }

    return false;
}

void QAbstractContainer::reportStatus(const QVariant &status)
{
    QString topic = m_topic->getRestfulTopic(QAbstractTopic::status);
    bool ok = false;
    QJson::Serializer serializer;
    QVariantMap variantMap = status.toMap();
    variantMap[QAbstractStatus::PropertyTopic] = topic;
    QByteArray json = serializer.serialize(variantMap, &ok);

    if (ok) {
        if (json.length() <= 0) {
            qprofileDebug(QtDebugMsg) << "Failed to serialize message " << topic
                                      <<
                                      m_model->toJson() << " for " << metaObject()->className();
        } else {
            if (0 == m_ipcClient->publish(topic, json, QAbstractIpcClient::QOS1)) {
                if (json.length() <= MAX_QPROFILE_DEBUG_LENGTH) {
                    qprofileDebug(QtDebugMsg) << "Publish failed, topic = " <<
                                              m_topic->getRestfulTopic(
                                                  QAbstractTopic::status) << " message = " << json;
                } else {
                    qprofileDebug(QtDebugMsg) << "Publish failed, topic = " <<
                                              m_topic->getRestfulTopic(QAbstractTopic::status);
                }
            } else {
                if (json.length() <= MAX_QPROFILE_DEBUG_LENGTH) {
                    qprofileDebug(QtDebugMsg) << this << "publish topic = " << topic <<
                                              "content = " << json;
                }
            }
        }
    } else {
        qprofileDebug(QtCriticalMsg) << this << "Failed to json " << status;
    }
}

void QAbstractContainer::onCommand()
{
    bool ok = m_mutex->tryLock();

    if (ok) {
        QGenericCommand command(this);
        command.fromVariant(model()->takeCommand());
        qprofileDebug(QtDebugMsg) << "id = " << command.id() << ", method = " <<
                                  command.method();
        QVariant statusVar;

        if (!processCommands(command.id(), command.method(), command.resource(),
                             &statusVar)) {
            qprofileDebug(QtWarningMsg) << "processCommands failed!";
        }

        if (statusVar.isValid()) {
            reportStatus(statusVar);
        }

        m_mutex->unlock();
    } else {
        QTimer::singleShot(100, this, SLOT(onCommand()));
    }
}

bool QAbstractContainer::processCommands(const QString &commandId,
        const QString &commandType, QVariant commandVar, QVariant *status)
{
    qprofileDebug(QtDebugMsg) <<
                              "This virtual function should be implemented in subclass." << commandType <<
                              commandVar;
    Q_ASSERT(false);
    return true;
}

void QAbstractContainer::onTrigger()
{
    bool ok = m_triggerMutex->tryLock();

    if (ok) {
        QVariant triggerVar = model()->takeTrigger();
        qprofileDebug(QtDebugMsg) << "triggerVar = " << triggerVar;
        QVariant responseVar;

        if (triggerVar.isValid()) {
            if (!processTriggers(triggerVar, &responseVar)) {
                qprofileDebug(QtWarningMsg) << "processTriggers failed!";
            }

            if (responseVar.isValid()) {
                reportResponse(responseVar);
            }
        }

        m_triggerMutex->unlock();
    } else {
        QTimer::singleShot(100, this, SLOT(onTrigger()));
    }
}

bool QAbstractContainer::processTriggers(const QVariant triggerVar,
        QVariant *response)
{
    qprofileDebug(QtDebugMsg) <<
                              "This virtual function should be implemented in subclass." << triggerVar;
    Q_ASSERT(false);
    return true;
}

void QAbstractContainer::reportResponse(const QVariant &response)
{
    QString topic = m_topic->getRestfulTopic(QAbstractTopic::response);
    bool ok = false;
    QJson::Serializer serializer;
    QVariantMap variantMap = response.toMap();
    QByteArray json = serializer.serialize(variantMap, &ok);

    if (ok) {
        if (json.length() <= 0) {
            qprofileDebug(QtDebugMsg) << "Failed to serialize message " << topic <<
                                      m_model->toJson() << " for " << metaObject()->className();
        } else {
            if (0 == m_ipcClient->publish(topic, json, QAbstractIpcClient::QOS1)) {
                if (json.length() <= MAX_QPROFILE_DEBUG_LENGTH) {
                    qprofileDebug(QtDebugMsg) << "Publish failed, topic = " << topic <<
                                              " message = " << json;
                } else {
                    qprofileDebug(QtDebugMsg) << "Publish failed, topic = " << topic;
                }
            } else {
                if (json.length() <= MAX_QPROFILE_DEBUG_LENGTH) {
                    qprofileDebug(QtDebugMsg) << this << "publish topic = " << topic << "content = "
                                              << json;
                }
            }
        }
    } else {
        qprofileDebug(QtCriticalMsg) << this << "Failed to json " << response;
    }
}

void QAbstractContainer::overwriteRetainModelMessage(quint32 api)
{
    QString topic = m_topic->getRestfulTopic((QAbstractTopic::RestfulApi)api);

    if (!topic.isEmpty()) {
        QGenericMessage message(this);
        message.setTopic(topic);
        message.setContent(m_model->toVariant());
        QByteArray json = message.toJson();
        m_ipcClient->publish(topic, json, QAbstractIpcClient::QOS1, true);
    }
}

void QAbstractContainer::onModelChanged()
{
    QString topic = m_topic->getRestfulTopic(QAbstractTopic::get);
    QGenericMessage message(this);
    message.setTopic(topic);
    message.setContent(m_model->toVariant());
    QByteArray json = message.toJson();

    if (json.length() <= 0) {
        qprofileDebug(QtDebugMsg) << "Failed to serialize message " << message.topic()
                                  <<
                                  m_model->toJson() << " for " << metaObject()->className();
    } else {
        if (0 == m_ipcClient->publish(topic, json, QAbstractIpcClient::QOS1,
                                      m_isRetainMessage)) {
            if (json.length() <= MAX_QPROFILE_DEBUG_LENGTH) {
                qprofileDebug(QtDebugMsg) << "Publish failed, topic = " <<
                                          m_topic->getRestfulTopic(
                                              QAbstractTopic::get) << " message = " << json;
            } else {
                qprofileDebug(QtDebugMsg) << "Publish failed, topic = " <<
                                          m_topic->getRestfulTopic(
                                              QAbstractTopic::get);
            }
        } else {
            if (json.length() <= MAX_QPROFILE_DEBUG_LENGTH) {
                if (qobject_cast<QTimer *>(sender()) == nullptr) {
                    qprofileDebug(QtDebugMsg) << sender() << this << "publish topic = " << topic <<
                                              "content = " << json;
                }
            }
        }
    }
}

const QByteArray QAbstractContainer::put(const QGenericMessage &message)
{
    if (m_model != nullptr) {
        if (message.content() == model()->toVariant()) {
            return "";
        }

        QJson::QObjectHelper::qvariant2qobject(message.content().toMap(),
                                               m_model.data());
        return m_model->toJson();
    }

    return QByteArray();
}
