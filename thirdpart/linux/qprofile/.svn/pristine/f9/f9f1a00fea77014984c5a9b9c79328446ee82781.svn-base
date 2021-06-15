/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QIPATTRIBUTE_H
#define QIPATTRIBUTE_H

#include "qabstractattribute.h"

QT_BEGIN_NAMESPACE
class QVariant;
QT_END_NAMESPACE

namespace QProfile
{
    class QIpAttribute : public QAbstractAttribute
    {
            Q_GADGET
        public:
            QIpAttribute(QObject *parent = nullptr);
            QIpAttribute(const QVariant &value, QObject *parent);
            ~QIpAttribute() {}
    };
}
Q_DECLARE_METATYPE(QProfile::QIpAttribute)
#endif // QIPATTRIBUTE_H
