/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */


#include "qrservermodel.h"
#include "qobjectfactory.h"

using namespace QProfile;
using namespace QRserver;

QObjectSelfRegisteration<QRserverModel> registerQRserverModel;

const QString QRserverModel::DefaultRserverDeviceInfoConfig = "devicelist.json";
const QString QRserverModel::CommandPing = "ping";
const QString QRserverModel::PingTopic = "rserver\\qrserver\\command";
const QString QRserverModel::PongTopic = "rserver\\qrserver\\status";

QRserverModel::QRserverModel(QObject *parent) : QAbstractModel(parent),
    m_mutex(new QMutex())
{
    Q_ASSERT(parent != nullptr);
}

bool QRserverModel::isChassis(const QString serialNumber) const
{
    QMutexLocker locker(m_mutex);
    QList<QDeviceInfo *>::const_iterator i;

    for (i = m_deviceList.constBegin(); i != m_deviceList.constEnd(); ++i) {
        if ((*i)->serialNumber() == serialNumber) {
            return (*i)->isChassis();
        }
    }

    return false;
}

const QVariant QRserverModel::getChassis() const
{
    QMutexLocker locker(m_mutex);
    QList<QDeviceInfo *>::const_iterator i;

    for (i = m_deviceList.constBegin(); i != m_deviceList.constEnd(); ++i) {
        if ((*i)->isChassis()) {
            return (*i)->toVariant();
        }
    }

    return QVariant();
}

QRserverModel::QRserverModel(const QRserverModel &model)
    : QAbstractModel(model), m_mutex(new QMutex())
{
}

void QRserverModel::initAttributes()
{
}

/*bool QRserverModel::haveDeviceStatus(qint32 status) const
{
    QMutexLocker locker(m_mutex);
    QList<QDeviceInfo *>::const_iterator i;

    for (i = m_deviceList.constBegin(); i != m_deviceList.constEnd(); ++i) {
        if ((*i)->deviceStatus() == status) {
            return true;
        }
    }

    return false;
}

bool QRserverModel::isDeviceStatusEqual(qint32 status) const
{
    QMutexLocker locker(m_mutex);
    QList<QDeviceInfo *>::const_iterator i;

    for (i = m_deviceList.constBegin(); i != m_deviceList.constEnd(); ++i) {
        if ((*i)->deviceStatus() != status) {
            return false;
        }
    }

    return (m_deviceList.count() > 0);
}*/

void QRserverModel::initialDeviceStatus()
{
    QMutexLocker locker(m_mutex);
    QList<QDeviceInfo *>::iterator i;

    for (i = m_deviceList.begin(); i != m_deviceList.end(); ++i) {
        (*i)->blockNotify(true);
        (*i)->setDeviceStatus(QDeviceInfo::Unknown);
        (*i)->setRemoteVersion("");
        (*i)->blockNotify(false);
    }

    asynPropertyChangeNotify();
}

void QRserverModel::updateRemoteVersion(const qint32 deviceType,
                                        const QString remoteVersion)
{
    QMutexLocker locker(m_mutex);
    QList<QDeviceInfo *>::iterator i;

    for (i = m_deviceList.begin(); i != m_deviceList.end(); ++i) {
        if ((*i)->deviceType()  == (quint32)deviceType) {
            (*i)->blockNotify(true);
            (*i)->setRemoteVersion(remoteVersion);
            (*i)->blockNotify(false);
            asynPropertyChangeNotify();
            break;
        }
    }
}

void QRserverModel::updateRemoteVersion(const QString serialNumber,
                                        const QString remoteVersion)
{
    QMutexLocker locker(m_mutex);
    QList<QDeviceInfo *>::iterator i;

    for (i = m_deviceList.begin(); i != m_deviceList.end(); ++i) {
        if ((*i)->serialNumber()  == serialNumber) {
            (*i)->blockNotify(true);
            (*i)->setRemoteVersion(remoteVersion);
            (*i)->blockNotify(false);
            asynPropertyChangeNotify();
            break;
        }
    }
}

