/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QTIMERSTATUSWATCHER_H
#define QTIMERSTATUSWATCHER_H

#include "qabstractstatuswatcher.h"
#include <QEventLoop>

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QProfile
{
    class QTimerStatusWatcher : public QAbstractStatusWatcher
    {
            Q_OBJECT

        public:
            enum ResultCode {Success = 0, Timeout = 1};

        public:
            QTimerStatusWatcher(QObject *parent, const QString &topic, const QString id,
                                quint32 timeout = 10000);
            ~QTimerStatusWatcher();

            int exec();

        private slots:
            void timeout();

        private:
            QEventLoop			m_eventLoop;
    };
}

#endif // QTIMERSTATUSWATCHER_H
