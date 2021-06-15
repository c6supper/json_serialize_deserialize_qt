/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */


#include "qdeviceinfo.h"
#include "qobjectfactory.h"
#include "qrserverdevicetype.h"
#include "qrserverdevice.h"
#include <QMetaProperty>
#include "qadminsettingprofile.h"

using namespace QProfile;
using namespace QRserver;

QObjectSelfRegisteration<QRserver::QDeviceInfo> registerQDeviceInfo;

const QString QDeviceInfo::PropertyDeviceName = "deviceName";
const QString QDeviceInfo::PropertyDeviceType = "deviceType";
const QString QDeviceInfo::PropertyImageType = "imageType";
const QString QDeviceInfo::PropertyVersion = "version";
const QString QDeviceInfo::PropertyRemoteVersion = "remoteVersion";
const QString QDeviceInfo::PropertySerialNumber = "serialNumber";
const QString QDeviceInfo::PropertyDeviceTypeEnum = "deviceTypeEnum";
const QStringList QDeviceInfo::PropertyList = propertyList();
const qint32 QDeviceInfo::SerialNuberLengthLimitation = 14;

const QStringList QDeviceInfo::propertyList()
{
    QStringList list;

    for (int propertyIdx = 0;
         propertyIdx < QDeviceInfo::staticMetaObject.propertyCount(); propertyIdx++) {
        QMetaProperty property = QDeviceInfo::staticMetaObject.property(propertyIdx);
        list.append(property.name());
    }

    list.removeOne("objectName");
    list.removeOne("command");
    list.removeOne("trigger");
    return list;
}

QDeviceInfo::QDeviceInfo(QObject *parent) : QAbstractModel(parent),
    m_deviceTypeEnum("")
{
    Q_ASSERT(parent != nullptr);
    m_version = QSharedPointer<QStringAttribute>(new QStringAttribute(this));
    m_remoteVersion = QSharedPointer<QStringAttribute>(new QStringAttribute(this));
    m_serialNumber = QSharedPointer<QStringAttribute>(new QStringAttribute(this));
    m_mac = QSharedPointer<QMacAttribute>(new QMacAttribute(this));
    m_cableMac = QSharedPointer<QMacAttribute>(new QMacAttribute(this));
    m_deviceType = QSharedPointer<QNumericAttribute>(new QNumericAttribute(0,
                   QVariant::UInt, this));
    m_imageType = QSharedPointer<QNumericAttribute>(new QNumericAttribute(0,
                  QVariant::Int, this));
    m_deviceStatus	= QSharedPointer<QNumericAttribute>(new QNumericAttribute((
                          qint32)QDeviceInfo::Unknown,
                      QVariant::Int, this));
    m_deviceName = QSharedPointer<QStringAttribute>(new QStringAttribute(this));
    m_testsetStatus	= QSharedPointer<QNumericAttribute>(new QNumericAttribute((
                          qint32)TestsetStatusUnknown, QVariant::Int, this));
}

void QDeviceInfo::initAttributes()
{
}

bool QDeviceInfo::isChassis()
{
    const QRserverDeviceType *rserverDeviceType =
        QRserverDevice::DeviceTypeEnum2RserverDeviceType(deviceTypeEnum());

    if (rserverDeviceType != nullptr) {
        return rserverDeviceType->isChassis();
    } else {
        qprofileDebug(QtWarningMsg) << "<isChassis>Unsupported device " <<
                                    deviceTypeEnum();
        return false;
    }
}

bool QDeviceInfo::supportProfile()
{
    const QRserverDeviceType *rserverDeviceType =
        QRserverDevice::DeviceTypeEnum2RserverDeviceType(deviceTypeEnum());

    if (rserverDeviceType != nullptr) {
        return rserverDeviceType->supportProfile();
    } else {
        qprofileDebug(QtWarningMsg) << "<supportProfile>Unsupported device " <<
                                    deviceTypeEnum();
        return false;
    }
}

bool QDeviceInfo::canReleaseAppManually()
{
    const QRserverDeviceType *rserverDeviceType =
        QRserverDevice::DeviceTypeEnum2RserverDeviceType(deviceTypeEnum());

    if (rserverDeviceType != nullptr) {
        return rserverDeviceType->canReleaseAppManually();
    } else {
        qprofileDebug(QtWarningMsg) << "<canReleaseAppManually>Unsupported device " <<
                                    deviceTypeEnum();
        return false;
    }
}

bool QDeviceInfo::canAssignOption()
{
    const QRserverDeviceType *rserverDeviceType =
        QRserverDevice::DeviceTypeEnum2RserverDeviceType(deviceTypeEnum());

    if (rserverDeviceType != nullptr) {
        return rserverDeviceType->canAssignOption();
    } else {
        qprofileDebug(QtWarningMsg) << "<canAssignOption>Unsupported device " <<
                                    deviceTypeEnum();
        return false;
    }
}

QDeviceInfo::Style QDeviceInfo::style()
{
    const QRserverDeviceType *rserverDeviceType =
        QRserverDevice::DeviceTypeEnum2RserverDeviceType(deviceTypeEnum());

    if (rserverDeviceType != nullptr) {
        return (QDeviceInfo::Style)rserverDeviceType->style();
    } else {
        qprofileDebug(QtWarningMsg) << "<style>Unsupported device " <<
                                    deviceTypeEnum();
        return DefaultStyle;
    }
}

QString QDeviceInfo::version() const
{
    return m_version->value().toString();
}
void QDeviceInfo::setVersion(QString version)
{
    m_version->setValue(version);
}

QString QDeviceInfo::remoteVersion() const
{
    return m_remoteVersion->value().toString();
}

void QDeviceInfo::setRemoteVersion(QString remoteVersion)
{
    m_remoteVersion->setValue(remoteVersion);
}

