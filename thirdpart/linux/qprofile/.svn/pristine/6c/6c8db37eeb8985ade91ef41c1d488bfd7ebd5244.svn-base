/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QSTRINGATTRIBUTE_H
#define QSTRINGATTRIBUTE_H

#include "qabstractattribute.h"

QT_BEGIN_NAMESPACE
class QVariant;
QT_END_NAMESPACE

namespace QProfile
{
    class QStringAttribute : public QAbstractAttribute
    {
            Q_GADGET
        public:
            QStringAttribute(QObject *parent = nullptr);
            QStringAttribute(const QVariant &value, QObject *parent = nullptr);
            ~QStringAttribute() {}
            void toChar(char *string, int length);
            static void StringToChar(const QString &qString, char *string, int length);
    };
}

Q_DECLARE_METATYPE(QProfile::QStringAttribute)

#endif // QSTRINGATTRIBUTE_H
