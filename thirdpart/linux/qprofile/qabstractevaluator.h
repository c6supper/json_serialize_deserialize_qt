#ifndef QABSTRACTEVALUATOR_H
#define QABSTRACTEVALUATOR_H

#include "qprofile_export.h"
#include "qprofile_global.h"
#include <QObject>
#include <QPointer>
#include <QMap>
#include <QVariant>

#include "qabstractstatus.h"

QT_BEGIN_NAMESPACE
namespace QProfile
{
    class QAbstractIpcClient;
    class QAbstractTrigger;
    //class QAbstractResponse;
}
QT_END_NAMESPACE

namespace QProfile
{
    class QAbstractEvaluator : public QObject
    {
            Q_OBJECT

        public:
            enum EvaluateStep { StepStart = 0, StepButtom };
            QAbstractEvaluator(QObject *parent = nullptr);
            ~QAbstractEvaluator();

            QVariant evaluate(const QVariant para = QVariant());
            void registerTrigger(qint32 step, QAbstractTrigger *trigger);

        protected:
            quint16 publish(const QString &topic, const QByteArray &json);
            virtual bool postNextStep(qint32 step, QAbstractTrigger *trigger,
                                      QVariant responseVar, QVariant *retVar) = 0;

        private:
            QPointer<QAbstractIpcClient> m_ipcClient;
            QMap<qint32, QAbstractTrigger *> m_triggerMap;
            QVariantMap m_stepResponseMap;
    };
}

#endif /* QABSTRACTEVALUATOR_H */
