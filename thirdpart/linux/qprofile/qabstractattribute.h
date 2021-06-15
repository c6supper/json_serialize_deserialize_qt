/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */


#ifndef QABSTRACTATTRIBUTE_H
#define QABSTRACTATTRIBUTE_H

#include "qprofile_export.h"
#include "qprofile_debug.h"
#include "qprofile_global.h"
#include <QMetaType>
#include <QMutexLocker>
#include <QRegExp>

QT_BEGIN_NAMESPACE
class QVariant;
QT_END_NAMESPACE

namespace QProfile
{
    class QAbstractAttribute
    {
            Q_GADGET
        public:
            QAbstractAttribute(QObject *parent = nullptr);
            QAbstractAttribute(const QVariant &value, QVariant::Type type,
                               QObject *parent = nullptr);
            virtual bool setValue(const QVariant &value);
            virtual const QVariant value();
            virtual const QVariant takeValue();
            virtual ~QAbstractAttribute();
            virtual QVariant::Type type() const;

            virtual void setRegExp(const QString &pattern,
                                   Qt::CaseSensitivity cs = Qt::CaseInsensitive);

        protected:
            QObject						*m_parent;
            const QVariant::Type 		m_type;
            QVariant 					m_value;
            QRegExp 					m_regExp;
            QMutex						*m_mutex;

        protected:
            bool isModelParent();
    };
}
Q_DECLARE_METATYPE(QProfile::QAbstractAttribute)
#endif // QABSTRACTATTRIBUTE_H
