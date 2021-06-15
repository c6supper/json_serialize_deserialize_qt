/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include <QMetaEnum>
#include <QCoreApplication>
#include "qrserverdevicetype.h"

using namespace QRserver;

const QString QRserverDeviceType::PropertyDeviceTypeTagName = "deviceType";
const QString QRserverDeviceType::PropertyImageTypeTagName = "imageType";
const QString QRserverDeviceType::PropertyDeviceNameTagName = "deviceName";
const QString QRserverDeviceType::PropertyUncompressDirTagName =
    "uncompressDir";
const QString QRserverDeviceType::PropertyTargetDirTagName = "targetDir";
const QString QRserverDeviceType::PropertyRegExpSNTagName = "regExpSN";
const QString QRserverDeviceType::PropertyIsChassisTagName = "isChassis";
const QString QRserverDeviceType::PropertySupportProfileTagName =
    "supportProfile";
const QString QRserverDeviceType::PropertyCanReleaseAppManuallyTagName =
    "canReleaseAppManually";
const QString QRserverDeviceType::PropertyCanAssignOptionTagName =
    "canAssignOption";
const QString QRserverDeviceType::PropertyExtraOptionModuleListTagName =
    "extraOptionModuleList";
const QString QRserverDeviceType::PropertyStyleTagName = "style";

QRserverDeviceType::QRserverDeviceType(QObject *parent)
    : QProfile::QAbstractJsonElement(parent)
{
}

QRserverDeviceType::~QRserverDeviceType()
{
}

quint32 QRserverDeviceType::deviceType() const
{
    return m_deviceType;
}
void QRserverDeviceType::setDeviceType(quint32 deviceType)
{
    m_deviceType = deviceType;
}

qint32 QRserverDeviceType::imageType() const
{
    return m_imageType;
}
void QRserverDeviceType::setImageType(qint32 imageType)
{
    m_imageType = imageType;
}

QString QRserverDeviceType::deviceName() const
{
    return m_deviceName;
}
void QRserverDeviceType::setDeviceName(QString deviceName)
{
    m_deviceName = deviceName;
}

QString QRserverDeviceType::uncompressDir() const
{
    return m_uncompressDir;
}
void QRserverDeviceType::setUncompressDir(QString uncompressDir)
{
    m_uncompressDir = uncompressDir;
}

QString QRserverDeviceType::targetDir() const
{
    return m_targetDir;
}
void QRserverDeviceType::setTargetDir(QString targetDir)
{
    m_targetDir = targetDir;
}

QString QRserverDeviceType::regExpSN() const
{
    return m_regExpSN;
}
void QRserverDeviceType::setRegExpSN(QString regExpSN)
{
    m_regExpSN = regExpSN;
}

bool QRserverDeviceType::isChassis() const
{
    return m_isChassis;
}
void QRserverDeviceType::setIsChassis(bool b)
{
    m_isChassis = b;
}

bool QRserverDeviceType::supportProfile() const
{
    return m_supportProfile;
}
void QRserverDeviceType::setSupportProfile(bool b)
{
    m_supportProfile = b;
}

bool QRserverDeviceType::canReleaseAppManually() const
{
    return m_canReleaseAppManually;
}
void QRserverDeviceType::setCanReleaseAppManually(bool b)
{
    m_canReleaseAppManually = b;
}

bool QRserverDeviceType::canAssignOption() const
{
    return m_canAssignOption;
}
void QRserverDeviceType::setCanAssignOption(bool b)
{
    m_canAssignOption = b;
}

QStringList QRserverDeviceType::extraOptionModuleList() const
{
    return m_extraOptionModuleList;
}
void QRserverDeviceType::setExtraOptionModuleList(QStringList list)
{
    m_extraOptionModuleList = list;
}

quint32 QRserverDeviceType::style() const
{
    return m_style;
}
void QRserverDeviceType::setStyle(quint32 style)
{
    m_style = style;
}

bool QRserverDeviceType::isValid()
{
    return (m_deviceType > 0 && m_imageType > 0 && !m_deviceName.isEmpty());
}

const QString QRserverDeviceType::ImageType2String(const qint32 imageType)
{
    qint32 idx =
        QRserverDeviceType::staticMetaObject.indexOfEnumerator("ImageType");
    QMetaEnum metaEnum = QRserverDeviceType::staticMetaObject.enumerator(idx);
    return metaEnum.valueToKey((DeviceType)imageType);
}
const qint32 QRserverDeviceType::String2ImageType(const QString imageTypeString)
{
    qint32 idx =
        QRserverDeviceType::staticMetaObject.indexOfEnumerator("ImageType");
    QMetaEnum metaEnum = QRserverDeviceType::staticMetaObject.enumerator(idx);
    return metaEnum.keyToValue(imageTypeString.toLatin1().data());
}
const QString QRserverDeviceType::DeviceType2String(const quint32 deviceType)
{
    qint32 idx =
        QRserverDeviceType::staticMetaObject.indexOfEnumerator("DeviceType");
    QMetaEnum metaEnum = QRserverDeviceType::staticMetaObject.enumerator(idx);
    return metaEnum.valueToKey((DeviceType)deviceType);
}
const quint32 QRserverDeviceType::String2DeviceType(const QString
        deviceTypeString)
{
    qint32 idx =
        QRserverDeviceType::staticMetaObject.indexOfEnumerator("DeviceType");
    QMetaEnum metaEnum = QRserverDeviceType::staticMetaObject.enumerator(idx);
    return metaEnum.keyToValue(deviceTypeString.toLatin1().data());
}
const quint32 QRserverDeviceType::String2Style(const QString styleString)
{
    qint32 idx =
        QRserverDeviceType::staticMetaObject.indexOfEnumerator("Style");
    QMetaEnum metaEnum = QRserverDeviceType::staticMetaObject.enumerator(idx);
    return metaEnum.keyToValue(styleString.toLatin1().data());
}

