#ifndef QREGISTERSTATUS_H
#define QREGISTERSTATUS_H

#include "qabstractstatus.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QRserver
{
    class QRegisterStatus : public QProfile::QAbstractStatus
    {
            Q_OBJECT

            Q_ENUMS(RegisterStatus)
            Q_PROPERTY(QVariant rserverModelVar READ rserverModelVar WRITE
                       setRserverModelVar)
            Q_PROPERTY(QVariant registerModelVar READ registerModelVar WRITE
                       setRegisterModelVar)
            Q_PROPERTY(QString compatibilityClassName READ compatibilityClassName WRITE
                       setCompatibilityClassName)

        public:
            enum RegisterStatus {UptadeTechNameFailed = QProfile::QAbstractStatus::StatusButtom, TestsetBeRevoked, TestsetBeStolen,};

        public:
            QRegisterStatus(QObject *parent = nullptr);
            QRegisterStatus(const QVariant &statusVar);
            QRegisterStatus(QObject *parent, const QString &id,
                            const quint32 commandStatus = QProfile::QAbstractStatus::Failed);
            ~QRegisterStatus();

            const QVariant rserverModelVar();
            void setRserverModelVar(const QVariant rserverModelVar);

            const QVariant registerModelVar();
            void setRegisterModelVar(const QVariant registerModelVar);

            const QString compatibilityClassName() const;
            void setCompatibilityClassName(const QString compatibilityClassName);

        private:
            QVariant m_rserverModelVar;
            QVariant m_registerModelVar;
            QString  m_compatibilityClassName;
    };
}

Q_DECLARE_METATYPE(QRserver::QRegisterStatus)

#endif // QREGISTERSTATUS_H
