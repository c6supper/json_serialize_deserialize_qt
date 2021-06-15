
#ifndef QVCONNECTMODEL_H
#define QVCONNECTMODEL_H

#include <QObject>
#include <QSharedPointer>
#include <QByteArray>
#include "qprofile_debug.h"
#include "qprofile_global.h"
#include "qstringattribute.h"
#include "qipattribute.h"
#include "qmacattribute.h"
#include "qnumericattribute.h"
#include "qregistermodel.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QRserver
{
    class QVConnectModel: public QProfile::QAbstractModel
    {
            Q_OBJECT
            Q_ENUMS(RServerMode)

            Q_PROPERTY(RServerMode rserverMode READ rserverMode WRITE setRserverMode)
            Q_PROPERTY(QString vconnectServerIp READ vconnectServerIp WRITE
                       setVconnectServerIp)
            Q_PROPERTY(QString serverUrl READ serverUrl WRITE setServerUrl)
            Q_PROPERTY(QString techId READ techId WRITE setTechId)
            Q_PROPERTY(QString techName READ techName WRITE setTechName)
            Q_PROPERTY(QString company READ company WRITE setCompany)
            Q_PROPERTY(QString supervisorId READ supervisorId WRITE setSupervisorId)
            Q_PROPERTY(quint32 deviceType READ deviceType WRITE setDeviceType)
            Q_PROPERTY(QString serialNumber READ serialNumber WRITE setSerialNumber)
            Q_PROPERTY(QString mac READ mac WRITE setMac)
            Q_PROPERTY(QString cableMac READ cableMac WRITE setCableMac)
            Q_PROPERTY(QString version READ version WRITE setVersion)
            Q_PROPERTY(QString deviceName READ deviceName WRITE setDeviceName)

        public:
            enum RServerMode {ModeRServer = 0,  ModeVConnect};
            static const QString DefaultVConnectServerConfig;
            static const QString DefaultVConnectClientConfig;
            static const QString CommandUpdateVConnectInfo;
            static const quint32 UpdateVConnectInfoTimeout;

        public:
            QVConnectModel(QObject *parent, const QByteArray &content);
            QVConnectModel(QObject *parent = nullptr);
            QVConnectModel(const QVConnectModel &model);

            void initial();
            bool isVConnectValid();

            RServerMode rserverMode() const;
            void setRserverMode(RServerMode rserverMode);

            QString vconnectServerIp() const;
            void setVconnectServerIp(QString vconnectServerIp);

            QString serverUrl() const;
            void setServerUrl(QString serverUrl);

            QString techId() const;
            void setTechId(QString techId);

            QString techName() const;
            void setTechName(QString techName);

            QString company() const;
            void setCompany(QString company);

            QString supervisorId() const;
            void setSupervisorId(QString supervisorId);

            quint32 deviceType() const;
            void setDeviceType(quint32 deviceType);

            QString serialNumber() const;
            void setSerialNumber(QString serialNumber);

            QString mac() const;
            void setMac(QString mac);

            QString cableMac() const;
            void setCableMac(QString cableMac);

            QString version() const;
            void setVersion(QString version);

            QString deviceName() const;
            void setDeviceName(QString deviceName);

            const QRegisterModel &registerModel() const
            {
                return *m_registerModel;
            }

        signals:
            void rserverModeChanged(qint32 mode);

        public slots:
            virtual void initAttributes();

        private:
            QSharedPointer<QProfile::QNumericAttribute> m_rserverMode;
            QSharedPointer<QProfile::QIpAttribute>        m_vconnectServerIp;
            QSharedPointer<QProfile::QStringAttribute>  m_serverUrl;
            QSharedPointer<QProfile::QStringAttribute>  m_techId;
            QSharedPointer<QProfile::QStringAttribute>  m_techName;
            QSharedPointer<QProfile::QStringAttribute>  m_company;
            QSharedPointer<QProfile::QStringAttribute>  m_supervisorId;
            QSharedPointer<QProfile::QNumericAttribute> m_deviceType;
            QSharedPointer<QProfile::QStringAttribute>      m_serialNumber;
            QSharedPointer<QProfile::QMacAttribute>         m_mac;
            QSharedPointer<QProfile::QMacAttribute>         m_cableMac;
            QSharedPointer<QProfile::QStringAttribute>      m_version;
            QSharedPointer<QProfile::QStringAttribute> 		m_deviceName;
            const QRegisterModel                                       *m_registerModel;
    };
}

Q_DECLARE_METATYPE(QRserver::QVConnectModel)

#endif
