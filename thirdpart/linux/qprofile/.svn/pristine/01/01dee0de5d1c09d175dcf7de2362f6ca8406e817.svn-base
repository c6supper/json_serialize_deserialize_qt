#ifndef QABSTRACTTRIGGER_H
#define QABSTRACTTRIGGER_H

#include "qabstractcommand.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QProfile
{
    class QAbstractTrigger : public QProfile::QAbstractCommand
    {
            Q_OBJECT

        public:
            static const QString PropertyId;
            static const QString PropertyType;
            static const QString PropertyResource;
            static const QString DefaultTriggerTopic;
            static const QString DefaultResponseTopic;
            static const quint32 DefaultTriggerTimeout;

        public:
            QAbstractTrigger(QObject *parent = nullptr,
                             const QVariant resource = QVariant());
            QAbstractTrigger(const QAbstractTrigger &trigger);
            ~QAbstractTrigger();

            static bool IsTrigger(const QVariant &variant);
            virtual bool isValid();
            virtual void pureVirtualMock() {};
            virtual const QString triggerTopic() = 0;
            virtual const QString responseTopic() = 0;
            virtual const quint32 timeout();

        protected slots:
            virtual void initAttributes();
    };
}

#endif /* QABSTRACTTRIGGER_H */
