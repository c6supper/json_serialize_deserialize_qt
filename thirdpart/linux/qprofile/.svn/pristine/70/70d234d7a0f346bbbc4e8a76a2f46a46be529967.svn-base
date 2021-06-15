/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QMACATTRIBUTE_H
#define QMACATTRIBUTE_H

#include "qabstractattribute.h"

QT_BEGIN_NAMESPACE
class QVariant;
QT_END_NAMESPACE

namespace QProfile
{
    class QMacAttribute : public QAbstractAttribute
    {
            Q_GADGET
        public:
            QMacAttribute(QObject *parent = nullptr);
            QMacAttribute(const QVariant &value, QObject *parent);
            ~QMacAttribute() {}
            void toChar(char *mac);
            void fromChar(char *mac, int length);
    };
}
Q_DECLARE_METATYPE(QProfile::QMacAttribute)
#endif // QMACATTRIBUTE_H
