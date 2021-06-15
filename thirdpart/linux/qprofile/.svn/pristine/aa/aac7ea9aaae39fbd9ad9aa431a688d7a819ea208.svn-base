/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */


#include "qabstractattribute.h"
#include "qobjectfactory.h"
#include <QtCore/QVariant>
#include <QStringList>
#include "qabstractmodel.h"

using namespace QProfile;

QObjectSelfRegisteration<QAbstractAttribute> registerQAbstractAttribute;

bool QAbstractAttribute::isModelParent()
{
    return qobject_cast<QAbstractModel *>(m_parent) != nullptr;
}

QAbstractAttribute::QAbstractAttribute(QObject *parent): m_parent(parent),
    m_type(QVariant::Invalid), m_mutex(new QMutex())
{
    Q_ASSERT(parent != nullptr);

    if (isModelParent()) {
        QMetaObject::invokeMethod(parent, "registerAttribute",
                                  Qt::DirectConnection, Q_ARG(QAbstractAttribute *, this));
    }
}
QAbstractAttribute::QAbstractAttribute(const QVariant &value,
                                       QVariant::Type type, QObject *parent): m_parent(parent), m_type(type),
    m_value(value), m_mutex(new QMutex())
{
    Q_ASSERT(parent != nullptr);

    if (isModelParent()) {
        QMetaObject::invokeMethod(parent, "registerAttribute",
                                  Qt::DirectConnection, Q_ARG(QAbstractAttribute *, this));
    }
}
bool QAbstractAttribute::setValue(const QVariant &value)
{
    QMutexLocker locker(m_mutex);
    bool ok = false;

    if (m_type != QVariant::Invalid) {
        if (value.canConvert(m_type) || m_type == QVariant::List
            || (m_type == QVariant::StringList && value.canConvert(QVariant::String))) {
            if (!m_regExp.isValid() || m_regExp.isEmpty()) {
                ok = true;
            }

            if (m_regExp.exactMatch(value.toString())) {
                ok = true;
            }
        }
    }

    if (ok) {
        if (m_type == QVariant::StringList) {
            QStringList stringList = m_value.toStringList();
            stringList.append(value.toString());
            m_value = stringList;
        } else if (m_type == QVariant::List) {
            QVariantList valueList = m_value.toList();
            valueList.append(value);
            m_value = valueList;
        } else {
            if (m_value != value) {
                /*qprofileDebug(QtDebugMsg) << m_parent->metaObject()->className() << this <<
                                          m_value << value;*/
                m_value = value;

                if (isModelParent()) {
                    QMetaObject::invokeMethod(m_parent, "propertyChangeNotifyLater",
                                              Qt::DirectConnection);
                }
            }
        }
    } else {
        qprofileDebug(QtDebugMsg) << "Falied to convert value = " << value <<
                                  "to m_type = " <<
                                  m_type ;

        if (m_regExp.isValid()) {
            qprofileDebug(QtDebugMsg) <<  "regExp = " << m_regExp.pattern() << "error = " <<
                                      m_regExp.errorString();
        }
    }

    return ok;
}

const QVariant QAbstractAttribute::takeValue()
{
    QMutexLocker locker(m_mutex);
    QVariant ret;

    if (m_type == QVariant::StringList) {
        QStringList stringList = m_value.toStringList();

        if (stringList.count() > 0) {
            ret = stringList.takeFirst();
            m_value = stringList;
        }
    } else if (m_type == QVariant::List) {
        QVariantList valueList = m_value.toList();

        if (valueList.count() > 0) {
            ret = valueList.takeFirst();
            m_value = valueList;
        }
    } else {
        ret = m_value;
        m_value = QVariant();
    }

    return ret;
}

const QVariant QAbstractAttribute::value()
{
    QMutexLocker locker(m_mutex);
    QVariant ret;

    if (m_type == QVariant::StringList) {
        QStringList stringList = m_value.toStringList();

        if (stringList.count() > 0) {
            ret = stringList.first();
        }
    } else if (m_type == QVariant::List) {
        QVariantList valueList = m_value.toList();

        if (valueList.count() > 0) {
            ret = valueList.first();
        }
    } else
        ret = m_value;

    return ret;
}
QVariant::Type QAbstractAttribute::type() const
{
    return m_type;
}
void QAbstractAttribute::setRegExp(const QString &pattern,
                                   Qt::CaseSensitivity cs)
{
    m_regExp = QRegExp(pattern, cs);
}

QAbstractAttribute::~QAbstractAttribute()
{
    /*    qprofileDebug(QtDebugMsg) << this <<  " old = " <<
                        m_value;*/
    delete m_mutex;
}
