
#ifndef QGETRESULTSTATUS_H
#define QGETRESULTSTATUS_H

#include "quploadstatus.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QRserver
{
    class QGetResultStatus : public QUploadStatus
    {
            Q_OBJECT

            Q_PROPERTY(QVariant vConnectResultModel READ vConnectResultModel WRITE
                       setVConnectResultModel)

        public:
            QGetResultStatus(QObject *parent = nullptr);
            QGetResultStatus(const QVariant &statusVar);
            QGetResultStatus(QObject *parent, const QString &id,
                             const quint32 commandStatus = QProfile::QAbstractStatus::Failed);
            ~QGetResultStatus();

            const QVariant vConnectResultModel() const;
            void setVConnectResultModel(const QVariant &vConnectResultModel);

        private:
            QVariant m_vConnectResultModel;
    };
}

Q_DECLARE_METATYPE(QRserver::QGetResultStatus)

#endif // QGETRESULTSTATUS_H
