/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qrserverclient.h"
#include "qclientmodel.h"
#include "RserverConnecterHelper.h"
#include "qmessagefactory.h"
#include "qabstractbinmsg.h"
#include "qrservermessageevent.h"
#include "qabstractmodel.h"
#include "qgenericbinmessage.h"
#include "qrservertransition.h"
#include "qrservertimeoutevent.h"
#include "qrservertimeouttransition.h"
#include "qmessagedispatcher.h"
#include <QFinalState>
#include "qabstracttopic.h"
#include "qmodelfactory.h"
#include "qprocessmessagecommand.h"
#ifdef Q_OS_LINUX
#include <QDateTime>
#include "unistd.h"
#endif

using namespace QRserver;
using namespace QProfile;
using namespace rserver;

//like the profile download ,need check the last packet, more than one response
quint32 QRserverTimeoutEvent::TimeoutInterval = 40 * 1000;

QRserverClient::QRserverClient(QObject *parent)
    : QRserverContainer(parent), m_rserverConnecter(new RserverConnecter()),
      m_stateMachine(nullptr), m_stateMacheDelayEventId(-1),
      m_isMessageProcessing(false), m_lastCommandStatus(nullptr)
{
}

void QRserverClient::allocTopic()
{
    QRserverContainer::allocTopic();
    QClientModel::RserverClientCommandTopic = m_topic->getRestfulTopic(
                QAbstractTopic::command);
    QClientModel::RserverClientStatusTopic = m_topic->getRestfulTopic(
                QAbstractTopic::status);
}

bool QRserverClient::processCommands(const QString &commandId,
                                     const QString &commandType, QVariant commandVar, QVariant *status)
{
    if (!isStateMachineRunning()) {
        if (!commandType.isEmpty()) {
            if (commandType == QClientModel::CommandProcessMessage) {
                m_isMessageProcessing = processMessageSeries(commandVar);

                if (m_lastCommandStatus != nullptr) {
                    m_lastCommandStatus->deleteLater();
                }

                m_lastCommandStatus = new QProcessMessageStatus(this, commandId);
                m_lastCommandStatus->setMethod(commandType);
                m_lastCommandStatus->setResource(commandVar);
            }
        }

        return true;
    }

    return false;
}

void QRserverClient::startProcessMessage()
{
    if (!m_stateMachine.isNull())
        delete m_stateMachine.data();

    m_stateMacheDelayEventId = -1;
    m_stateMachine = new QRserverStateMachine(this);

    if (getServerIP() != m_lastIpAddress
        && !m_lastIpAddress.isEmpty()) {
        const QMetaObject *metaObj = this->metaObject();
        qprofileDebug(QtWarningMsg) << metaObj->className() << "---" << "getServerIP = "
                                    << getServerIP() << "m_lastIpAddress = " << m_lastIpAddress;
        QProcessMessageCommand::RemoveBindDevice();
        m_rserverConnecter->disconnect();
        delete m_rserverConnecter;
        m_rserverConnecter = new RserverConnecter();
    }

    m_lastIpAddress = getServerIP();
    m_rserverConnecter->registerMessageCallback(m_stateMachine,
            rserverMessageCallback);
    connect(m_stateMachine, SIGNAL(finished()), this, SLOT(stateMachineStopped()));
    connect(m_stateMachine, SIGNAL(stopped()), this, SLOT(stateMachineStopped()));
    connect(m_stateMachine, SIGNAL(started()), this, SLOT(stateMachineStarted()));
}

bool QRserverClient::isStateMachineRunning()
{
    if (m_stateMachine.isNull())
        return false;

    return (m_isMessageProcessing || m_stateMachine->isRunning());
}

void QRserverClient::stateMachineStarted()
{
    qprofileDebug(QtWarningMsg) << "stateMachine started";

    if (m_stateMacheDelayEventId < 0 && m_stateMachine != nullptr) {
        m_stateMacheDelayEventId = m_stateMachine->postDelayedEvent(
                                       new QRserverTimeoutEvent(QRserverTimeoutEvent::TimeoutInterval),
                                       QRserverTimeoutEvent::TimeoutInterval);
    }
}

