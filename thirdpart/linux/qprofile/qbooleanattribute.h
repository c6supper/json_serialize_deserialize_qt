/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QBOOLEANATTRIBUTE_H
#define QBOOLEANATTRIBUTE_H

#include "qabstractattribute.h"

QT_BEGIN_NAMESPACE
class QVariant;
QT_END_NAMESPACE

namespace QProfile
{
    class QBooleanAttribute : public QAbstractAttribute
    {
            Q_GADGET
        public:
            QBooleanAttribute(QObject *parent = nullptr);
            QBooleanAttribute(const bool &value, QObject *parent = nullptr);
            ~QBooleanAttribute() {}
    };
}

Q_DECLARE_METATYPE(QProfile::QBooleanAttribute)

#endif // QBOOLEANATTRIBUTE_H
