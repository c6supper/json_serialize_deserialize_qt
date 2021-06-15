/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QDATETIMEATTRIBUTE_H
#define QDATETIMEATTRIBUTE_H

#include "qabstractattribute.h"

QT_BEGIN_NAMESPACE
class QVariant;
QT_END_NAMESPACE

namespace QProfile
{
    class QDateTimeAttribute : public QAbstractAttribute
    {
            Q_GADGET
        public:
            QDateTimeAttribute(QObject *parent = nullptr);
            QDateTimeAttribute(const QVariant &value, const QVariant::Type type,
                               QObject *parent);
            ~QDateTimeAttribute() {}
    };
}
Q_DECLARE_METATYPE(QProfile::QDateTimeAttribute)
#endif // QDATETIMEATTRIBUTE_H
