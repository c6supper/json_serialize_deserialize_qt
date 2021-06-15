/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QRSERVERCLIENT_H
#define QRSERVERCLIENT_H

#include "qprofile_global.h"
#include "qrserverstate.h"
#include <QObject>
#include <QSharedPointer>
#include <QPointer>
#include "qclientmodel.h"
#include "qrservercontainer.h"
#include <QStateMachine>
#include <QMutexLocker>
#include "qrserverstatemachine.h"
#include "qabstractstatus.h"
#include "qprocessmessagestatus.h"

QT_BEGIN_NAMESPACE
namespace rserver
{
    class RserverConnecter;
}
QT_END_NAMESPACE

namespace QRserver
{
    class QRserverClient: public QRserverContainer
    {
            Q_OBJECT

        public:
            QRserverClient(QObject *parent = nullptr);
            ~QRserverClient();

            static void rserverMessageCallback(void *obj, unsigned int type,
                                               const unsigned char *data, unsigned int length);

        protected slots:
            virtual bool initPropertiesMap(const QVariantMap &properties =
                                               QVariantMap());
            virtual	void allocModel();

            virtual void onModelChanged();
            void stateMachineStopped();
            void stateMachineStarted();
            void onSendFailed();
            void onClientClosed();
            void onNetworkErrorState();
            void onTimeout();
            void connect2Rserver();

        protected:
            virtual void	customEvent(QEvent *event);
            void startProcessMessage();
            virtual bool processCommands(const QString &commandId,
                                         const QString &commandType, QVariant commandVar,
                                         QVariant *status = nullptr);
            virtual void allocTopic();
            virtual const QString getServerIP();
            QString m_lastIpAddress;

        private:
            bool processMessageSeries(const QVariant &messageSeriesVar);
            bool isStateMachineRunning();
            void setMessageStateMachine(QStateMachine *stateMachine,
                                        QState *parentState, QMessageSeries *messageSeries);

        signals:
            void toFinalState();

        private:
            rserver::RserverConnecter					*m_rserverConnecter;
            QPointer<QRserverStateMachine>				m_stateMachine;
            QPointer<QClientModel>						m_clientModel;
            QMutex										m_mutex;
            qint32										m_stateMacheDelayEventId;
            bool										m_isMessageProcessing;
            QProcessMessageStatus 						*m_lastCommandStatus;
    };
}


#endif /* QRSERVERCLIENT_H */
