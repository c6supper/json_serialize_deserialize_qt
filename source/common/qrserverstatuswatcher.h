/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QRSERVERSTATUSWATCHER_H
#define QRSERVERSTATUSWATCHER_H


#include "qabstractstatuswatcher.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QRserver
{
    class QRserverStatusWatcher : public QProfile::QAbstractStatusWatcher
    {
            Q_OBJECT

        public:
            QRserverStatusWatcher(QObject *parent, const QString &topic, const QString id,
                                  const QString &method, const QVariant &expectedStatus = QVariant());
            ~QRserverStatusWatcher();
    };
}
#endif // QRSERVERSTATUSWATCHER_H
