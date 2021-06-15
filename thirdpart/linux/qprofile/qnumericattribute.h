/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QNUMERICATTRIBUTE_H
#define QNUMERICATTRIBUTE_H

#include "qabstractattribute.h"

QT_BEGIN_NAMESPACE
class QVariant;
QT_END_NAMESPACE

namespace QProfile
{
    class QNumericAttribute : public QAbstractAttribute
    {
            Q_GADGET
        public:
            QNumericAttribute(QObject *parent = nullptr);
            QNumericAttribute(const QVariant &value, const QVariant::Type type,
                              QObject *parent);
            ~QNumericAttribute() {}
    };
}
Q_DECLARE_METATYPE(QProfile::QNumericAttribute)
#endif // QNUMERICATTRIBUTE_H
