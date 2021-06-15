/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */


#ifndef QABSTRACTSTATUSWATCHER_H
#define QABSTRACTSTATUSWATCHER_H

#include "qprofile_export.h"
#include "qprofile_global.h"
#include <QObject>
#include <QVariant>
#include "qabstractipcclient.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QProfile
{
    class QAbstractStatusWatcher : public QObject
    {
            Q_OBJECT

        public:
            QAbstractStatusWatcher(QObject *parent, const QString &topic, const QString id,
                                   const QString &method, const QVariant &expectedStatus = QVariant());
            ~QAbstractStatusWatcher();
            const QVariant &status();

        private slots:
            void onConnected();
            void onSubscribed(const QString &);

        protected:
            virtual void customEvent(QEvent *event);

        signals:
            void statusArrived();

        private:
            QAbstractIpcClient		*m_ipcClient;
            QString					m_topic;
            QString					m_id;
            QString					m_notifyMethod;
            QVariant				m_expectedStatus;
            QVariant				m_status;
    };
}
#endif // QABSTRACTSTATUSWATCHER_H
