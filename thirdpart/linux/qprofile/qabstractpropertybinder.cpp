/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qprofile_debug.h"
#include "qabstractpropertybinder.h"
#include <QMetaProperty>
#include <QMetaObject>
#include <QWidget>

using namespace QProfile;

const QString QAbstractPropertyBinder::PropertyChangeNotifySignalName =
    "propertyChanged()";

QAbstractPropertyBinder::QAbstractPropertyBinder(QObject *view, QObject *model)
    : QObject(view), m_model(model), m_view(view), m_mutex(new QMutex())
{
    Q_ASSERT(view != nullptr && model != nullptr);
    connect(model, SIGNAL(destroyed()), this, SLOT(destroyBinder()));
    bindPropertyChanged(model);
    bindPropertyChanged(view);
}

void QAbstractPropertyBinder::bindPropertyChanged(QObject *notifier)
{
    const QMetaObject *notifierMeta = notifier->metaObject();
    QMetaMethod propertyChangedMethod = metaObject()->method(
                                            metaObject()->indexOfSlot("propertyChanged()"));
    int idx = 0;

    for (; idx < notifierMeta->methodCount(); ++idx) {
        QMetaMethod method = notifierMeta->method(idx);

        if (method.methodType() == QMetaMethod::Signal) {
            QString methodSignature(method.signature());

            if (methodSignature.endsWith(PropertyChangeNotifySignalName)) {
                connect(notifier, method, this, propertyChangedMethod);
                break;
            }
        }
    }

    if (idx >= notifierMeta->methodCount()) {
        qprofileDebug(QtFatalMsg) << "No " << PropertyChangeNotifySignalName <<
                                  " found in class " << notifier;
    }
}
void QAbstractPropertyBinder::destroyBinder()
{
    QMutexLocker locker(m_mutex);
    deleteLater();
}

const QStringList QAbstractPropertyBinder::ignoredPropertyList =
    IgnoredPropertyList();

const QStringList QAbstractPropertyBinder::IgnoredPropertyList()
{
    QStringList ignoredPropertyList;

    for (int propertyIdx = 0;
         propertyIdx < QWidget::staticMetaObject.propertyCount(); propertyIdx++) {
        QMetaProperty property = QWidget::staticMetaObject.property(propertyIdx);
        ignoredPropertyList.append(property.name());
    }

    return ignoredPropertyList;
}

bool QAbstractPropertyBinder::isIgnoredProperty(const QString property)
{
    return ignoredPropertyList.contains(property);
}

void QAbstractPropertyBinder::copyProperty(QObject *src, QObject *dest)
{
    QMutexLocker locker(m_mutex);
    /*    qprofileDebug(QtDebugMsg) << "bind properties from " <<
                                  src->metaObject()->className() << " to " << dest->metaObject()->className();*/
    const QMetaObject *metaoOject = src->metaObject();

    for (int idx = 0; idx < metaoOject->propertyCount(); ++idx) {
        QMetaProperty metaProperty = metaoOject->property(idx);
        const char *name = metaProperty.name();

        if (!metaProperty.isReadable())
            continue;

        if (isIgnoredProperty(QString(name))) {
            continue;
        }

        QVariant srcValue = src->property(name);
        QVariant destValue = dest->property(name);
        /*        qprofileDebug(QtDebugMsg) << "copy property name = " << name << ", srcValue = "
                                          <<
                                          srcValue << " destValue = " << destValue;*/

        if (srcValue != destValue) {
            dest->setProperty(name, srcValue);
        }
    }
}
void QAbstractPropertyBinder::propertyChanged()
{
    if (sender() == m_view) {
        copyProperty(m_view, m_model);
    }

    if (sender() == m_model) {
        copyProperty(m_model, m_view);
    }
}
QAbstractPropertyBinder::~QAbstractPropertyBinder()
{
    delete m_mutex;
}
