#ifndef QMSGUPDATEVCONNECTINFO_H
#define QMSGUPDATEVCONNECTINFO_H

#include <QMetaType>
#include "qabstractbinmsg.h"
#include "qstringattribute.h"
#include "qmacattribute.h"
#include "qnumericattribute.h"

namespace QRserver
{
    class QMsgUpdateVConnectInfo : public QAbstractBinMsg
    {
            Q_OBJECT

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
            QMsgUpdateVConnectInfo(QObject *parent = nullptr);
            QMsgUpdateVConnectInfo(const QMsgUpdateVConnectInfo &message);
            virtual ~QMsgUpdateVConnectInfo();

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

        protected:
            virtual QDataStream &read(QDataStream &in) ;
            virtual QDataStream &write(QDataStream &out) const;
            virtual const QByteArray toByteArray() const;
            virtual void initAttributes();

        protected:
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
    };
}

Q_DECLARE_METATYPE(QRserver::QMsgUpdateVConnectInfo)

#endif /* QMSGUPDATEVCONNECTINFO_H */

