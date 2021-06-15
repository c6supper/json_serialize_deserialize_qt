/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QSOFTWAREMODEL_H
#define QSOFTWAREMODEL_H

#include "qprofile_debug.h"
#include "qprofile_global.h"
#include "qregistermodel.h"
#include "qrservermodel.h"
#include "qabstractmodel.h"
#include "qnumericattribute.h"
#include "qupgradeinfo.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QRserver
{
    class QSoftwareModel: public QProfile::QAbstractModel
    {
            Q_OBJECT

            Q_PROPERTY(quint16 serverPort READ serverPort WRITE setServerPort)
            Q_PROPERTY(QVariantList deviceList READ deviceList)
            Q_PROPERTY(QVariantList upgradeInfoList READ upgradeInfoList WRITE
                       setUpgradeInfoList)

        public:
            static const QString CommandUpgrade;
            static const quint32 PackageTimeout;
            static const quint32 PropertyChangedTimeout;
            static const quint32 MaxPacSizeForOldUpgradingMethod;

        public:
            QSoftwareModel(QObject *parent = nullptr);
            ~QSoftwareModel();

            quint16 serverPort() const;
            void setServerPort(quint16 port);

            const QRserverModel &rserverModel() const
            {
                return *m_rserverModel;
            }

            const QRegisterModel &registerModel() const
            {
                return *m_registerModel;
            }

            const QVariantList deviceList() const;
            //            void setDeviceList(const QVariantList deviceList);

            const QVariantList upgradeInfoList() const;
            void setUpgradeInfoList(const QVariantList upgradeInfoList);
            void setUpgradeInfoMd5(const QString serialNumber, const QByteArray &md5);
            const QByteArray getUpgradeInfoMd5(const QString serialNumber);
            void setUpgradeInfoStatus(const QString serialNumber, const quint32 status);
            const quint32 getUpgradeInfoImageSize(const QString serialNumber);
            void setUpgradeInfoImageSize(const QString serialNumber, const quint32 size);
            const quint32 getUpgradeInfoDownloadedSize(const QString serialNumber);
            void setUpgradeInfoDownloadedSize(const QString serialNumber,
                                              const quint32 size);
            const QString getUpgradeInfoDeltaImageName(const QString serialNumber);
            const QString getUpgradeInfoDownloadedPercentage(const QString serialNumber);
            void setUpgradeInfoDownloadedPercentage(const QString serialNumber,
                                                    const QString downloadedPercentage);

        public slots:
            virtual void initAttributes();

        private:
            const QRserverModel 							*m_rserverModel;
            const QRegisterModel							*m_registerModel;
            QSharedPointer<QProfile::QNumericAttribute> 	m_serverPort;
            QVariantList									m_deviceList;
            QList<QUpgradeInfo *>							m_upgradeInfoList;
            QMutex											*m_mutex;
    };
}

Q_DECLARE_METATYPE(QRserver::QSoftwareModel)

#endif