QString QDeviceInfo::serialNumber() const
{
    return m_serialNumber->value().toString();
}
void QDeviceInfo::setSerialNumber(QString serialNumber)
{
    m_serialNumber->setValue(serialNumber);

    if (deviceType() != 0) {
        return;
    }

    const QRserverDeviceType *rserverDeviceType =
        QRserverDevice::SerialNumber2RserverDeviceType(serialNumber);

    if (rserverDeviceType != nullptr) {
        setDeviceType(rserverDeviceType->deviceType());
    } else {
        qprofileDebug(QtWarningMsg) << "Unsupported device " << serialNumber;
    }
}

QString QDeviceInfo::mac() const
{
    return m_mac->value().toString();
}
void QDeviceInfo::setMac(QString mac)
{
    m_mac->setValue(mac);
}

QString QDeviceInfo::cableMac() const
{
    return m_cableMac->value().toString();
}
void QDeviceInfo::setCableMac(QString cableMac)
{
    m_cableMac->setValue(cableMac);
}

quint32 QDeviceInfo::deviceType() const
{
    return m_deviceType->value().toUInt();
}
void QDeviceInfo::setDeviceType(quint32 deviceType)
{
    m_deviceType->setValue(deviceType);

    if (m_deviceTypeEnum == "") {
        m_deviceTypeEnum = QRserverDeviceType::DeviceType2String(deviceType);
        const QRserverDeviceType *rserverDeviceType =
            QRserverDevice::DeviceTypeEnum2RserverDeviceType(m_deviceTypeEnum);

        if (rserverDeviceType != nullptr) {
            setImageType(rserverDeviceType->imageType());
            setDeviceName(rserverDeviceType->deviceName());
            setExtraOptionModuleList(rserverDeviceType->extraOptionModuleList());
        } else {
            m_deviceTypeEnum = "";
            qprofileDebug(QtWarningMsg) << "Unsupported device " << deviceType;
        }
    }
}

quint32 QDeviceInfo::imageType() const
{
    return m_imageType->value().toUInt();
}
void QDeviceInfo::setImageType(quint32 imageType)
{
    m_imageType->setValue(imageType);
}

QString QDeviceInfo::deviceName() const
{
    return m_deviceName->value().toString();
}
void QDeviceInfo::setDeviceName(QString deviceName)
{
    m_deviceName->setValue(deviceName);
}

QString QDeviceInfo::deviceTypeEnum() const
{
    return m_deviceTypeEnum;
}
void QDeviceInfo::setDeviceTypeEnum(QString deviceTypeEnum)
{
    m_deviceTypeEnum = deviceTypeEnum;

    if (deviceType() == 0) {
        const QRserverDeviceType *rserverDeviceType =
            QRserverDevice::DeviceTypeEnum2RserverDeviceType(deviceTypeEnum);

        if (rserverDeviceType != nullptr) {
            setDeviceType(rserverDeviceType->deviceType());
            setImageType(rserverDeviceType->imageType());
            setDeviceName(rserverDeviceType->deviceName());
            setExtraOptionModuleList(rserverDeviceType->extraOptionModuleList());
        } else {
            qprofileDebug(QtWarningMsg) << "Unsupported device " << deviceTypeEnum;
        }
    }
}

QDeviceInfo::DeviceStatus QDeviceInfo::deviceStatus() const
{
    return (QDeviceInfo::DeviceStatus)m_deviceStatus->value().toInt();
}
void QDeviceInfo::setDeviceStatus(QDeviceInfo::DeviceStatus deviceStatus)
{
    m_deviceStatus->setValue((qint32)deviceStatus);
}

QDeviceInfo::TestsetStatus QDeviceInfo::testsetStatus() const
{
    return (QDeviceInfo::TestsetStatus)m_testsetStatus->value().toInt();
}
void QDeviceInfo::setTestsetStatus(QDeviceInfo::TestsetStatus testsetStatus)
{
    m_testsetStatus->setValue((qint32)testsetStatus);
}

QStringList QDeviceInfo::extraOptionModuleList() const
{
    return m_extraOptionModuleList;
}
void QDeviceInfo::setExtraOptionModuleList(QStringList list)
{
    m_extraOptionModuleList = list;
}

QVariant QDeviceInfo::getQDeviceInfo(const QString &mac, const QString &sn,
                                     const QString &version, quint32 deviceType)
{
    if (mac.isEmpty() || mac.isNull() || sn.isEmpty() || sn.isNull()) {
        return QVariant();
    }

    QDeviceInfo deviceInfo;
    deviceInfo.setMac(mac.toUpper());
    deviceInfo.setVersion(version);
    deviceInfo.setDeviceType(deviceType);
    deviceInfo.setSerialNumber(sn.toUpper().left(SerialNuberLengthLimitation));

    if (deviceInfo.isChassis() && (version.isEmpty() || version.isNull())) {
        reserverLog("Device version error!");
        reserverLog(QString("The serial number is : %1").arg(sn));
        reserverLog(QString("The version is : %1").arg(version));
        reserverLog(QString("The device type is : %1").arg(deviceType));
        return QVariant();
    }

    if (!IsValidSerialNumber(deviceInfo.serialNumber())) {
        reserverLog("Serial number error!");
        reserverLog(QString("The serial number is : %1").arg(sn));
        reserverLog(QString("The version is : %1").arg(version));
        reserverLog(QString("The device type is : %1").arg(deviceType));
        return QVariant();
    }

    return deviceInfo.toVariant();
}

bool QDeviceInfo::IsValidSerialNumber(const QString &serialNumber)
{
    return (nullptr != QRserverDevice::SerialNumber2RserverDeviceType(
                serialNumber));
}
