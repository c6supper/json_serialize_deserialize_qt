#include "qabstractevaluator.h"
#include "qabstracttrigger.h"
#include "qabstractresponse.h"
#include "qabstractipcclient.h"
#include "qtimerresponsewatcher.h"

using namespace QProfile;

QAbstractEvaluator::QAbstractEvaluator(QObject *parent)
    : QObject(parent), m_ipcClient(nullptr)
{
    m_ipcClient = QAbstractIpcClient::Instance();
}

QAbstractEvaluator::~QAbstractEvaluator()
{
    QMap<qint32, QAbstractTrigger *>::iterator iter;

    for (iter = m_triggerMap.begin(); iter != m_triggerMap.end(); ++iter) {
        QAbstractTrigger *trigger = iter.value();
        trigger->deleteLater();
        m_triggerMap.remove(iter.key());
    }
}

void QAbstractEvaluator::registerTrigger(qint32 step,
        QAbstractTrigger *trigger)
{
    if (!trigger->isValid()) {
        /* if the trigger is invalid, the evaluator will be error */
        qprofileDebug(QtFatalMsg) << trigger->toJson();
        return;
    }

    if (m_triggerMap.contains(step)) {
        QAbstractTrigger *l_trigger = m_triggerMap.value(step);
        l_trigger->deleteLater();
        m_triggerMap.remove(step);
    }

    qprofileDebug(QtDebugMsg) << trigger->toJson();
    m_triggerMap.insert(step, trigger);
}

quint16 QAbstractEvaluator::publish(const QString &topic,
                                    const QByteArray &json)
{
    return m_ipcClient->publish(topic, json, QAbstractIpcClient::QOS1);
}

QVariant QAbstractEvaluator::evaluate(const QVariant para)
{
    QVariant stepRetVar;
    QAbstractResponse response;
    response.setTriggerResponse((QAbstractResponse::TriggerResponse)
                                QAbstractStatus::Succeed);
    QMap<qint32, QAbstractTrigger *>::iterator iter;

    for (iter = m_triggerMap.begin(); iter != m_triggerMap.end(); ++iter) {
        stepRetVar.clear();
        QAbstractTrigger *trigger = iter.value();
        trigger->setResource(para);
        qprofileDebug(QtWarningMsg) << trigger->toJson();

        if (0 == publish(trigger->triggerTopic(), trigger->toJson())) {
            qprofileDebug(QtWarningMsg) << "publish command failed!";
            response.setTriggerResponse((QAbstractResponse::TriggerResponse)
                                        QAbstractStatus::IPCError);
            break;
        } else {
            QTimerResponseWatcher watcher(this, trigger->responseTopic(),
                                          trigger->id(), trigger->timeout());

            if (watcher.exec() == QTimerResponseWatcher::Timeout) {
                qprofileDebug(QtWarningMsg) << "QTimerResponseWatcher timeout " <<
                                            trigger->metaObject()->className();
                response.setTriggerResponse((QAbstractResponse::TriggerResponse)
                                            QAbstractStatus::Timeout);
                break;
            }

            m_stepResponseMap[trigger->metaObject()->className()] = watcher.status();

            if (!postNextStep(iter.key(), trigger, watcher.status(), &stepRetVar)) {
                break;
            }
        }
    }

    if (!stepRetVar.isValid()) {
        QVariantMap map;
        map[QAbstractStatus::PropertyCommandStatus] = (QAbstractStatus::CommandStatus)
                response.triggerResponse();
        stepRetVar = QVariant(map);
    }

    QVariantMap retMap = stepRetVar.toMap();
    retMap[QAbstractResponse::StepResponseMap] = m_stepResponseMap;
    qprofileDebug(QtWarningMsg) << retMap;
    return QVariant(retMap);
}

