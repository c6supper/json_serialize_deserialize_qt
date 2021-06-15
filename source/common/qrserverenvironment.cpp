/* This file is part of qrserver
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QCoreApplication>
#include <QMetaProperty>
#include <QTimer>
#include "qprofile_debug.h"
#include "qrserverenvironment.h"
#include "qrserverversion.h"
#ifdef BUILD_X86
#include <unistd.h>
#endif

using namespace QRserver;
using namespace QProfile;

namespace QRserver
{
    class QRserverResult
    {
        public:
            static const QString DefaultRserverResultConfig;
    };
    class QRserverProfile
    {
        public:
            static const QString DefaultRserverProfileConfig;
            static const QString DeviceRelatedProfileConfig;
    };
    class QRserverModel
    {
        public:
            static const QString DefaultRserverDeviceInfoConfig;
    };
    class QRserverDevice
    {
        public:
            static const QString DefaultRserverDeviceConfig;
    };
    class QVConnectModel
    {
        public:
            static const QString DefaultVConnectServerConfig;
            static const QString DefaultVConnectClientConfig;
    };
#ifdef BROADCAST_PROFILE_DOWNLOAD
    class QDownloadFileModel
    {
        public:
            static const QString DownloadedProfileMark;
    };
#endif
}

namespace QProfile
{
    class QAbstractRemoteCommand
    {
        public:
            static const QString DefaultRemoteExecutorCMDProfileConfig;
    };
}

const QString QRserverEnvironment::defaultRserverHome = "rserver";
const QString QRserverEnvironment::defaultStoreRoot = "/home";
const QString QRserverEnvironment::defaultStoreRserver = "rserver";
const QString QRserverEnvironment::defaultTmp = QDir::temp().absolutePath();
const QString QRserverEnvironment::defaultStoreImage = "image";
const QString QRserverEnvironment::defaultRserverEnvProfile = "env";
const QString QRserverEnvironment::defaultRserverTmp = "/tmp/var/run/rserver";

QRserverEnvironment::QRserverEnvironment(QObject *parent): QAbstractEnvironment(
        parent), m_rserverHome(defaultRserverHome + "/"),
    m_profileConfig(QRserverProfile::DefaultRserverProfileConfig),
    m_deviceRelatedProfileConfig(QRserverProfile::DeviceRelatedProfileConfig),
    m_vConncetServerConfig(QVConnectModel::DefaultVConnectServerConfig),
    m_vConncetClientConfig(QVConnectModel::DefaultVConnectClientConfig),
    m_resultConfig(QRserverResult::DefaultRserverResultConfig),
    m_remoteExecutorCMDConfig(
        QAbstractRemoteCommand::DefaultRemoteExecutorCMDProfileConfig),
    m_storageRoot(defaultStoreRoot), m_storageRserver(defaultStoreRserver),
    m_deviceInfo(QRserverModel::DefaultRserverDeviceInfoConfig),
    m_deviceConfig(QRserverDevice::DefaultRserverDeviceConfig),
    m_storageImage(defaultStoreImage), m_defaultProfile("default"),
    m_lastProfile("lastprofile")
{
#ifdef BUILD_X86
    setStorageRoot(QCoreApplication::applicationDirPath() + defaultStoreRoot);
#endif
    setStorageRserver(m_storageRserver);
    setStorageImage(m_storageImage);
}

QRserverEnvironment *QRserverEnvironment::instance()
{
    if (m_instance == nullptr) {
        m_instance = new QRserverEnvironment(QCoreApplication::instance());
    }

    return dynamic_cast<QRserverEnvironment *>(m_instance);
}

const QString QRserverEnvironment::ResultConfig()
{
    return instance()->profileHome() + "/" + instance()->rserverHome() + "/" +
           instance()->resultConfig();
}

const QString QRserverEnvironment::RemoteExecutorCMDConfig()
{
    return instance()->profileHome() + "/" + instance()->rserverHome() +
           instance()->remoteExecutorCMDConfig();
}

const qint32 QRserverEnvironment::LogLevel()
{
    return instance()->logLevel();
}

void QRserverEnvironment::initialize()
{
    QFileInfo fileInfo(StorageRserver()  + "/" + defaultRserverEnvProfile);

    if (!fileInfo.exists()) {
        toFile(StorageRserver()  + "/" + defaultRserverEnvProfile);
    } else {
        fromFile(StorageRserver()  + "/" + defaultRserverEnvProfile);
    }

    QAbstractEnvironment::initialize();
    m_fileSystemWatcher.addPath(StorageRserver()  + "/" + defaultRserverEnvProfile);
    connect(&m_fileSystemWatcher, SIGNAL(fileChanged(const QString)), this,
            SLOT(fileChanged(const QString)));
}

void QRserverEnvironment::fileChanged(const QString &path)
{
    if (path == (StorageRserver()  + "/" + defaultRserverEnvProfile)) {
        fromFile(StorageRserver()  + "/" + defaultRserverEnvProfile);
    }
}

const QString QRserverEnvironment::lastProfile()
{
    return instance()->storageRoot() + instance()->storageRserver() +
           m_lastProfile;
}
const QString QRserverEnvironment::defaultProfile()
{
    return instance()->storageRoot() + instance()->storageRserver() +
           m_defaultProfile;
}

const QString QRserverEnvironment::StorageImage()
{
    return instance()->StorageRserver() + instance()->storageImage();
}

const QString QRserverEnvironment::DeviceConfig()
{
    return instance()->profileHome() + "/" + instance()->rserverHome() +
           instance()->deviceConfig();
}

const QString QRserverEnvironment::ProfileConfig()
{
    return instance()->profileHome() + "/" + instance()->rserverHome() +
           instance()->profileConfig();
}

const QString QRserverEnvironment::DeviceInfo()
{
#ifdef BUILD_X86
    return QString("%1/../%2").arg(
               QCoreApplication::applicationDirPath()).arg("testcase/rserverengine/qrserver/devicelist.json");
#else
    return instance()->storageRoot() + instance()->storageRserver() +
           instance()->deviceInfo();
#endif
}

const QString QRserverEnvironment::rserverTmp()
{
    QString path;
#ifdef BUILD_X86
    path = QString("%1/%2").arg(QCoreApplication::applicationDirPath()).arg(
               defaultRserverTmp);
#else
    path = defaultRserverTmp;
#endif
    QDir rservertmp(path);

    if (!rservertmp.exists()) {
        if (!rservertmp.mkpath(rservertmp.absolutePath())) {
            qprofileDebug(QtWarningMsg) << "Could not mk path " <<
                                        rservertmp.absolutePath();
        }
    }

    return path;
}

const QString QRserverEnvironment::VConncetServerConfig()
{
    return instance()->rserverTmp() + "/" + instance()->vConncetServerConfig();
}

const QString QRserverEnvironment::VConncetClientConfig()
{
    return instance()->rserverTmp() + "/" + instance()->vConncetClientConfig();
}

const QString QRserverEnvironment::DeviceRelatedProfileConfig()
{
    return instance()->profileHome() + "/" + instance()->rserverHome() +
           instance()->deviceRelatedProfileConfig();
}
const QString QRserverEnvironment::StorageRoot()
{
    return instance()->storageRoot();
}
const QString QRserverEnvironment::StorageRserver()
{
    return instance()->storageRoot() + instance()->storageRserver();
}
bool QRserverEnvironment::TouchRserverFile(const QString baseName)
{
    return TouchFile(StorageRserver() + "/" + baseName);
}

bool QRserverEnvironment::TouchTmpFile(const QString baseName)
{
    return TouchFile(defaultTmp + "/" + baseName);
}

void QRserverEnvironment::printEnvironmentVarList()
{
    qprofileDebug(QtWarningMsg) << "DeviceConfig" << DeviceConfig();
    qprofileDebug(QtWarningMsg) << "DeviceInfo" << DeviceInfo();
    qprofileDebug(QtWarningMsg) << "ResultConfig" << ResultConfig();
    qprofileDebug(QtWarningMsg) << "RemoteExecutorCMDConfig" <<
                                RemoteExecutorCMDConfig();
    qprofileDebug(QtWarningMsg) << "ProfileConfig" << ProfileConfig();
    qprofileDebug(QtWarningMsg) << "VConncetServerConfig" << VConncetServerConfig();
    qprofileDebug(QtWarningMsg) << "VConncetClientConfig" << VConncetClientConfig();
    qprofileDebug(QtWarningMsg) << "DeviceRelatedProfileConfig" <<
                                DeviceRelatedProfileConfig();
    qprofileDebug(QtWarningMsg) << "StorageRoot" << StorageRoot();
    qprofileDebug(QtWarningMsg) << "StorageRserver" << StorageRserver();
    qprofileDebug(QtWarningMsg) << "StorageImage" << StorageImage();
    return QAbstractEnvironment::printEnvironmentVarList();
}

const QString QRserverEnvironment::ProfileDecryptTool()
{
#ifdef BUILD_X86
    return QCoreApplication::applicationDirPath() +
           "/../thirdpart/linux/rsa/v300-rsadec";
#else
    return "/usr/local/bin/v300-rsadec";
#endif
}

#ifdef BROADCAST_PROFILE_DOWNLOAD
const QString QRserverEnvironment::DownloadedProfileMark()
{
    return QString("%1/%2").arg(instance()->rserverTmp()).arg(
               QDownloadFileModel::DownloadedProfileMark);
}
#endif

void QRserverEnvironment::checkEnvironment()
{
    QFileInfo fileInfo(QRserverEnvironment::ProfileDecryptTool());

    if (!fileInfo.exists()) {
        qprofileDebug(QtFatalMsg) << QRserverEnvironment::ProfileDecryptTool() <<
                                  " is missing!";
    }
}

bool QRserverEnvironment::TouchFile(const QString absoluteFilePath)
{
    QFile file(absoluteFilePath);

    if (file.exists()) {
        file.remove();
    }

#ifdef BUILD_X86
    bool ok = file.open(QIODevice::WriteOnly);
    file.close();

    while (!file.exists()) {
        usleep(200);
    }

    if (!file.setPermissions(QFile::ReadOwner | QFile::WriteOwner | QFile::ReadUser
                             | QFile::WriteUser | QFile::ReadOther | QFile::WriteOther | QFile::WriteGroup |
                             QFile::ReadGroup | QFile::ExeOwner | QFile::ExeUser | QFile::ExeGroup |
                             QFile::ExeOther)) {
        qprofileDebug(QtDebugMsg) << "Failed to set permission for file " <<
                                  absoluteFilePath;
    }

    return ok;
#else
    return file.open(QIODevice::WriteOnly);
#endif
}

QRserverEnvironment::~QRserverEnvironment()
{
    toFile(StorageRserver()  + "/" + defaultRserverEnvProfile);
}

const QString QRserverEnvironment::rserverHome() const
{
    return m_rserverHome;
}

void QRserverEnvironment::setRserverHome(const QString rserverHome)
{
    QDir rserverHomeDir(profileHome() + "/" + rserverHome);

    if (rserverHomeDir.isAbsolute()) {
        m_rserverHome = rserverHome;

        if (!rserverHomeDir.exists()) {
            if (!rserverHomeDir.mkpath(m_rserverHome)) {
                qprofileDebug(QtWarningMsg) << "Could not mk path " <<
                                            rserverHomeDir.absolutePath();
            }
        }
    } else {
        qprofileDebug(QtWarningMsg) << "Rserver Home path is not a relative path.";
    }
}

const QString QRserverEnvironment::deviceConfig() const
{
    return m_deviceConfig;
}
void QRserverEnvironment::setDeviceConfig(const QString deviceConfig)
{
    QFileInfo configFile(QDir(m_rserverHome), deviceConfig);

    if (configFile.exists() && configFile.isFile()) {
        m_deviceConfig = deviceConfig;
    } else {
        qprofileDebug(QtWarningMsg) << "RServer device config file is not existed.";
    }
}

const QString QRserverEnvironment::profileConfig() const
{
    return m_profileConfig;
}
void QRserverEnvironment::setProfileConfig(const QString profileConfig)
{
    m_profileConfig = profileConfig;
}

const QString QRserverEnvironment::vConncetServerConfig() const
{
    return m_vConncetServerConfig;
}

const QString QRserverEnvironment::vConncetClientConfig() const
{
    return m_vConncetClientConfig;
}

const QString QRserverEnvironment::deviceRelatedProfileConfig() const
{
    return m_deviceRelatedProfileConfig;
}

void QRserverEnvironment::setDeviceRelatedProfileConfig(const QString
        deviceRelatedProfileConfig)
{
    m_deviceRelatedProfileConfig = deviceRelatedProfileConfig;
}

const QStringList QRserverEnvironment::ignoredProperties() const
{
    QStringList ignoredPropertiesList(QAbstractEnvironment::ignoredProperties());

    for (int i = metaObject()->propertyOffset(); i < metaObject()->propertyCount();
         ++i) {
        ignoredPropertiesList << QString(metaObject()->property(i).name());
    }

    return ignoredPropertiesList;
}

const QString QRserverEnvironment::QRserverEnvironment::resultConfig() const
{
    return m_resultConfig;
}
void QRserverEnvironment::setResultConfig(const QString resultConfig)
{
    QFileInfo configFile(QDir(m_rserverHome), resultConfig);

    if (configFile.exists() && configFile.isFile()) {
        m_resultConfig = resultConfig;
    } else {
        qprofileDebug(QtWarningMsg) << "RServer result config file is not existed.";
    }
}

const QString
QRserverEnvironment::QRserverEnvironment::remoteExecutorCMDConfig() const
{
    return m_remoteExecutorCMDConfig;
}
void QRserverEnvironment::setRemoteExecutorCMDConfig(const QString
        remoteExecutorCMDConfig)
{
    QFileInfo configFile(QDir(m_rserverHome), remoteExecutorCMDConfig);

    if (configFile.exists() && configFile.isFile()) {
        m_remoteExecutorCMDConfig = remoteExecutorCMDConfig;
    } else {
        qprofileDebug(QtWarningMsg) <<
                                    "Remote executor CMD config file is not existed.";
    }
}

const QString QRserverEnvironment::QRserverEnvironment::storageRoot() const
{
    return m_storageRoot + "/";
}
void QRserverEnvironment::setStorageRoot(const QString storageRoot)
{
    QDir storageRootDir(storageRoot);
#ifdef BUILD_X86

    if (!storageRootDir.exists()) {
        storageRootDir.mkpath(storageRoot);
    }

#endif

    if (storageRootDir.isAbsolute() && storageRootDir.exists()) {
        m_storageRoot = storageRoot;
    } else {
        qprofileDebug(QtWarningMsg) << "Storage path is not exist or not absolute.";
    }

#ifdef BUILD_X86
    QFile storageDir(storageRootDir.absolutePath());
    storageDir.setPermissions(QFile::ReadOwner | QFile::WriteOwner | QFile::ReadUser
                              | QFile::WriteUser | QFile::ReadOther | QFile::WriteOther | QFile::WriteGroup |
                              QFile::ReadGroup | QFile::ExeOwner | QFile::ExeUser | QFile::ExeGroup |
                              QFile::ExeOther);
#endif
}

const QString QRserverEnvironment::deviceInfo() const
{
    return m_deviceInfo;
}
void QRserverEnvironment::setDeviceInfo(const QString deviceInfo)
{
    m_deviceInfo = deviceInfo;
}

const QString QRserverEnvironment::version() const
{
    return QString(RSERVER_VERSION);
}

void QRserverEnvironment::setVersion(const QString version)
{
    if (version != QString(RSERVER_VERSION)) {
        qprofileDebug(QtWarningMsg) << "Set version is not allowed.";
        QTimer::singleShot(500, this, SLOT(versionChanged()));
    }
}

const QString QRserverEnvironment::storageImage() const
{
    return m_storageImage  + "/";
}
void QRserverEnvironment::setStorageImage(const QString storageImage)
{
    QDir storageImageDir(storageRoot() + storageRserver() + storageImage);
    m_storageImage = storageImage;

    if (!storageImageDir.exists()) {
        if (!storageImageDir.mkpath(storageImageDir.absolutePath())) {
            qprofileDebug(QtWarningMsg) << "Failed to mk dir " <<
                                        storageImageDir.absolutePath();
        }

#ifdef BUILD_X86
        QFile storageDir(storageImageDir.absolutePath());
        storageDir.setPermissions(QFile::ReadOwner | QFile::WriteOwner | QFile::ReadUser
                                  | QFile::WriteUser | QFile::ReadOther | QFile::WriteOther | QFile::WriteGroup |
                                  QFile::ReadGroup | QFile::ExeOwner | QFile::ExeUser | QFile::ExeGroup |
                                  QFile::ExeOther);
#endif
    }
}

const QString QRserverEnvironment::QRserverEnvironment::storageRserver() const
{
    return m_storageRserver  + "/";
}
void QRserverEnvironment::setStorageRserver(const QString storageRserver)
{
    QDir storageRserverDir(this->storageRoot() + storageRserver);
    m_storageRserver = storageRserver;

    if (!storageRserverDir.exists()) {
        if (!storageRserverDir.mkpath(storageRserverDir.absolutePath())) {
            qprofileDebug(QtWarningMsg) << "Failed to mk dir " <<
                                        storageRserverDir.absolutePath();
        }

#ifdef BUILD_X86
        QFile storageDir(storageRserverDir.absolutePath());
        storageDir.setPermissions(QFile::ReadOwner | QFile::WriteOwner | QFile::ReadUser
                                  | QFile::WriteUser | QFile::ReadOther | QFile::WriteOther | QFile::WriteGroup |
                                  QFile::ReadGroup | QFile::ExeOwner | QFile::ExeUser | QFile::ExeGroup |
                                  QFile::ExeOther);
#endif
    }
}

void QRserverEnvironment::versionChanged()
{
    toFile(StorageRserver()  + "/" + defaultRserverEnvProfile);
}

const QString QRserverEnvironment::Version()
{
    return instance()->version();
}
