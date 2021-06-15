/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QPROCESSMESSAGESTATUS_H
#define QPROCESSMESSAGESTATUS_H

#include "qabstractstatus.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QRserver
{
    class QProcessMessageStatus : public QProfile::QAbstractStatus
    {
            Q_OBJECT

        public:
            QProcessMessageStatus(QObject *parent = nullptr);
            QProcessMessageStatus(const QVariant &statusVar);
            QProcessMessageStatus(QObject *parent, const QString &id,
                                  const quint32 status = QProfile::QAbstractStatus::Failed);
            ~QProcessMessageStatus();
    };
}

Q_DECLARE_METATYPE(QRserver::QProcessMessageStatus)

#endif // QPROCESSMESSAGESTATUS_H
