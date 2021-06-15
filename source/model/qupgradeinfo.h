/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QUPGRADEINFO_H
#define QUPGRADEINFO_H

#include "qdeviceinfo.h"
#include "qnumericattribute.h"
#include "qstringattribute.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QRserver
{
    class QUpgradeInfo: public QDeviceInfo
    {
            Q_OBJECT

            Q_PROPERTY(QByteArray softwareMd5 READ softwareMd5 WRITE setSoftwareMd5)
            Q_PROPERTY(qint32 imageSize READ imageSize WRITE setImageSize)
            Q_PROPERTY(qint32 downloadedSize READ downloadedSize WRITE setDownloadedSize)
            Q_PROPERTY(qint32 upgradeStatus READ upgradeStatus WRITE setUpgradeStatus)
            Q_PROPERTY(QString downloadedPercentage READ downloadedPercentage WRITE
                       setDownloadedPercentage)

        public:
            enum UpgradeStatus {Idle = 0xFF,
                                FineToUpgrade = 0,
                                VersionCantFind,
                                VersionIsNewest,
                                TooMuchConnection,
                                SaveFileFailed,
                                MD5Mismatch,
                                DownloadFileSuccess,
                                UnknowError,
                                NotEnoughSpace,
                                Timeout,
                                StartFailed,
                                RemoteFileNotExists,
                                ModuleVersionEmpty,
                                CreatePathFailed,
                                SaveDirectoryNotExists,
                                ExceedCapOfPacSize
                               };

        public:
            QUpgradeInfo(QObject *parent = nullptr);

            const QByteArray softwareMd5() const;
            void setSoftwareMd5(const QByteArray softwareMd5);

            quint32 imageSize() const;
            void setImageSize(quint32 imageSize);

            quint32 downloadedSize() const;
            void setDownloadedSize(quint32 downloadedSize);

            qint32 upgradeStatus() const;
            void setUpgradeStatus(qint32 upgradeStatus);

            const QString downloadedPercentage() const;
            void setDownloadedPercentage(const QString downloadedPercentage);

            const QString getDeltaImageName();

        private:
            void initAttributes();

        private:
            QByteArray 									m_softwareMd5;
            QSharedPointer<QProfile::QNumericAttribute> m_imageSize;
            QSharedPointer<QProfile::QNumericAttribute> m_downloadedSize;
            QSharedPointer<QProfile::QNumericAttribute> m_upgradeStatus;
            QSharedPointer<QProfile::QStringAttribute>  m_downloadedPercentage;
    };
}
Q_DECLARE_METATYPE(QRserver::QUpgradeInfo)
#endif
