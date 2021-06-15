/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */


#include "qabstractmodel.h"
#include "qobjectfactory.h"
#include <QMetaProperty>
#include <QFile>

using namespace QProfile;

QObjectPointerSelfRegisteration<QAbstractModel> registerQAbstractModelPointer;

const qint32 QAbstractModel::MaxNumberErrorStored = 10;

const qint32 QAbstractModel::PropertyChangedNotifyInterval = 100;

QAbstractModel::QAbstractModel(QObject *parent,
                               const QByteArray &content) : QAbstractJsonElement(parent),
    m_commandMutex(new QMutex()), m_isValid(false)
{
    Q_UNUSED(content);
    m_isNotify = true;
    m_changeNotifyTimer.setSingleShot(true);
    m_changeNotifyTimer.setInterval(PropertyChangedNotifyInterval);
    connect(&m_changeNotifyTimer, SIGNAL(timeout()), this,
            SLOT(propertyChangeNotify()));
    QAbstractModel *parentModel = qobject_cast<QAbstractModel *>(parent);

    if (parentModel != nullptr) {
        blockNotify(!parentModel->isNotify());
        connect(this, SIGNAL(propertyChanged()), parent,
                SIGNAL(propertyChanged()));
    }

    QTimer::singleShot(0, this, SLOT(initAttributes()));
}

const QStringList QAbstractModel::ignoredProperties() const
{
    QStringList list = QAbstractJsonElement::ignoredProperties();
    list << "command" << "trigger";
    return list;
}

QAbstractModel::QAbstractModel(QObject *parent) : QAbstractJsonElement(parent),
    m_commandMutex(new QMutex())
{
    Q_ASSERT(parent != nullptr);
    m_isNotify = true;
    m_changeNotifyTimer.setSingleShot(true);
    m_changeNotifyTimer.setInterval(PropertyChangedNotifyInterval);
    connect(&m_changeNotifyTimer, SIGNAL(timeout()), this,
            SLOT(propertyChangeNotify()));
    QAbstractModel *parentModel = qobject_cast<QAbstractModel *>(parent);

    if (parentModel != nullptr) {
        blockNotify(!parentModel->isNotify());
        connect(this, SIGNAL(propertyChanged()), parent,
                SIGNAL(propertyChanged()));
    }

    QTimer::singleShot(0, this, SLOT(initAttributes()));
}

QAbstractModel::QAbstractModel(const QAbstractModel &model) :
    QAbstractJsonElement(
        model.parent()), m_commandMutex(new QMutex())
{
    m_isNotify = true;

    for (int propertyIdx = 0; propertyIdx < metaObject()->propertyCount();
         propertyIdx++) {
        QMetaProperty property = metaObject()->property(propertyIdx);
        int srcIdx = model.metaObject()->indexOfProperty(property.name());

        if (srcIdx > 0) {
            if (!setProperty(property.name(), model.property(property.name())))
                qprofileDebug(QtDebugMsg) << "set property " << property.name() <<
                                          "Failed, for class " <<
                                          metaObject()->className();
        } else {
            qprofileDebug(QtDebugMsg) << "Miss property "  << property.name() <<
                                      ", for class" <<
                                      metaObject()->className();
        }
    }

    m_changeNotifyTimer.setSingleShot(true);
    m_changeNotifyTimer.setInterval(PropertyChangedNotifyInterval);
    connect(&m_changeNotifyTimer, SIGNAL(timeout()), this,
            SLOT(propertyChangeNotify()));
    QAbstractModel *parentModel = qobject_cast<QAbstractModel *>(parent());

    if (parentModel != nullptr) {
        blockNotify(!parentModel->isNotify());
        connect(this, SIGNAL(propertyChanged()), parent(),
                SIGNAL(propertyChanged()));
    }
}

void QAbstractModel::emitModelChangedByOthers()
{
    emit modelChangedByOthers();
}

void QAbstractModel::asynPropertyChangeNotify()
{
    if (m_isNotify) {
        QTimer::singleShot(0, this, SLOT(propertyChangeNotify()));
    }
}

void QAbstractModel::propertyChangeNotify()
{
    if (m_isNotify) {
        m_changeNotifyTimer.stop();
        emit propertyChanged();
    }
}
void QAbstractModel::propertyChangeNotifyLater()
{
    if (m_isNotify) {
        m_changeNotifyTimer.start();
    }
}

bool QAbstractModel::isNotify()
{
    return m_isNotify;
}

bool QAbstractModel::blockNotify(bool block)
{
    if (block) {
        m_changeNotifyTimer.stop();
    }

    m_isNotify = !block;

    foreach (QObject *child, children()) {
        QAbstractModel *childModel = qobject_cast<QAbstractModel *>(child);

        if (childModel != nullptr) {
            childModel->blockNotify(block);
        }
    }

    return m_isNotify;
}

void QAbstractModel::registerAttribute(QAbstractAttribute *attribute)
{
    Q_ASSERT(attribute != nullptr);
    m_attributeList.append(attribute);
}

const QVariant QAbstractModel::takeCommand()
{
    QMutexLocker locker(m_commandMutex);

    if (m_commandList.isEmpty())
        return QVariant();

    return m_commandList.takeFirst();
}
const QVariant QAbstractModel::command()
{
    QMutexLocker locker(m_commandMutex);

    if (m_commandList.isEmpty())
        return QVariant();

    return m_commandList.first();
}

void QAbstractModel::appendCommand(const QVariant command)
{
    QMutexLocker locker(m_commandMutex);
    m_commandList.append(command);
    QTimer::singleShot(0, this, SIGNAL(commandPropertyChanged()));
}

bool QAbstractModel::isValid()
{
    return m_isValid;
}

const QVariant QAbstractModel::takeTrigger()
{
    QMutexLocker locker(m_commandMutex);

    if (m_triggerList.isEmpty())
        return QVariant();

    return m_triggerList.takeFirst();
}
const QVariant QAbstractModel::trigger()
{
    QMutexLocker locker(m_commandMutex);

    if (m_triggerList.isEmpty())
        return QVariant();

    return m_triggerList.first();
}

void QAbstractModel::appendTrigger(const QVariant trigger)
{
    QMutexLocker locker(m_commandMutex);
    m_triggerList.append(trigger);
    QTimer::singleShot(0, this, SIGNAL(triggerPropertyChanged()));
}

QAbstractModel::~QAbstractModel()
{
    m_commandMutex->lock();
    m_commandList.clear();
    m_triggerList.clear();
    m_attributeList.clear();
    m_commandMutex->unlock();
    delete m_commandMutex;
}
