/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qprofile_debug.h"
#include "qtimerstatuswatcher.h"
#include <QTimer>

using namespace QProfile;

QTimerStatusWatcher::QTimerStatusWatcher(QObject *parent, const QString &topic,
        const QString id, quint32 timeout)
    : QAbstractStatusWatcher(parent, topic, id, "")
{
    QTimer::singleShot(timeout, this, SLOT(timeout()));
    connect(this, SIGNAL(statusArrived()), &m_eventLoop, SLOT(quit()));
}

void QTimerStatusWatcher::timeout()
{
    m_eventLoop.exit(Timeout);
}

int QTimerStatusWatcher::exec()
{
    return m_eventLoop.exec();
}

QTimerStatusWatcher::~QTimerStatusWatcher()
{
}