void QRserverClient::stateMachineStopped()
{
    qprofileDebug(QtWarningMsg) << "stateMachine stopped";
    m_rserverConnecter->registerMessageCallback(nullptr, nullptr);
    m_stateMachine->disconnect(this);
    delete m_stateMachine.data();

    if (m_lastCommandStatus != nullptr) {
        m_lastCommandStatus->setCommandStatus(QAbstractStatus::Succeed);
        reportStatus(m_lastCommandStatus->toVariant());
        delete m_lastCommandStatus;
        m_lastCommandStatus = nullptr;
    }

    m_isMessageProcessing = false;
}

QRserverClient::~QRserverClient()
{
    delete m_rserverConnecter;
}

bool QRserverClient::initPropertiesMap(const QVariantMap &properties)
{
    if (properties.count() <= 0) {
        setProperty("tagName", "qrserverclient");
    }

    return QRserverContainer::initPropertiesMap(properties);
}

void QRserverClient::allocModel()
{
    setModel(m_clientModel = QModelFactory::construct<QClientModel>(this));
}

void QRserverClient::onModelChanged()
{
    //todo
    //    QAbstractContainer::onModelChanged();
}

void QRserverClient::customEvent(QEvent *event)
{
#ifdef Q_OS_LINUX
    static qint64 lastSendMSecs = 0;
#endif

    if (QRserverMessageEvent::messageEventType() == event->type()) {
        QRserverMessageEvent *messageEvent = (QRserverMessageEvent *)event;
        QAbstractBinMsg *message = (QAbstractBinMsg *)QRserver::QMessageFactory::create(
                                       messageEvent->message().toMap());

        if (message != nullptr) {
#ifdef Q_OS_LINUX
            qint64 sendingPeriod = QDateTime::currentMSecsSinceEpoch() - lastSendMSecs;

            if (sendingPeriod < 0)
                sendingPeriod = 0;

            if (sendingPeriod < 200) {
                usleep((200 - sendingPeriod) * 1000);
            }

            lastSendMSecs = QDateTime::currentMSecsSinceEpoch();
#endif
            const QByteArray &messageArray = message->toByteArray();
            m_rserverConnecter->sendCmd(message->type(),
                                        (quint8 *)messageArray.data(), messageArray.count());

            if (messageArray.count() > MAX_QPROFILE_DEBUG_LENGTH) {
                qprofileDebug(QtDebugMsg) << "send message " << "type = " << message->type() <<
                                          "length = " << messageArray.count();
            } else {
                qprofileDebug(QtDebugMsg) << "send message = " << message->toJson() << "type = "
                                          <<
                                          message->type() << "length = " << messageArray.count();
            }

            if (!m_stateMachine.isNull()) {
                if (m_stateMacheDelayEventId != -1) {
                    if (!m_stateMachine->cancelDelayedEvent(m_stateMacheDelayEventId)) {
                        qprofileDebug(QtDebugMsg) << "Failed to cancel delayed event.";
                    }
                }

                m_stateMacheDelayEventId = m_stateMachine->postDelayedEvent(
                                               new QRserverTimeoutEvent(QRserverTimeoutEvent::TimeoutInterval),
                                               QRserverTimeoutEvent::TimeoutInterval);
            }

            delete message;
        }
    }

    QRserverContainer::customEvent(event);
}

void QRserverClient::onSendFailed()
{
    qprofileDebug(QtWarningMsg) << "Failed to send.";
    QProcessMessageCommand::RemoveBindDevice();

    if (m_lastCommandStatus != nullptr) {
        m_lastCommandStatus->setCommandStatus(QAbstractStatus::Failed);
        reportStatus(m_lastCommandStatus->toVariant());
        delete m_lastCommandStatus;
        m_lastCommandStatus = nullptr;
    }

    emit toFinalState();
}

void QRserverClient::onClientClosed()
{
    qprofileDebug(QtWarningMsg) << "Client Closed.";
    QProcessMessageCommand::RemoveBindDevice();
    m_rserverConnecter->disconnect();
    delete m_rserverConnecter;
    m_rserverConnecter = new RserverConnecter();

    if (m_lastCommandStatus != nullptr) {
        m_lastCommandStatus->setCommandStatus(QAbstractStatus::ClientClosed);
        reportStatus(m_lastCommandStatus->toVariant());
        delete m_lastCommandStatus;
        m_lastCommandStatus = nullptr;
    }

    emit toFinalState();
}

