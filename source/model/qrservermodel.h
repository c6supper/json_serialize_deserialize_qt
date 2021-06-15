/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QRSERVERMODEL_H
#define QRSERVERMODEL_H

#include <QObject>
#include <QVariant>
#include <QList>
#include <QMutexLocker>
#include "qprofile_debug.h"
#include "qprofile_global.h"
#include "qdeviceinfo.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QRserver
{
    class QRserverModel: public QProfile::QAbstractModel
    {
            Q_OBJECT

            Q_PROPERTY(QVariantList deviceList READ deviceList WRITE setDeviceList)

        public:
            QRserverModel(QObject *parent = nullptr);
            ~QRserverModel();
            QRserverModel(const QRserverModel &model);

            const QVariantList deviceList() const;
            void setDeviceList(const QVariantList deviceList);
            const QVariant getChassis() const;
            void updateDeviceStatus(const QString serialNumber, const qint32 deviceStatus);
            void updateTestsetStatus(const QString serialNumber,
                                     const qint32 testsetStatus);
            void updateRemoteVersion(const QString serialNumber,
                                     const QString remoteVersion);
            void updateRemoteVersion(const qint32 deviceType, const QString remoteVersion);
            bool supportProfile() const;
            bool canReleaseAppManually() const;
            QDeviceInfo::Style getStyle() const;
            static QDeviceInfo::DeviceStatus GetDeviceStatus(const QRserverModel
                    &rserverModel);
            static QDeviceInfo::TestsetStatus GetTestsetStatus(const QRserverModel
                    &rserverModel);
            const qint32 deviceCount() const;
            bool isChassis(const QString serialNumber) const;

        public:
            static const QString DefaultRserverDeviceInfoConfig;
            static const QString CommandPing;
            static const QString PingTopic;
            static const QString PongTopic;
            static const int PingTimeout = 5 * 1000;

        public slots:
            void initialDeviceStatus();

        private:
            void initAttributes();

        private:
            QList<QDeviceInfo *>				m_deviceList;
            QMutex								*m_mutex;
    };
}

Q_DECLARE_METATYPE(QRserver::QRserverModel)

#endif
