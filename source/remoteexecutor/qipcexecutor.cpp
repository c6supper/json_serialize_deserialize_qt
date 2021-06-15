
#include "qipcexecutor.h"
#include "qexecutorfactory.h"
#include <QMetaProperty>
#include <QCoreApplication>
#include <QTime>
#include "qeventprogress.h"
#include "qabstractstatus.h"
#include "qrserverstatuswatcher.h"
#include "qpongstatus.h"
#include "unistd.h"
#if defined(BUILD_X86)
#include "qjson/serializer.h"
#else
#include <QJson/Serializer>
#endif
#include "qgenericcommand.h"
#include "qrservermodel.h"
#include "qabstractremotecommand.h"

using namespace QProfile;
using namespace QRserver;

QExecutorSelfRegisteration<QIpcExecutor> registerQIpcExecutor("QIpcExecutor");

QIpcExecutor::QIpcExecutor(QObject *parent)
    : QAbstractExecutor(parent)
{
    m_ipcClient = QAbstractIpcClient::Instance();

    if (!m_ipcClient->isConnectedToHost()) {
        m_ipcClient->connectToHost();
    }
}

QIpcExecutor::QIpcExecutor(const QIpcExecutor &executor)
    : QAbstractExecutor(executor)
{
    m_ipcClient = QAbstractIpcClient::Instance();

    if (!m_ipcClient->isConnectedToHost()) {
        m_ipcClient->connectToHost();
    }
}

quint32 QIpcExecutor::waitToConnected(int timeout)
{
    QTime timer;
    timer.start();

    while (1) {
        if (m_ipcClient->isConnectedToHost()) {
            break;
        }

        if (timer.elapsed() > timeout) {
            break;
        }

        QCoreApplication::processEvents(QEventLoop::AllEvents);
        usleep(100);
    }

    int remainingTime = timeout - timer.elapsed();
    return (quint32)((remainingTime > 0) ? remainingTime : 0);
}

QAbstractExecutor::RetCode QIpcExecutor::getEngineStatus()
{
    QGenericCommand pingCommand(this, QRserverModel::CommandPing,
                                QRserverModel::PingTopic);
    QByteArray json = pingCommand.toJson();

    if (0 == m_ipcClient->publish(QRserverModel::PingTopic, json)) {
        qprofileDebug(QtDebugMsg) << "publish failed, topic = " <<
                                  QRserverModel::PingTopic <<
                                  " command = " << json;
        return QAbstractExecutor::IPCError;
    } else {
        qprofileDebug(QtDebugMsg) << "publish successfully, topic =" <<
                                  QRserverModel::PingTopic
                                  <<
                                  "command = " << json;
        QRserverStatusWatcher watcher(this, QRserverModel::PongTopic, pingCommand.id(),
                                      "");
        QEventProgress *backProgress = (QEventProgress *)QEventProgress::Instance();
        connect(&watcher, SIGNAL(statusArrived()), backProgress, SLOT(quit()));
        int retCommand = backProgress->run(QRserverModel::PingTimeout);
        watcher.disconnect(backProgress);

        if (QBaseProgress::Timeout == retCommand) {
            return QAbstractExecutor::EngineIsDead;
        } else {
            QPongStatus status(watcher.status());
            qprofileDebug(QtDebugMsg) << watcher.status();

            switch ((int)status.commandStatus()) {
            case QPongStatus::NotChecked:
                return QAbstractExecutor::NotChecked;

            case QPongStatus::RegisteredAuthorized:
                return Succeed;

            case QPongStatus::RegisteredUnauthorized:
                return QAbstractExecutor::RegisteredUnauthorized;

            case QPongStatus::Unregistered:
                return QAbstractExecutor::Unregistered;

            case QPongStatus::VConnectMode:
                return QAbstractExecutor::Succeed;

            default:
                return QAbstractExecutor::UnknownError;
            }
        }
    }
}

int QIpcExecutor::exec(QAbstractRemoteCommand *cmd)
{
    // TODO:
    //cache check
    //QVariant var = loadCache(cmd->toJson()); //timeout
    //
    quint32 remainingTime = waitToConnected(cmd->timeout());

    if (remainingTime <= 0) {
        return QAbstractExecutor::IPCError;
    }

    QAbstractExecutor::RetCode engineStatus = getEngineStatus();

    if (engineStatus != QAbstractExecutor::Succeed) {
        return engineStatus;
    }

    qint32 retCommand = -1;
    QVariant toExecInput = cmd->toExecInput();

    if (toExecInput.isNull() || !toExecInput.isValid()) {
        return InvalidCommand;
    }

    bool ok = false;
    QJson::Serializer serializer;
    QByteArray json = serializer.serialize(toExecInput.toMap(), &ok);

    if (!ok) {
        return InvalidCommand;
    }

    if (0 == m_ipcClient->publish(cmd->inTopic(), json)) {
        qprofileDebug(QtDebugMsg) << "publish failed, topic = " << cmd->inTopic() <<
                                  " command = " << json;
        return IPCError;
    } else {
        qprofileDebug(QtDebugMsg) << "publish successfully, topic =" << cmd->inTopic()
                                  <<
                                  "command = " << json;
        QRserverStatusWatcher watcher(this, cmd->outTopic(),
                                      QAbstractCommand::GetId(toExecInput), "");
        QEventProgress *backProgress = (QEventProgress *)QEventProgress::Instance();
        connect(&watcher, SIGNAL(statusArrived()), backProgress, SLOT(quit()));
        retCommand = backProgress->run(remainingTime);
        watcher.disconnect(backProgress);

        if (QBaseProgress::Timeout == retCommand) {
            return Timeout;
        } else {
            QAbstractStatus status(watcher.status());
            qprofileDebug(QtDebugMsg) << watcher.status();

            switch ((quint32)status.commandStatus()) {
            case QAbstractStatus::ClientClosed:
            case QAbstractStatus::NetworkError: {
                return NetworkError;
            }

            case QAbstractStatus::IPCError: {
                return IPCError;
            }

            case QAbstractStatus::Timeout: {
                return Timeout;
            }

            case QAbstractStatus::Succeed: {
                QVariant var = cmd->parseExecOutput(watcher.status());

                if (var.isNull()) {
                    return OutputError;
                }

                return toOutput(cmd->output(), var);
            }

            default: {
                QVariant var = cmd->parseExecOutput(watcher.status());
            }

            return CustomError;
            }
        }

        return UnknownError;
    }
}

int QIpcExecutor::toOutput(const QString &output, const QVariant &var)
{
    bool ok = false;
    QJson::Serializer serializer;
    QByteArray json = serializer.serialize(var.toMap(), &ok);

    if (ok) {
        qprofileDebug(QtDebugMsg) << var;

        if (output.isNull() || output.isEmpty()) {
            fprintf(stdout, "%s\n", json.data());
            return Succeed;
        }

        if (saveFile(output, json)) {
            return Succeed;
        } else {
            fprintf(stdout, "%s\n",
                    QString("{\"error\":\"Failed to save the file (%1)\"}").
                    arg(output).toStdString().c_str());
        }
    } else {
        fprintf(stdout, "%s\n",
                QString("{\"error\":\"Failed to serialize json\"}").toStdString().c_str());
    }

    return OutputError;
}

bool QIpcExecutor::isFinished()
{
    return false;
}

QIpcExecutor::~QIpcExecutor()
{
}

