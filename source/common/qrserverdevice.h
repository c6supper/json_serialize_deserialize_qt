/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QRSERVERDEVICE_H
#define QRSERVERDEVICE_H

#include <QMutexLocker>
#include "qrserverdevicetype.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QRserver
{
    class QRserverDevice: public QObject
    {
            Q_OBJECT

            Q_PROPERTY(QVariantList deviceTypeList READ deviceTypeList WRITE
                       setDeviceTypeListList)

        public:
            QRserverDevice(QObject *parent = nullptr);
            ~QRserverDevice();

            const QVariantList deviceTypeList();
            void setDeviceTypeListList(const QVariantList &deviceTypeList);

            static const QRserverDeviceType *DeviceTypeEnum2RserverDeviceType(
                const QString deviceTypeEnum);

            static const QRserverDeviceType *SerialNumber2RserverDeviceType(
                const QString serialNumber);

        public:
            static const QString DefaultRserverDeviceConfig;


        private:
            static QRserverDevice				*instance();
            const QRserverDeviceType			*getDeviceType(const QString deviceTypeEnum);
            const QRserverDeviceType			*getDeviceTypeBySN(const QString serialNumber);

        private:
            static QRserverDevice				*m_instance;
            QMutex								m_mutex;
            QList<QRserverDeviceType *>			m_deviceTypeList;
    };
}


#endif /* QRSERVERDEVICE_H */
