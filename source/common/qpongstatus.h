

/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QPONGSTATUS_H
#define QPONGSTATUS_H

#include "qabstractstatus.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QRserver
{
    class QPongStatus : public QProfile::QAbstractStatus
    {
            Q_OBJECT

        public:
            enum PongStatus {
                NotChecked = QProfile::QAbstractStatus::StatusButtom,
                RegisteredAuthorized,
                Unregistered,
                RegisteredUnauthorized,
                VConnectMode
            };

        public:
            QPongStatus(QObject *parent = nullptr);
            QPongStatus(QObject *parent, const QString &id,
                        const quint32 commandStatus = QProfile::QAbstractStatus::Failed);
            QPongStatus(const QVariant &statusVar);
            ~QPongStatus();
    };
}

Q_DECLARE_METATYPE(QRserver::QPongStatus)

#endif // QPONGSTATUS_H
