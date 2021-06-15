/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QSCPISERIALIZER_H
#define QSCPISERIALIZER_H

#include "qabstractserializer.h"

namespace QProfile
{
    class QScpiSerializer : public QAbstractSerializer
    {
            Q_OBJECT
        public:
            QScpiSerializer(QObject *parent = nullptr);
            virtual QByteArray 	serialize(const QVariant &variant, bool *ok);
            virtual const QString tagKey()
            {
                return "scpi";
            }
    };
}
#endif // QSCPISERIALIZER_H
