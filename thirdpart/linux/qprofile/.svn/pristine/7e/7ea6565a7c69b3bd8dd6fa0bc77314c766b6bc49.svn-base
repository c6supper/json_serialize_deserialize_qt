#ifndef QABSTRACTRESPONSE_H
#define QABSTRACTRESPONSE_H

#include "qabstractstatus.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QProfile
{
    class QAbstractResponse : public QProfile::QAbstractStatus
    {
            Q_OBJECT

            Q_ENUMS(TriggerResponse)
            Q_PROPERTY(QString serialNumber READ serialNumber WRITE setSerialNumber)
            Q_PROPERTY(TriggerResponse triggerResponse READ triggerResponse WRITE
                       setTriggerResponse)

        public:
            static const QString PropertyId;
            static const QString PropertyType;
            static const QString PropertyTriggerResponse;
            static const QString StepResponseMap;

            enum TriggerResponse {
                ParseMsgError = QProfile::QAbstractStatus::StatusButtom,
                ReplyIsNull,
                ResponseButtom
            };

        public:
            QAbstractResponse(QObject *parent = nullptr);
            QAbstractResponse(const QVariant &responseVar);
            ~QAbstractResponse();

            const QString serialNumber() const;
            void setSerialNumber(const QString serialNumber);

            virtual const TriggerResponse triggerResponse() const;
            virtual  void setTriggerResponse(const TriggerResponse triggerResponse);

        private:
            QString m_serialNumber;
            quint32 m_triggerResponse;
    };
}

Q_DECLARE_METATYPE(QProfile::QAbstractResponse)

#endif /* QABSTRACTRESPONSE_H */