void QRserverClient::onTimeout()
{
    qprofileDebug(QtWarningMsg) << "timeout.";
    QProcessMessageCommand::RemoveBindDevice();
    m_rserverConnecter->disconnect();
    delete m_rserverConnecter;
    m_rserverConnecter = new RserverConnecter();

    if (m_lastCommandStatus != nullptr) {
        m_lastCommandStatus->setCommandStatus(QAbstractStatus::Timeout);
        reportStatus(m_lastCommandStatus->toVariant());
        delete m_lastCommandStatus;
        m_lastCommandStatus = nullptr;
    }

    emit toFinalState();
}

void QRserverClient::onNetworkErrorState()
{
    qprofileDebug(QtWarningMsg) << "network error.";
    QProcessMessageCommand::RemoveBindDevice();
    m_rserverConnecter->disconnect();
    delete m_rserverConnecter;
    m_rserverConnecter = new RserverConnecter();

    if (m_lastCommandStatus != nullptr) {
        m_lastCommandStatus->setCommandStatus(QAbstractStatus::NetworkError);
        reportStatus(m_lastCommandStatus->toVariant());
        delete m_lastCommandStatus;
        m_lastCommandStatus = nullptr;
    }

    emit toFinalState();
}

bool QRserverClient::processMessageSeries(const QVariant &messageSeriesVar)
{
    Q_ASSERT(!m_stateMachine.isNull());
    Q_ASSERT(!m_stateMachine->isRunning());
    QMutexLocker locker(&m_mutex);
    //TODO
    /*    if (!QMessageSeries::Valid(messageSeriesVar)) {
            qprofileDebug(QtDebugMsg) << "Invalid messageSerires = " << messageSeriesVar;
            return false;
        }*/
    startProcessMessage();
    QRserverState *disconnectState = new QRserverState(m_stateMachine);
    QRserverState *connectState = new QRserverState();
    QGenericBinMessage reply(eConnect2Server, this);
    QRserverTransition *disconnect2Connect = new QRserverTransition(
        reply.toVariant(), disconnectState);
    disconnect2Connect->setTargetState(connectState);
    QMessageSeries messageSeries(this);
    messageSeries.fromVariant(messageSeriesVar.toMap());
    m_stateMachine->registerPostBackParas(messageSeries.device());
    setMessageStateMachine(m_stateMachine, connectState, &messageSeries);
    QFinalState *finalState = new QFinalState();
    QRserverTimeoutTransition *timeoutTransition = new QRserverTimeoutTransition(
        connectState);
    QState *timeoutState = new QState();
    connect(timeoutState, SIGNAL(entered()), this, SLOT(onTimeout()));
    timeoutState->addTransition(this, SIGNAL(toFinalState()), finalState);
    timeoutTransition->setTargetState(timeoutState);
    QState *sendFailedState = new QState();
    connect(sendFailedState, SIGNAL(entered()), this, SLOT(onSendFailed()));
    reply.setType(eSendCmdFail);
    QRserverTransition *transition = new QRserverTransition(reply.toVariant(),
            connectState);
    transition->setTargetState(sendFailedState);
    sendFailedState->addTransition(this, SIGNAL(toFinalState()), finalState);
    QState *clientClosedState = new QState();
    connect(clientClosedState, SIGNAL(entered()), this, SLOT(onClientClosed()));
    reply.setType(eServerClosed);
    transition = new QRserverTransition(reply.toVariant(), connectState);
    transition->setTargetState(clientClosedState);
    clientClosedState->addTransition(this, SIGNAL(toFinalState()), finalState);
    QState *networkErrorState = new QState();
    connect(networkErrorState, SIGNAL(entered()), this,
            SLOT(onNetworkErrorState()));
    reply.setType(eConnectFail);
    transition = new QRserverTransition(reply.toVariant(), disconnectState);
    transition->setTargetState(networkErrorState);
    networkErrorState->addTransition(this, SIGNAL(toFinalState()), finalState);
    m_stateMachine->addState(sendFailedState);
    m_stateMachine->addState(timeoutState);
    m_stateMachine->addState(clientClosedState);
    m_stateMachine->addState(networkErrorState);
    m_stateMachine->addState(connectState);
    m_stateMachine->addState(finalState);
    m_stateMachine->setInitialState(disconnectState);
    m_stateMachine->start();
    QTimer::singleShot(0, this, SLOT(connect2Rserver()));
    return true;
}

