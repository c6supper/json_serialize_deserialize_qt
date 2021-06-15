/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qmessagefactory.h"
#include "qmsgupdateversioninfo.h"
#include "qrserverdevicetype.h"
#include "qdeviceinfo.h"

using namespace QRserver;
using namespace QProfile;
using namespace rserver;

QMessageSelfRegisteration<QMsgUpdateVersionInfo> registerQMsgUpdateVersionInfo(
    rserver::UpdateVersionInfo);

QDataStream &QMsgUpdateVersionInfo::read(QDataStream &in)
{
    return in;
}

const QByteArray QMsgUpdateVersionInfo::toByteArray() const
{
    st_UpdateVersionInfo message;
    bzero(&message, sizeof(message));
    QByteArray messageArray((const char *) &message, sizeof(message));
    QDataStream out(&messageArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_6);
    out << *this;
    return messageArray;
}

QDataStream &QMsgUpdateVersionInfo::write(QDataStream &out) const
{
    st_UpdateVersionInfo stMessage;
    bzero(&stMessage, sizeof(stMessage));
    m_serialNumber->toChar(stMessage.serialNum, sizeof(stMessage.serialNum));
    QMutexLocker locker(m_mutex);
    QList<QDeviceInfo *>::const_iterator i;
    QString versions("[");

    for (i = m_deviceList.constBegin(); i != m_deviceList.constEnd(); ++i) {
        qint32 deviceType = QRserverDeviceType::String2DeviceType((
                                *i)->deviceTypeEnum());

        if (-1 == deviceType) {
            qprofileDebug(QtWarningMsg) << "Invalid device " << (*i)->toJson();
            continue;
        }

        versions += QString("{\"%1\":\"%2\"}").arg(deviceType).arg((
                        *i)->version());//*NOPAD*

        if (i != m_deviceList.constEnd() - 1) {
            versions += ",";
        }
    }

    versions += "]";
    qprofileDebug(QtDebugMsg) << "Update version to " << versions;
    QStringAttribute versionString(versions);
    versionString.toChar(stMessage.version, sizeof(stMessage.version));
    out.writeRawData((const char *) &stMessage, sizeof(stMessage));
    return out;
}

QMsgUpdateVersionInfo::QMsgUpdateVersionInfo(QObject *parent) :
    QAbstractBinMsg(rserver::UpdateVersionInfo, parent), m_mutex(new QMutex())
{
    initAttributes();
}

QMsgUpdateVersionInfo::QMsgUpdateVersionInfo(const QMsgUpdateVersionInfo
        &message) :
    QAbstractBinMsg(message), m_mutex(new QMutex())
{
    initAttributes();
}

void QMsgUpdateVersionInfo::initAttributes()
{
}

QMsgUpdateVersionInfo::~QMsgUpdateVersionInfo()
{
    m_mutex->lock();

    while (!m_deviceList.isEmpty())
        delete m_deviceList.takeFirst();

    m_mutex->unlock();
    delete m_mutex;
}

const QVariantList QMsgUpdateVersionInfo::deviceList() const
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

void QMsgUpdateVersionInfo::setDeviceList(const QVariantList deviceList)
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

        // make sure platform version shown at the beginning
        if (device->isChassis()) {
            m_deviceList.prepend(device);
        } else {
            m_deviceList.append(device);
        }
    }
}

