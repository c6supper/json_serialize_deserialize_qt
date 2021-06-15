/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QDEVICEINFO_H
#define QDEVICEINFO_H

#include <QObject>
#include <QSharedPointer>
#include "qprofile_debug.h"
#include "qprofile_global.h"
#include "qstringattribute.h"
#include "qmacattribute.h"
#include "qnumericattribute.h"
#include "qabstractmodel.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QRserver
{
    class QDeviceInfo: public QProfile::QAbstractModel
    {
            Q_OBJECT
            Q_ENUMS(DeviceStatus)
            Q_ENUMS(TestsetStatus)
            Q_ENUMS(Style)

            Q_PROPERTY(quint32 deviceType READ deviceType WRITE setDeviceType)
            Q_PROPERTY(QString deviceTypeEnum READ deviceTypeEnum WRITE setDeviceTypeEnum)
            Q_PROPERTY(QString deviceName READ deviceName WRITE setDeviceName)
            Q_PROPERTY(quint32 imageType READ imageType WRITE setImageType)
            Q_PROPERTY(QString serialNumber READ serialNumber WRITE setSerialNumber)
            Q_PROPERTY(QString mac READ mac WRITE setMac)
            Q_PROPERTY(QString cableMac READ cableMac WRITE setCableMac)
            Q_PROPERTY(QString version READ version WRITE setVersion)
            Q_PROPERTY(QString remoteVersion READ remoteVersion WRITE setRemoteVersion)
            Q_PROPERTY(DeviceStatus deviceStatus READ deviceStatus WRITE setDeviceStatus)
            Q_PROPERTY(TestsetStatus testsetStatus READ testsetStatus WRITE
                       setTestsetStatus)
            Q_PROPERTY(QStringList extraOptionModuleList READ extraOptionModuleList WRITE
                       setExtraOptionModuleList)

        public:
            enum DeviceStatus {Unregistered = 0,  RegisteredAuthorized, RegisteredUnauthorized,
                               RegisteredRMA, Unknown, RegisterSucceed, RegisterFailed, UnregisterSucceed, UnregisterFailed
                              };
            enum TestsetStatus {TestsetStatusUnknown = 0, Approved, Revoked, Stolen, Loaned,
                                Inventory = 100, PreApprove, Missing, Repair, Broken, Calibration, Retired, Sold
                               };
            enum Style {
                DefaultStyle = 0, FX150LStyle
            };
            static const QString PropertyDeviceName;
            static const QString PropertyDeviceType;
            static const QString PropertyImageType;
            static const QString PropertyVersion;
            static const QString PropertyRemoteVersion;
            static const QString PropertySerialNumber;
            static const QString PropertyDeviceTypeEnum;
            static const QStringList PropertyList;
            static const qint32 SerialNuberLengthLimitation;

        public:
            QDeviceInfo(QObject *parent = nullptr);
            static const QStringList propertyList();
            static QVariant getQDeviceInfo(const QString &mac, const QString &sn,
                                           const QString &version, quint32 deviceType = 0);
            static bool IsValidSerialNumber(const QString &serialNumber);

            QString version() const;
            void setVersion(QString version);

            QString remoteVersion() const;
            void setRemoteVersion(QString remoteVersion);

            QString serialNumber() const;
            void setSerialNumber(QString serialNumber);

            QString deviceTypeEnum() const;
            void setDeviceTypeEnum(QString deviceTypeEnum);

            QString deviceName() const;
            void setDeviceName(QString deviceName);

            QString mac() const;
            void setMac(QString mac);

            QString cableMac() const;
            void setCableMac(QString cableMac);

            quint32 deviceType() const;
            void setDeviceType(quint32 deviceType);

            quint32 imageType() const;
            void setImageType(quint32 imageType);

            DeviceStatus deviceStatus() const;
            void setDeviceStatus(DeviceStatus deviceStatus);

            TestsetStatus testsetStatus() const;
            void setTestsetStatus(TestsetStatus testsetStatus);

            QStringList extraOptionModuleList() const;
            void setExtraOptionModuleList(QStringList list);

            bool isChassis();
            bool supportProfile();
            bool canReleaseAppManually();
            bool canAssignOption();
            Style style();

        private:
            void initAttributes();

        private:
            QSharedPointer<QProfile::QStringAttribute> 		m_version;
            QSharedPointer<QProfile::QStringAttribute> 		m_remoteVersion;
            QSharedPointer<QProfile::QStringAttribute> 		m_serialNumber;
            QSharedPointer<QProfile::QMacAttribute> 		m_mac;
            QSharedPointer<QProfile::QMacAttribute> 		m_cableMac;
            QSharedPointer<QProfile::QNumericAttribute> 	m_deviceType;
            QSharedPointer<QProfile::QNumericAttribute> 	m_imageType;
            QSharedPointer<QProfile::QNumericAttribute> 	m_deviceStatus;
            QSharedPointer<QProfile::QStringAttribute> 		m_deviceName;
            QString									 		m_deviceTypeEnum;
            QSharedPointer<QProfile::QNumericAttribute> 	m_testsetStatus;
            QStringList m_extraOptionModuleList;
    };
}
Q_DECLARE_METATYPE(QRserver::QDeviceInfo)
#endif
