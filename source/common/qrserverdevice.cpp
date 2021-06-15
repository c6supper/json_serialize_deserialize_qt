/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include <QtGlobal>
#include <QMetaEnum>
#include <QCoreApplication>
#include <QFile>
#include <QFileInfo>
#include "qprofile_debug.h"
#include "qrserverdevice.h"
#include "qrserverenvironment.h"
#include <QDir>

#if defined(BUILD_X86)
#include "qjson/parser.h"
#include "qjson/qobjecthelper.h"
#else
#include <QJson/Parser>
#include <QJson/QObjectHelper>
#endif

using namespace QRserver;

QRserverDevice *QRserverDevice::m_instance = nullptr;
const QString QRserverDevice::DefaultRserverDeviceConfig = "rserverdevice.json";

QRserverDevice::QRserverDevice(QObject *parent)
    : QObject(parent)
{
}

QRserverDevice::~QRserverDevice()
{
    QMutexLocker locker(&m_mutex);

    while (!m_deviceTypeList.isEmpty())
        delete m_deviceTypeList.takeFirst();
}

const QVariantList QRserverDevice::deviceTypeList()
{
    QMutexLocker locker(&m_mutex);
    QVariantList deviceTypeList;
    QList<QRserverDeviceType *>::const_iterator i;

    for (i = m_deviceTypeList.constBegin();
         i != m_deviceTypeList.constEnd(); ++i) {
        deviceTypeList.append((*i)->toVariant());
    }

    return deviceTypeList;
}
void QRserverDevice::setDeviceTypeListList(const QVariantList &deviceTypeList)
{
    if (deviceTypeList == this->deviceTypeList()) {
        return;
    }

    QMutexLocker locker(&m_mutex);

    while (!m_deviceTypeList.isEmpty())
        delete m_deviceTypeList.takeFirst();

    QVariantList::const_iterator iter;

    for (iter = deviceTypeList.constBegin(); iter != deviceTypeList.constEnd();
         ++iter) {
        QRserverDeviceType *deviceType = new QRserverDeviceType(this);
        QVariantMap deviceTypeMap = (*iter).toMap();

        if (deviceTypeMap.count() > 0) {
            quint32 deviceTypeUInt32 =
                QRserverDeviceType::String2DeviceType(
                    deviceTypeMap[QRserverDeviceType::PropertyDeviceTypeTagName].toString());
            qint32 imageType =
                QRserverDeviceType::String2ImageType(
                    deviceTypeMap[QRserverDeviceType::PropertyImageTypeTagName].toString());
            deviceType->setDeviceName(
                deviceTypeMap[QRserverDeviceType::PropertyDeviceNameTagName].toString());
            deviceType->setDeviceType(deviceTypeUInt32);
            deviceType->setImageType(imageType);
            deviceType->setUncompressDir(
                deviceTypeMap[QRserverDeviceType::PropertyUncompressDirTagName].toString());
            deviceType->setTargetDir(
                deviceTypeMap[QRserverDeviceType::PropertyTargetDirTagName].toString());
            deviceType->setRegExpSN(
                deviceTypeMap[QRserverDeviceType::PropertyRegExpSNTagName].toString());
            deviceType->setIsChassis(
                deviceTypeMap[QRserverDeviceType::PropertyIsChassisTagName].toBool());
            deviceType->setSupportProfile(
                deviceTypeMap[QRserverDeviceType::PropertySupportProfileTagName].toBool());
            deviceType->setCanReleaseAppManually(
                deviceTypeMap[QRserverDeviceType::PropertyCanReleaseAppManuallyTagName].toBool());
            deviceType->setCanAssignOption(
                deviceTypeMap[QRserverDeviceType::PropertyCanAssignOptionTagName].toBool());
            deviceType->setExtraOptionModuleList(
                deviceTypeMap[QRserverDeviceType::PropertyExtraOptionModuleListTagName].toStringList());

            if (!deviceTypeMap.contains(QRserverDeviceType::PropertyStyleTagName)) {
                deviceType->setStyle(QRserverDeviceType::eDefaultStyle);
            } else {
                quint32 style = QRserverDeviceType::String2Style(
                                    deviceTypeMap[QRserverDeviceType::PropertyStyleTagName].toString());
                deviceType->setStyle(style);
            }

            if (deviceType->isValid()) {
                m_deviceTypeList.append(deviceType);
                continue;
            }
        }

        qprofileDebug(QtWarningMsg) << "got a invalid deviceType " << (*iter);
        delete deviceType;
    }
}

const QRserverDeviceType *QRserverDevice::getDeviceType(
    const QString deviceTypeEnum)
{
    QMutexLocker locker(&m_mutex);
    QList<QRserverDeviceType *>::const_iterator i;

    for (i = m_deviceTypeList.constBegin();
         i != m_deviceTypeList.constEnd(); ++i) {
        if ((*i)->deviceType() == QRserverDeviceType::String2DeviceType(
                deviceTypeEnum)) {
            return (*i);
        }
    }

    return nullptr;
}

const QRserverDeviceType *QRserverDevice::getDeviceTypeBySN(
    const QString serialNumber)
{
    QMutexLocker locker(&m_mutex);
    QList<QRserverDeviceType *>::const_iterator i;

    for (i = m_deviceTypeList.constBegin();
         i != m_deviceTypeList.constEnd(); ++i) {
        QRegExp exp((*i)->regExpSN());

        if (exp.exactMatch(serialNumber)) {
            return (*i);
        }
    }

    return nullptr;
}

const QRserverDeviceType *QRserverDevice::DeviceTypeEnum2RserverDeviceType(
    const QString deviceTypeEnum)
{
    return instance()->getDeviceType(deviceTypeEnum);
}

const QRserverDeviceType *QRserverDevice::SerialNumber2RserverDeviceType(
    const QString serialNumber)
{
    return instance()->getDeviceTypeBySN(serialNumber);
}

QRserverDevice *QRserverDevice::instance()
{
    if (m_instance == nullptr) {
        m_instance = new QRserverDevice(QCoreApplication::instance());
        QFile file(QRserverEnvironment::DeviceConfig());

        if (!file.exists()) {
            qprofileDebug(QtFatalMsg) << QRserverEnvironment::DeviceConfig() <<
                                      " is missing! ";
        }

        if (file.open(QIODevice::ReadOnly)) {
            QByteArray data = file.readAll();
            QJson::Parser parser;
            bool ok = false;
            QVariant variant = parser.parse(data, &ok);

            if (!ok) {
                qprofileDebug(QtFatalMsg) << "Failed to load rserverprofile.json!";
            }

            QJson::QObjectHelper::qvariant2qobject(variant.toMap(), m_instance);
            file.close();
        }
    }

    return m_instance;
}
