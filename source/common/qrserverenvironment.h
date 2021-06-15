/* This file is part of rserver
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QRSERVERENVIRONMENT_H
#define QRSERVERENVIRONMENT_H

#include "qrserverconfig.h"
#include "qabstractenvironment.h"
#include <QFileSystemWatcher>

namespace QRserver
{
    class QRserverEnvironment : public QProfile::QAbstractEnvironment
    {
            Q_OBJECT

            Q_PROPERTY(QString rserverHome READ rserverHome WRITE setRserverHome)
            Q_PROPERTY(QString profileConfig READ profileConfig WRITE setProfileConfig)
            Q_PROPERTY(QString vConncetServerConfig READ vConncetServerConfig)
            Q_PROPERTY(QString vConncetClientConfig READ vConncetClientConfig)
            Q_PROPERTY(QString deviceRelatedProfileConfig READ deviceRelatedProfileConfig
                       WRITE setDeviceRelatedProfileConfig)
            Q_PROPERTY(QString resultConfig READ resultConfig WRITE setResultConfig)
            Q_PROPERTY(QString remoteExecutorCMDConfig READ remoteExecutorCMDConfig WRITE
                       setRemoteExecutorCMDConfig)
            Q_PROPERTY(QString storageRoot READ storageRoot WRITE setStorageRoot)
            Q_PROPERTY(QString storageRserver READ storageRserver WRITE setStorageRserver)
            Q_PROPERTY(QString storageImage READ storageImage WRITE setStorageImage)
            Q_PROPERTY(QString deviceInfo READ deviceInfo WRITE setDeviceInfo)

        public:
            explicit QRserverEnvironment(QObject *parent = nullptr);
            virtual ~QRserverEnvironment();

            const QString rserverHome() const;
            void setRserverHome(const QString rserverHome);

            const QString profileConfig() const;
            void setProfileConfig(const QString profileConfig);

            const QString vConncetServerConfig() const;
            const QString vConncetClientConfig() const;

            const QString deviceRelatedProfileConfig() const;
            void setDeviceRelatedProfileConfig(const QString deviceRelatedProfileConfig);

            const QString resultConfig() const;
            void setResultConfig(const QString resultConfig);

            const QString remoteExecutorCMDConfig() const;
            void setRemoteExecutorCMDConfig(const QString remoteExecutorCMDConfig);

            const QString deviceConfig() const;
            void setDeviceConfig(const QString deviceConfig);

            const QString storageRoot() const;
            void setStorageRoot(const QString storageRoot);

            const QString storageRserver() const;
            void setStorageRserver(const QString storageRserver);

            const QString storageImage() const;
            void setStorageImage(const QString storageImage);

            const QString deviceInfo() const;
            void setDeviceInfo(const QString deviceInfo);

            virtual const QString version() const;
            virtual void setVersion(const QString version);

            virtual const QString lastProfile();
            virtual const QString defaultProfile();

            virtual void initialize();

            static const QString ResultConfig();
            static const QString RemoteExecutorCMDConfig();
            static const QString ProfileConfig();
            static const QString VConncetServerConfig();
            static const QString VConncetClientConfig();
            static const QString DeviceRelatedProfileConfig();
            static const QString StorageRoot();
            static const QString StorageRserver();
            static const QString DeviceInfo();
            static const QString DeviceConfig();
            static const QString StorageImage();
            static const qint32 LogLevel();
            static bool TouchRserverFile(const QString baseName);
            static bool TouchTmpFile(const QString baseName);
            static const QString ProfileDecryptTool();
            static const QString DefaultProfile();
            static const QString LastProfile();
            static const QString Version();
#ifdef BROADCAST_PROFILE_DOWNLOAD
            static const QString DownloadedProfileMark();
#endif

        protected:
            virtual void printEnvironmentVarList();
            virtual void checkEnvironment();
            virtual const QStringList ignoredProperties() const;

        private:
            static QRserverEnvironment *instance();
            static bool TouchFile(const QString absoluteFilePath);
            const QString rserverTmp();

        private slots:
            void fileChanged(const QString &path);
            void versionChanged();

        private:
            QString 						m_rserverHome;
            QString							m_profileConfig;
            QString							m_deviceRelatedProfileConfig;
            QString							m_vConncetServerConfig;
            QString							m_vConncetClientConfig;
            QString							m_resultConfig;
            QString							m_remoteExecutorCMDConfig;
            QString							m_storageRoot;
            QString							m_storageRserver;
            QString							m_deviceInfo;
            QString							m_deviceConfig;
            QString							m_storageImage;
            const QString					m_defaultProfile;
            const QString					m_lastProfile;
            QFileSystemWatcher				m_fileSystemWatcher;
            static const QString			defaultRserverHome;
            static const QString			defaultStoreRoot;
            static const QString			defaultStoreRserver;
            static const QString			defaultTmp;
            static const QString			defaultStoreImage;
            static const QString			defaultRserverEnvProfile;
            static const QString			defaultRserverTmp;
    };
}
#endif // QRSERVERENVIRONMENT_H
