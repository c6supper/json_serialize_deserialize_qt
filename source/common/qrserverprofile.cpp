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
#include "qrserverprofile.h"
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

QRserverProfile *QRserverProfile::m_instance = nullptr;
const QString QRserverProfile::DefaultRserverProfileConfig =
    "rserverprofile.json";
const QString QRserverProfile::DeviceRelatedProfileConfig =
    "devicerelatedprofile.json";
const QString QRserverProfile::DefaultResultSignaturePath =
    "/root/v300-rslt/sinaturepad";

QRserverProfile::QRserverProfile(QObject *parent)
    : QRserverFile(parent)
{
}

QRserverProfile::~QRserverProfile()
{
    QMutexLocker locker(&m_mutex);

    while (!m_deviceRealtedProfileList.isEmpty())
        delete m_deviceRealtedProfileList.takeFirst();
}

QVariantList QRserverProfile::deviceRealtedProfileList()
{
    QMutexLocker locker(&m_mutex);
    QVariantList deviceRealtedProfileList;
    QList<QDeviceRelatedFileType *>::const_iterator i;

    for (i = m_deviceRealtedProfileList.constBegin();
         i != m_deviceRealtedProfileList.constEnd(); ++i) {
        deviceRealtedProfileList.append((*i)->toVariant());
    }

    return deviceRealtedProfileList;
}
void QRserverProfile::setDeviceRealtedProfileList(QVariantList
        &deviceRealtedProfileList)
{
    QMutexLocker locker(&m_mutex);

    while (!m_deviceRealtedProfileList.isEmpty())
        delete m_deviceRealtedProfileList.takeFirst();

    QVariantList::const_iterator iter;

    for (iter = deviceRealtedProfileList.constBegin();
         iter != deviceRealtedProfileList.constEnd();
         ++iter) {
        QDeviceRelatedFileType *deviceRelatedFileType = new QDeviceRelatedFileType(
            this);
        deviceRelatedFileType->fromVariant(*iter);
        const QRserverFileType *rserverFileType = getRserverFileType(
                    deviceRelatedFileType->localPath());

        if (rserverFileType == nullptr) {
            qprofileDebug(QtWarningMsg) << "Unkown device related profile " <<
                                        deviceRelatedFileType->toVariant();
            continue;
        }

        deviceRelatedFileType->fromVariant(rserverFileType->toVariant());
#ifdef BUILD_X86
        QString localPath = deviceRelatedFileType->localPath();
        localPath = QString("%1%2").arg(QCoreApplication::applicationDirPath()).arg(
                        localPath);
        deviceRelatedFileType->setLocalPath(localPath);
#endif
        m_deviceRealtedProfileList.append(deviceRelatedFileType);
    }
}

const QString QRserverProfile::resultSignaturePath() const
{
    return m_resultSignaturePath;
}

void QRserverProfile::setResultSignaturePath(QString resultSignaturePath)
{
    m_resultSignaturePath = resultSignaturePath;
}

const QString QRserverProfile::type2String(const qint32 type)
{
    qint32 idx = metaObject()->indexOfEnumerator("ProfileType");
    QMetaEnum metaEnum = metaObject()->enumerator(idx);
    return metaEnum.valueToKey((ProfileType)type);
}

const qint32 QRserverProfile::stringToType(const QString typeString)
{
    qint32 idx = metaObject()->indexOfEnumerator("ProfileType");
    QMetaEnum metaEnum = metaObject()->enumerator(idx);
    return metaEnum.keyToValue(typeString.toLatin1().data());
}

const QVariant QRserverProfile::GetFileType(const qint32 type)
{
    return instance()->getFileType(type);
}

const QVariant QRserverProfile::GetFileType(const QString &filePath,
        const bool enableUseDirPath)
{
    return instance()->getFileType(filePath, enableUseDirPath);
}

const QString QRserverProfile::getDisplayName(const QString &filePath,
        const QString measureMode, const QString measureType)
{
    return QString();
}

const QVariant QRserverProfile::GetDeviceRelatedFileType(
    const QString &filePath)
{
    return instance()->getDeviceRelatedFileType(filePath);
}

const QVariantList QRserverProfile::GetDeviceRelatedFileType(const qint32 type)
{
    return instance()->getDeviceRelatedFileType(type);
}

const QString QRserverProfile::getResultSignaturePath()
{
    return instance()->resultSignaturePath();
}

const QStringList QRserverProfile::GetDeviceRelatedProfilePathList()
{
    return instance()->getDeviceRelatedProfilePathList();
}

bool QRserverProfile::isKey(const QString &filePath, const QString measureMode,
                            const QString measureType)
{
    Q_UNUSED(measureMode);
    Q_UNUSED(measureType);
    QFileInfo fileInfo(filePath);
    return (fileInfo.isFile());
}

const QVariant QRserverProfile::getDeviceRelatedFileType(
    const QString &localPath)
{
    QMutexLocker locker(&m_mutex);
    QList<QDeviceRelatedFileType *>::const_iterator i;

    for (i = m_deviceRealtedProfileList.constBegin();
         i != m_deviceRealtedProfileList.constEnd(); ++i) {
        QDir localPath(localPath);
        QDir profileLocalPath((*i)->localPath());

        if (localPath == profileLocalPath) {
            return (*i)->toVariant();
        }
    }

    return QVariant();
}

const QVariantList QRserverProfile::getDeviceRelatedFileType(
    const qint32 type)
{
    QMutexLocker locker(&m_mutex);
    QVariantList list;
    QList<QDeviceRelatedFileType *>::const_iterator i;

    for (i = m_deviceRealtedProfileList.constBegin();
         i != m_deviceRealtedProfileList.constEnd(); ++i) {
        if (type == stringToType((*i)->fileType())) {
            list.append((*i)->toVariant());
        }
    }

    return list;
}

const QStringList QRserverProfile::getDeviceRelatedProfilePathList()
{
    QMutexLocker locker(&m_mutex);
    QStringList list;
    QList<QDeviceRelatedFileType *>::const_iterator i;

    for (i = m_deviceRealtedProfileList.constBegin();
         i != m_deviceRealtedProfileList.constEnd(); ++i) {
        list << (*i)->localPath();
    }

    return list;
}

void QRserverProfile::loadDeviceRelatedProfileConfig()
{
    QFile file(QRserverEnvironment::DeviceRelatedProfileConfig());

    if (!file.exists()) {
        qprofileDebug(QtWarningMsg) << QRserverEnvironment::DeviceRelatedProfileConfig()
                                    << " is missing!";
    }

    if (file.open(QIODevice::ReadOnly)) {
        QByteArray data = file.readAll();
        QJson::Parser parser;
        bool ok = false;
        QVariant variant = parser.parse(data, &ok);

        if (!ok) {
            qprofileDebug(QtFatalMsg) << "Failed to load rserverprofile.json!";
        }

        QJson::QObjectHelper::qvariant2qobject(variant.toMap(), instance());
        file.close();
    }

    if (instance()->resultSignaturePath().isEmpty()
        || instance()->resultSignaturePath().isNull()) {
        instance()->setResultSignaturePath(DefaultResultSignaturePath);
    }
}

QRserverProfile *QRserverProfile::instance()
{
    if (m_instance == nullptr) {
        m_instance = new QRserverProfile(QCoreApplication::instance());
        QFile file(QRserverEnvironment::ProfileConfig());

        if (!file.exists()) {
            qprofileDebug(QtFatalMsg) << QRserverEnvironment::ProfileConfig() <<
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

        m_instance->loadDeviceRelatedProfileConfig();
    }

    return m_instance;
}
