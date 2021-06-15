#include "qrservercontainerwidget.h"
#include "qrservertopic.h"
#include "qgenericcommand.h"
#include "qrservermessagebox.h"
#include "qrserverstatuswatcher.h"
#include "qrserverstrtable.h"
#include "qabstractstatus.h"

using namespace QRserver;
using namespace QProfile;

QRserverContainerWidget::QRserverContainerWidget(QWidget *parent,
        bool isRetainMessage)
    : QAbstractContainerWidget(parent)
{
}

const QVariant QRserverContainerWidget::issueCommand(const QVariant
        &commandVar,
        quint32 interval, const QString &method, const QVariant &exepcted)
{
    Q_ASSERT(commandVar.isValid() && interval > 0);
    qint32 retCommand = -1;

    if (commandVar.isValid()) {
        QString topic = m_topic->getRestfulTopic(QAbstractTopic::command);
        QGenericCommand command(this);
        command.fromVariant(commandVar);
        command.setTopic(topic);
        QByteArray json = command.toJson();

        if (0 == m_ipcClient->publish(topic, json, QAbstractIpcClient::QOS1)) {
            qprofileDebug(QtDebugMsg) << "Publish failed, topic = " <<
                                      m_topic->getRestfulTopic(QAbstractTopic::command) << " command.id() = " <<
                                      command.id() << " command = " << json;
            QRserverMessageBox::Information(this, TRANSLATE_STR_RSERVER_WARNING,
                                            TRANSLATE_STR_RSERVER_IPC_ERROR, QMessageBox::Ok);
        } else {
            qprofileDebug(QtWarningMsg) << sender() << this << "publish topic = " << topic
                                        <<
                                        "command.id() = " << command.id() << "command = " << json;
            QString topic = m_topic->getRestfulTopic(QAbstractTopic::status);
            QRserverStatusWatcher watcher(this, topic, command.id(), method, exepcted);
            QBaseProgress *backProgress = getBackProgress();
            connect(&watcher, SIGNAL(statusArrived()), this, SLOT(onStatusArrived()));
            retCommand = backProgress->run(interval);
            watcher.disconnect(this);

            if (QBaseProgress::Timeout == retCommand) {
                QRserverMessageBox::Information(this, TRANSLATE_STR_RSERVER_WARNING,
                                                TRANSLATE_STR_RSERVER_TIMEOUT, QMessageBox::Ok);
            } else {
                QAbstractStatus status(watcher.status());
                qprofileDebug(QtDebugMsg) << watcher.status();
                qprofileDebug(QtDebugMsg) << status.toJson();

                switch ((quint32)status.commandStatus()) {
                case QAbstractStatus::ClientClosed:
                case QAbstractStatus::NetworkError: {
                    QTimer::singleShot(0, QRserverNetworkChecker::Instance(),
                                       SIGNAL(networkError()));
                    return QVariant();
                }

                case QAbstractStatus::IPCError: {
                    QRserverMessageBox::Information(this, TRANSLATE_STR_RSERVER_WARNING,
                                                    TRANSLATE_STR_RSERVER_IPC_ERROR, QMessageBox::Ok);
                    return QVariant();
                }

                case QAbstractStatus::Timeout: {
                    QRserverMessageBox::Information(this, TRANSLATE_STR_RSERVER_WARNING,
                                                    TRANSLATE_STR_RSERVER_TIMEOUT, QMessageBox::Ok);
                    return QVariant();
                }

                default:
                    return watcher.status();
                }
            }
        }
    }

    return QVariant();
}

void QRserverContainerWidget::onStatusArrived()
{
    QBaseProgress *backProgress = getBackProgress();
    backProgress->finishWithResult(QBaseProgress::Accepted);
}
qint32 QRserverContainerWidget::issueCommand(const QVariant &commandVar,
        const QString &method, quint32 interval, quint32 checkPeriod)
{
    qint32 retCommand = -1;

    if (commandVar.isValid()) {
        QString topic = m_topic->getRestfulTopic(QAbstractTopic::command);
        QGenericCommand command(this);
        command.fromVariant(commandVar);
        command.setTopic(topic);
        QByteArray json = command.toJson();

        if (0 == m_ipcClient->publish(topic, json, QAbstractIpcClient::QOS1)) {
            qprofileDebug(QtDebugMsg) << "Publish failed, topic = " <<
                                      m_topic->getRestfulTopic(
                                          QAbstractTopic::command) << " command = " << json;
        } else {
            qprofileDebug(QtDebugMsg) << sender() << this << "publish topic = " << topic <<
                                      "command = " << json;

            if (!method.isEmpty()) {
                QBaseProgress *backProgress = getBackProgress();
                retCommand = backProgress->run(interval, this, method, checkPeriod);

                if (QBaseProgress::Timeout == retCommand) {
                    QRserverMessageBox::Information(this, TRANSLATE_STR_RSERVER_WARNING,
                                                    TRANSLATE_STR_RSERVER_TIMEOUT, QMessageBox::Ok);
                }
            }
        }
    }

    return retCommand;
}
void QRserverContainerWidget::setTagName(const QString &tagName)
{
    setProperty("tagName", tagName);
}
void QRserverContainerWidget::allocTopic()
{
    if (m_topic == NULL) {
        m_topic = new QRserverTopic(this, m_ipcClient, "",
                                    QAbstractTopic::get | QAbstractTopic::status | QAbstractTopic::trigger);
    }
}
void QRserverContainerWidget::onIpcDisconnected()
{
    QBaseProgress *backProgress = getBackProgress();

    if (backProgress->isActive()) {
        backProgress->finishWithResult(QBaseProgress::ForceClosed);
    }

    QAbstractContainerWidget::setDisabled(true);
}
void QRserverContainerWidget::onNetworkError()
{
    QBaseProgress *backProgress = getBackProgress();

    if (backProgress->isActive()) {
        backProgress->finishWithResult(QBaseProgress::ForceClosed);
    }
}
void QRserverContainerWidget::postponeInit()
{
    return QAbstractContainerWidget::postponeInit();
}
QRserverContainerWidget::~QRserverContainerWidget()
{
}