void QRserverClient::connect2Rserver()
{
    const QRegisterModel *registerModel =
        QModelFactory::constModel<QRegisterModel>
        (this);//cuz vconnect client has no m_clientModel->registerModel()
    qprofileDebug(QtWarningMsg) << "connect2Rserver, Ip/Port:" <<
                                getServerIP() << registerModel->serverPort();
    m_rserverConnecter->ConnectCmd(
        getServerIP().toLatin1().data(),
        registerModel->serverPort(),
        registerModel->networkDevice().toLatin1().data(),
        eConnect2Server);
}

const QString QRserverClient::getServerIP()
{
    const QRegisterModel &registerModel = m_clientModel->registerModel();
    return registerModel.serverIp();
}

void QRserverClient::setMessageStateMachine(QStateMachine *stateMachine,
        QState *parentState, QMessageSeries *messageSeries)
{
    //make sure modle has been checked valid before
    QList < QList<quint32> > messageRouteList =
        messageSeries->getMessageRouteList();
    QMap<quint32, QVariantMap> 	messageMap = messageSeries->getMessageMap();
    QMap<quint32, QRserverState *> stateMap;
    QFinalState *finalState = new QFinalState();

    for (int idx = 0; idx < messageRouteList.size(); ++idx) {
        QList<quint32> messageRoute = messageRouteList.at(idx);

        for (int sIdx = 0; sIdx < messageRoute.size(); sIdx += 2) {
            quint32 messageId = messageRoute.at(sIdx);

            if (!stateMap.contains(messageId)) {
                QRserverState *requestState = new QRserverState(parentState, this,
                        messageMap[messageId]);
                stateMap[messageId] = requestState;
            }
        }
    }

    for (int idx = 0; idx < messageRouteList.size(); ++idx) {
        QList<quint32> messageRoute = messageRouteList.at(idx);

        for (int sIdx = 1; sIdx < messageRoute.size(); sIdx += 2) {
            quint32 messageId = messageRoute.at(sIdx);

            if (!stateMap.contains(messageRoute.at(sIdx - 1))) {
                qprofileDebug(QtWarningMsg) << "Invalid message serires!!";
                return;
            }

            QRserverTransition *transition = new QRserverTransition(messageMap[messageId],
                    (QState *)stateMap[messageRoute.at(sIdx - 1)]);

            if ((sIdx + 1) < messageRoute.size()) {
                if (!stateMap.contains(messageRoute.at(sIdx + 1))) {
                    qprofileDebug(QtWarningMsg) << "Invalid message serires!!";
                    return;
                }

                transition->setTargetState((QState *)stateMap[messageRoute.at(sIdx + 1)]);
            } else {
                transition->setTargetState(finalState);
            }
        }
    }

    QMap<quint32, QRserverState *>::iterator i = stateMap.begin();
    parentState->setInitialState(*i);
    stateMachine->addState(finalState);
}

void QRserverClient::rserverMessageCallback(void *obj, unsigned int type,
        const unsigned char *data, unsigned int length)
{
    QRserverStateMachine *stateMachine = (QRserverStateMachine *)obj;
    QAbstractBinMsg *message = (QAbstractBinMsg *)QRserver::QMessageFactory::create(
                                   type, (const char *)data, length);

    if (message != nullptr) {
        QVariant messageVar = message->toVariant();
        message->setSerialNumber(
            stateMachine->postBackParas().toMap()[QDeviceInfo::PropertySerialNumber].toString());
        QMessageDispatcher::DispatchMessageEvent(message->type(),
                message->toVariant());
        stateMachine->postEvent(new QRserverMessageEvent(messageVar));
        delete message;
    }
}