void QRserverModel::updateDeviceStatus(const QString serialNumber,
                                       const qint32 deviceStatus)
{
    QMutexLocker locker(m_mutex);
    QList<QDeviceInfo *>::iterator i;

    for (i = m_deviceList.begin(); i != m_deviceList.end(); ++i) {
        if ((*i)->serialNumber() == serialNumber) {
            (*i)->blockNotify(true);
            (*i)->setDeviceStatus((QDeviceInfo::DeviceStatus)deviceStatus);
            (*i)->blockNotify(false);
            asynPropertyChangeNotify();
            break;
        }
    }
}

void QRserverModel::updateTestsetStatus(const QString serialNumber,
                                        const qint32 testsetStatus)
{
    QMutexLocker locker(m_mutex);
    QList<QDeviceInfo *>::iterator i;

    for (i = m_deviceList.begin(); i != m_deviceList.end(); ++i) {
        if ((*i)->serialNumber() == serialNumber) {
            (*i)->blockNotify(true);
            (*i)->setTestsetStatus((QDeviceInfo::TestsetStatus)testsetStatus);
            (*i)->blockNotify(false);
            asynPropertyChangeNotify();
            break;
        }
    }
}

const QVariantList QRserverModel::deviceList() const
{
    QMutexLocker locker(m_mutex);
    QVariantList deviceList;
    QList<QDeviceInfo *>::const_iterator i;

    for (i = m_deviceList.constBegin(); i != m_deviceList.constEnd(); ++i) {
        QVariant deviceVar = (*i)->toVariant();
        deviceList.append(deviceVar);
    }

    return deviceList;
}

void QRserverModel::setDeviceList(const QVariantList deviceList)
{
    if (this->deviceList() == deviceList) {
        qprofileDebug(QtDebugMsg) << "device is the same";
        return;
    }

    QMutexLocker locker(m_mutex);

    while (!m_deviceList.isEmpty())
        delete m_deviceList.takeFirst();

    QList<QVariant>::const_iterator i;

    for (i = deviceList.constBegin(); i != deviceList.constEnd(); ++i) {
        QDeviceInfo *device = new QDeviceInfo(this);
        device->fromVariant((*i).toMap());
        m_deviceList.append(device);
    }
}

bool QRserverModel::supportProfile() const
{
    QMutexLocker locker(m_mutex);
    bool supportProfile = false;
    QList<QDeviceInfo *>::const_iterator i;

    for (i = m_deviceList.begin(); i != m_deviceList.end(); ++i) {
        if ((*i)->supportProfile()) {
            supportProfile = true;
            break;
        }
    }

    return supportProfile;
}

QDeviceInfo::DeviceStatus QRserverModel::GetDeviceStatus(
    const QRserverModel &rserverModel)
{
    QDeviceInfo::DeviceStatus status = QDeviceInfo::Unregistered;
    QDeviceInfo chassis;
    chassis.fromVariant(rserverModel.getChassis());
    status = chassis.deviceStatus();
    return status;
}

QDeviceInfo::TestsetStatus QRserverModel::GetTestsetStatus(
    const QRserverModel &rserverModel)
{
    QDeviceInfo::TestsetStatus status = QDeviceInfo::TestsetStatusUnknown;
    QDeviceInfo chassis;
    chassis.fromVariant(rserverModel.getChassis());
    status = chassis.testsetStatus();
    return status;
}

QDeviceInfo::Style QRserverModel::getStyle() const
{
    QDeviceInfo::Style style = QDeviceInfo::DefaultStyle;
    QDeviceInfo chassis;
    chassis.fromVariant(getChassis());
    style = chassis.style();
    return style;
}

const qint32 QRserverModel::deviceCount() const
{
    return m_deviceList.count();
}

bool QRserverModel::canReleaseAppManually() const
{
    QMutexLocker locker(m_mutex);
    bool canReleaseAppManually = true;
    QList<QDeviceInfo *>::const_iterator i;

    for (i = m_deviceList.begin(); i != m_deviceList.end(); ++i) {
        if (!(*i)->canReleaseAppManually()) {
            canReleaseAppManually = false;
            break;
        }
    }

    return canReleaseAppManually;
}

QRserverModel::~QRserverModel()
{
    m_mutex->lock();

    while (!m_deviceList.isEmpty())
        delete m_deviceList.takeFirst();

    m_mutex->unlock();
    delete m_mutex;
}
