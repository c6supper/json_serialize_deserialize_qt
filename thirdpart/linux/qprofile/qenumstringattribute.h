/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QENUMSTRINGATTRIBUTE_H
#define QENUMSTRINGATTRIBUTE_H

#include "qabstractattribute.h"
#include <QStringList>

QT_BEGIN_NAMESPACE
class QVariant;
QT_END_NAMESPACE

namespace QProfile
{
    class QEnumStringAttribute : public QAbstractAttribute
    {
            Q_GADGET

        public:
            QEnumStringAttribute(QObject *parent = nullptr): QAbstractAttribute(parent)
            {
            }
            QEnumStringAttribute(const QStringList &valueList, QObject *parent = nullptr);
            virtual bool setValue(const QVariant &value);
            ~QEnumStringAttribute() {}

        private:
            QStringList m_enumValueList;
    };
}

Q_DECLARE_METATYPE(QProfile::QEnumStringAttribute)

#endif // QENUMSTRINGATTRIBUTE_H
