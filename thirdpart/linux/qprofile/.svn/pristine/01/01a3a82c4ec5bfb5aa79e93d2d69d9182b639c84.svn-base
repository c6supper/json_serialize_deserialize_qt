/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QBYTEARRAYATTRIBUTE_H
#define QBYTEARRAYATTRIBUTE_H

#include "qabstractattribute.h"

QT_BEGIN_NAMESPACE
class QVariant;
QT_END_NAMESPACE

namespace QProfile
{
    class QByteArrayAttribute : public QAbstractAttribute
    {
            Q_GADGET
        public:
            QByteArrayAttribute(QObject *parent = nullptr);
            QByteArrayAttribute(const QVariant &value, const QVariant::Type type,
                                QObject *parent);
            ~QByteArrayAttribute() {}
    };
}
Q_DECLARE_METATYPE(QProfile::QByteArrayAttribute)
#endif // QBYTEARRAYATTRIBUTE_H
