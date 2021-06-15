/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */


#include "qenumstringattribute.h"
#include "qobjectfactory.h"
#include <QtCore/QVariant>

using namespace QProfile;

QObjectSelfRegisteration<QEnumStringAttribute> registerQEnumStringAttribute;

QEnumStringAttribute::QEnumStringAttribute(const QStringList &valueList,
        QObject *parent)
    : QAbstractAttribute(valueList.first(), QVariant::String, parent),
      m_enumValueList(valueList)
{
}

bool QEnumStringAttribute::setValue(const QVariant &value)
{
    if (!value.canConvert<QString>()) {
        qprofileDebug(QtDebugMsg) << "Can't convert value = " << value << "to QString";
        return false;
    }

    QString newValue = value.toString().trimmed();

    if (m_enumValueList.contains(newValue) && m_value != newValue) {
        return QAbstractAttribute::setValue(value);
    }

    qprofileDebug(QtDebugMsg) << "Falied to convert the value = " << value <<
                              " to QString";
    return false;
}
