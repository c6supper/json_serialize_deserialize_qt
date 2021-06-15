/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */


#include "qobjecthelper.h"

#include <QtCore/QMetaObject>
#include <QtCore/QMetaProperty>
#include <QtCore/QObject>
#include <QMetaType>
#include "qprofile_debug.h"
#include "qabstractcontainer.h"
#include "qobjectfactory.h"

using namespace QProfile;

class QObjectHelper::QObjectHelperPrivate
{
};

QObjectHelper::QObjectHelper()
    : d(new QObjectHelperPrivate)
{
}

QObjectHelper::~QObjectHelper()
{
    delete d;
}

QVariantMap QObjectHelper::qobject2qvariant(const QObject *object,
        const QStringList &ignoredProperties)
{
    QVariantMap result;
    const QMetaObject *metaoOject = object->metaObject();
    int count = metaoOject->propertyCount();

    for (int i = 0; i < count; ++i) {
        QMetaProperty metaProperty = metaoOject->property(i);
        const char *name = metaProperty.name();

        if (!metaProperty.isReadable()
            || ignoredProperties.contains(QLatin1String(name)))
            continue;

        QVariant value = object->property(name);
        result[QLatin1String(name)] = value;
    }

    return result;
}

void QObjectHelper::qvariant2qobject(const QVariantMap &variantMap,
                                     QObject *object)
{
    Q_ASSERT(object != nullptr);
    const QMetaObject *metaObject = object->metaObject();

    for (QVariantMap::const_iterator childIter = variantMap.constBegin(),
         end = variantMap.constEnd(); childIter != end; ++childIter) {
        QByteArray childTypeByteArray = (childIter.key()).toLatin1();
        int propertyIdx = metaObject->indexOfProperty(childTypeByteArray.data());

        if (propertyIdx < 0) {
            qprofileDebug(QtDebugMsg) << "Missing property  " << childIter.key() <<
                                      " for class"
                                      << metaObject->className();
            continue;
        }

        QMetaProperty metaProperty = metaObject->property(propertyIdx);
        QVariant::Type type = metaProperty.type();
        QVariant v(childIter.value());
        QVariant::Type userType = (type == QVariant::UserType) ?
                                  (QVariant::Type)metaProperty.userType()
                                  : type;

        if (v.canConvert(userType)) {
            v.convert(userType);
            metaProperty.write(object, v);
        } else if (QLatin1String("QVariant") == QLatin1String(
                       metaProperty.typeName())) {
            metaProperty.write(object, v);
        }
    }

#if 0
    Q_ASSERT(object != nullptr);
    QString className(object->metaObject()->className());
    QByteArray typeByteArray = className.toLatin1();

    if (QMetaType::type(typeByteArray) <= 0) {
        qprofileDebug(QtDebugMsg) << "Missing meta class type " << className;
        return;
    }

    QString nameSpace;

    if (className.lastIndexOf("::") > 0) {
        nameSpace = className.mid(0, className.lastIndexOf("::") + 2);
    }

    const QMetaObject *metaObject = object->metaObject();

    for (QVariantMap::const_iterator iter = variantMap.constBegin(),
         end = variantMap.constEnd(); iter != end; ++iter) {
        if (iter.value().canConvert<QVariantMap>()) {
            if (QAbstractContainer::isXMLElementClass(metaObject, iter.key())) {
                for (QVariantMap::const_iterator childIter = iter.value().toMap().constBegin(),
                     end = iter.value().toMap().constEnd(); childIter != end; ++childIter) {
                    if (childIter.value().canConvert<QVariantMap>()) {
                        foreach (QObject *childObject, object->children()) {
                            const QMetaObject *metaObject = childObject->metaObject();

                            if (QAbstractContainer::isXMLElementClass(metaObject, childIter.key())) {
                                QVariantMap childMap;
                                childMap[childIter.key()] = childIter.value();
                                qvariant2qobject(childMap, childObject);
                                break;
                            }
                        }

                        qprofileDebug(QtDebugMsg) << "Missing xml container class for element " <<
                                                  childIter.key();
                    } else {
                        QByteArray childTypeByteArray = (childIter.key()).toLatin1();
                        int propertyIdx = metaObject->indexOfProperty(childTypeByteArray.data());

                        if (propertyIdx < 0) {
                            qprofileDebug(QtDebugMsg) << "Missing property  " << childIter.key() <<
                                                      " for class"
                                                      << metaObject->className();
                            continue;
                        }

                        QMetaProperty metaProperty = metaObject->property(propertyIdx);
                        QVariant::Type type = metaProperty.type();
                        QVariant v(childIter.value());
                        QVariant::Type userType = (type == QVariant::UserType) ?
                                                  (QVariant::Type)metaProperty.userType()
                                                  : type;

                        if (v.canConvert(userType)) {
                            v.convert(userType);
                            metaProperty.write(object, v);
                        } else if (QLatin1String("QVariant") == QLatin1String(
                                       metaProperty.typeName())) {
                            metaProperty.write(object, v);
                        }
                    }
                }
            } else {
                qvariant2qobject(iter.value().toMap(), object);
            }
        }
    }

#endif
}

QString QObjectHelper::removeClassName(QString className)
{
    if (className.contains("::"))
        return className.mid(className.lastIndexOf("::") + 2, className.length() - 1);

    return className;
}
