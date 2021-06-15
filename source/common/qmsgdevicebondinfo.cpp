#include "qmessagefactory.h"
#include "qmsgdevicebondinfo.h"

using namespace QRserver;
using namespace QProfile;
using namespace rserver;

QMessageSelfRegisteration<QMsgDeviceBondInfo> registerQMsgDeviceBondInfo(
    eDeviceBondInfo);

QMsgDeviceBondInfo::QMsgDeviceBondInfo(QObject *parent)
    : QAbstractBinMsg(eDeviceBondInfo, parent), m_mutex(new QMutex())
{
}

QMsgDeviceBondInfo::QMsgDeviceBondInfo(const QMsgDeviceBondInfo
                                       &message)
    : QAbstractBinMsg(message), m_mutex(new QMutex())
{
}

QMsgDeviceBondInfo::~QMsgDeviceBondInfo()
{
    m_mutex->lock();

    while (!m_deviceList.isEmpty())
        delete m_deviceList.takeFirst();

    m_mutex->unlock();
    delete m_mutex;
}

const QVariantList QMsgDeviceBondInfo::deviceList() const
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

void QMsgDeviceBondInfo::setDeviceList(const QVariantList deviceList)
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

        if (!device->canAssignOption()) {
            continue;
        }

        // make sure platform version shown at the beginning
        if (device->isChassis()) {
            m_deviceList.prepend(device);
        } else {
            m_deviceList.append(device);
        }
    }
}

qint32 QMsgDeviceBondInfo::moduleCanAssignOptionCount() const
{
    return (m_deviceList.count() - 1);
}

QDataStream &QMsgDeviceBondInfo::read(QDataStream &in)
{
    return in;
}

QDataStream &QMsgDeviceBondInfo::write(QDataStream &out) const
{
    QMutexLocker locker(m_mutex);
    QList<QDeviceInfo *>::const_iterator i;
    QStringAttribute devSn;

    for (i = m_deviceList.constBegin(); i != m_deviceList.constEnd(); ++i) {
        if ((*i)->isChassis()) {
            st_PlatformSnInfo stPlatSnInfo;
            bzero(&stPlatSnInfo, sizeof(st_PlatformSnInfo));
            stPlatSnInfo.type = (*i)->deviceType();
            devSn.setValue(QVariant((*i)->serialNumber()));
            devSn.toChar(stPlatSnInfo.platformSn, sizeof(stPlatSnInfo.platformSn));
            stPlatSnInfo.moduleCount = moduleCanAssignOptionCount();
            out.writeRawData((const char *)&stPlatSnInfo, sizeof(stPlatSnInfo));
        } else {
            st_ModuleSnInfo stModuleSnInfo;
            bzero(&stModuleSnInfo, sizeof(st_ModuleSnInfo));
            stModuleSnInfo.type = (*i)->deviceType();
            devSn.setValue(QVariant((*i)->serialNumber()));
            devSn.toChar(stModuleSnInfo.moduleSn, sizeof(stModuleSnInfo.moduleSn));
            out.writeRawData((const char *)&stModuleSnInfo, sizeof(stModuleSnInfo));
        }
    }

    return out;
}

const QByteArray QMsgDeviceBondInfo::toByteArray() const
{
    st_PlatformSnInfo stPlatSnInfo;
    bzero(&stPlatSnInfo, sizeof(st_PlatformSnInfo));
    QByteArray messageArray((const char *)&stPlatSnInfo,
                            sizeof(stPlatSnInfo));
    st_ModuleSnInfo stModuleSnInfo;
    bzero(&stModuleSnInfo, sizeof(st_ModuleSnInfo));
    qint32 count = moduleCanAssignOptionCount();

    while (count-- > 0) {
        messageArray.append((const char *)&stModuleSnInfo,
                            sizeof(stModuleSnInfo));
    }

    QDataStream out(&messageArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_6);
    out << *this;
    return messageArray;
}

