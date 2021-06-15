#ifndef QTIMERRESPONSEWATCHER_H
#define QTIMERRESPONSEWATCHER_H

#include "qtimerstatuswatcher.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QProfile
{
    class QTimerResponseWatcher : public QProfile::QTimerStatusWatcher
    {
            Q_OBJECT

        public:
            QTimerResponseWatcher(QObject *parent, const QString &topic, const QString id,
                                  quint32 timeout = 10000);
            ~QTimerResponseWatcher();
    };
}

#endif /* QTIMERRESPONSEWATCHER_H */