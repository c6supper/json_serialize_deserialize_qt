/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QABSTRACTSERIALIZER_H
#define QABSTRACTSERIALIZER_H

#include <QObject>
#include "qprofile_export.h"
#include "qprofile_global.h"

QT_BEGIN_NAMESPACE
class QIODevice;
QT_END_NAMESPACE

namespace QProfile
{
    class QAbstractSerializer : public QObject
    {
            Q_OBJECT
        public:
            explicit QAbstractSerializer(QObject *parent = nullptr);
            virtual void 	setDoublePrecision(int precision)
            {
                m_doublePrecision = precision;
            }
            //            virtual void 	serialize(const QVariant &variant, QIODevice *out, bool *ok) = 0;
            virtual QByteArray 	serialize(const QVariant &variant, bool *ok) = 0;
            virtual const QString tagKey() = 0;

        protected:
            int m_doublePrecision;
    };
}

#endif // QABSTRACTSERIALIZER_H
