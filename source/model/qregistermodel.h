/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QREGISTERMODEL_H
#define QREGISTERMODEL_H

#include <QObject>
#include <QSharedPointer>
#include <QByteArray>
#include "qprofile_debug.h"
#include "qprofile_global.h"
#include "qstringattribute.h"
#include "qabstractmodel.h"
#include "qipattribute.h"
#include "qnumericattribute.h"
#include "qrservermodel.h"
#include "qadminsettingproperties.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QRserver
{
    class QRegisterModel: public QProfile::QAbstractModel
    {
            Q_OBJECT

            Q_PROPERTY(QString techId READ techId WRITE setTechId)
            Q_PROPERTY(QString techName READ techName WRITE setTechName)
            Q_PROPERTY(QString company READ company WRITE setCompany)
            Q_PROPERTY(QString supervisorId READ supervisorId WRITE setSupervisorId)
            Q_PROPERTY(QString serverUrl READ serverUrl WRITE setServerUrl)
            Q_PROPERTY(QString serverIp READ serverIp WRITE setServerIp)
            Q_PROPERTY(quint16 serverPort READ serverPort WRITE setServerPort)
            Q_PROPERTY(QString networkDevice READ networkDevice WRITE setNetworkDevice)
            Q_PROPERTY(QString region READ region WRITE setRegion)
            Q_PROPERTY(QString district READ district WRITE setDistrict)
            Q_PROPERTY(QString system READ system WRITE setSystem)
            Q_PROPERTY(QString serverName READ serverName WRITE setServerName)
            Q_PROPERTY(quint32 orgChartStatus READ orgChartStatus WRITE setOrgChartStatus)
            Q_PROPERTY(QString apiVersion READ apiVersion WRITE setApiVersion)
            Q_PROPERTY(QVariant adminSettingProfile READ adminSettingProfile WRITE
                       setAdminSettingProfile)

        public:
            static const QString CommandRegister;
            static const QString CommandUnregister;
            static const QString CommandCheck;
            static const QString CommandUpdate;
            static const QString CommandUpdateTechName;

            static const quint32 RegisterSingleDeviceTimeout;
            static const quint32 UnregisterSingleDeviceTimeout;
            static const quint32 CheckSingleDeviceTimeout;
            static const quint32 UpdateDeviceInfoTimeout;
            static const quint32 GetServerVersionTimeout;
            static const quint32 UpdateTechNameTimeout;
            static const QString ApiVersion;
            static const QString InvalidIpAddress;
            static const QString AdminSettingProfileProperty;

            enum OrgChartStatus { Disable = 0, Enable = 1 };

        signals:
            void serverIpChanged();

        public:
            QRegisterModel(QObject *parent, const QByteArray &content);
            QRegisterModel(QObject *parent = nullptr);
            QRegisterModel(const QRegisterModel &model);
            void initial();

            QString techId() const;
            void setTechId(QString techId);

            QString techName() const;
            void setTechName(QString techName);

            QString company() const;
            void setCompany(QString company);

            QString supervisorId() const;
            void setSupervisorId(QString supervisorId);

            QString serverUrl() const;
            void setServerUrl(QString serverUrl);

            QString serverIp() const;
            void setServerIp(QString serverIp);

            QString apiVersion() const;
            void setApiVersion(QString apiVersion);

            quint16 serverPort() const;
            void setServerPort(quint16 port);

            QString networkDevice() const;
            void setNetworkDevice(QString networkDevice);

            QString region() const;
            void setRegion(QString region);

            QString district() const;
            void setDistrict(QString district);

            QString system() const;
            void setSystem(QString system);

            QString serverName() const;
            void setServerName(QString serverName);

            quint32 orgChartStatus() const;
            void setOrgChartStatus(quint32 status);

            const QVariant adminSettingProfile() const;
            void setAdminSettingProfile(const QVariant adminSettingProfileVar);

            const QRserverModel &rserverModel() const
            {
                return *m_rserverModel;
            }

        public slots:
            virtual void initAttributes();

        private:
            QSharedPointer<QProfile::QStringAttribute> 		m_techId;
            QSharedPointer<QProfile::QStringAttribute> 		m_techName;
            QSharedPointer<QProfile::QStringAttribute> 		m_company;
            QSharedPointer<QProfile::QStringAttribute> 		m_supervisorId;
            QSharedPointer<QProfile::QStringAttribute> 		m_serverUrl;
            QSharedPointer<QProfile::QIpAttribute> 			m_serverIp;
            QSharedPointer<QProfile::QNumericAttribute> 	m_serverPort;
            QSharedPointer<QProfile::QStringAttribute> 		m_networkDevice;
            QSharedPointer<QProfile::QStringAttribute> 		m_region;
            QSharedPointer<QProfile::QStringAttribute> 		m_district;
            QSharedPointer<QProfile::QStringAttribute> 		m_system;
            QSharedPointer<const QRserverModel>				m_rserverModel;
            QSharedPointer<QProfile::QStringAttribute> 		m_serverName;
            QSharedPointer<QProfile::QNumericAttribute> 	m_orgChartStatus;
            QString										 	m_apiVersion;
            QAdminSettingProfileProperties					m_adminSettingProfileProperties;
    };
}

Q_DECLARE_METATYPE(QRserver::QRegisterModel)

#endif
