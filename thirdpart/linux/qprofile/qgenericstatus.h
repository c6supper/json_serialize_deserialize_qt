/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QGENERICSTATUS_H
#define QGENERICSTATUS_H

#include "qabstractstatus.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QProfile
{
    class QGenericStatus : public QAbstractStatus
    {
            Q_OBJECT

        public:
            QGenericStatus(QObject *parent = nullptr);
            QGenericStatus(QObject *parent, const QString &id, const QString &topic = "",
                           const quint32 commandStatus = QAbstractStatus::Failed);
            QGenericStatus(const QVariant &statusVar);
            ~QGenericStatus();

            virtual bool isValid();
    };
}

Q_DECLARE_METATYPE(QProfile::QGenericStatus)

#endif // QGENERICSTATUS_H
