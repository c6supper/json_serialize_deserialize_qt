#include "qsoftware.h"
#include "qmodelfactory.h"
#include "qdeviceinfo.h"
#include "qupgradecommand.h"
#include "qprofile_debug.h"
#include "qmsgtestsetswversioninfo.h"
#include "qmsgtestsetswinfo.h"
#include "qmsgtestsetswblk.h"
#include "qmessagefactory.h"
#include "qmessagedispatcher.h"
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QDateTime>
#include <QApplication>
#include <QCryptographicHash>
#include "qrserverenvironment.h"
#include "qgenericbinmessage.h"
#include "qrserverdevicetype.h"
#include "qrserverdevice.h"
#include "qmsgtestsetswversionandurl.h"
#if defined(Q_OS_LINUX)
#include <sys/stat.h>
#include <sys/statvfs.h>
#include<unistd.h>
#include<sys/types.h>
#endif
#include "qrservercompatibility605.h"

using namespace QProfile;
using namespace QRserver;

qint64 QSoftware::getFreeSpace(const QString &dir)
{
    qint64 freeSpace = 0;
#if defined(Q_OS_LINUX)
    struct statvfs st;

    if (::statvfs(dir.toLocal8Bit(), &st) == -1)
        return freeSpace;

    freeSpace = st.f_bfree * st.f_frsize;
#endif
    return freeSpace;
}

QSoftware::QSoftware(QObject *parent): QRserverContainer(parent, false),
    m_softwareModel(nullptr), m_rserverSoftwareClient(nullptr)
{
    subscribeMessage(rserver::eConnectPkgUp);
    subscribeMessage(rserver::eTestsetSWinfo);
    subscribeMessage(rserver::eTestsetSWBLK);
    subscribeMessage(rserver::eTestsetSWEND);
    subscribeMessage(rserver::eUpgradeFail);
    subscribeMessage(rserver::eSoftwareVersion);
    subscribeMessage(rserver::eCheckAll);
    subscribeMessage(rserver::eCheckAll2);
    subscribeMessage(rserver::eTestsetSWVersionAndURL);

    if (m_rserverSoftwareClient == nullptr) {
        m_rserverSoftwareClient = new QRserverSoftwareClient(this);
        m_rserverSoftwareClient->registerMessageCallback(this, rserverMessageCallback);
    }

    m_upgradeTimer = new QTimer(this);
    m_upgradeTimer->setInterval(QSoftwareModel::PackageTimeout);
    m_upgradeTimer->setSingleShot(true);
    connect(m_upgradeTimer, SIGNAL(timeout()), this, SLOT(timeout()));
    m_download = new QRserverDownloadProcess(this);
    m_httpDownloadReportTimer = new QTimer(this);
    m_httpDownloadReportTimer->setInterval(QSoftwareModel::PropertyChangedTimeout);
    connect(m_httpDownloadReportTimer, SIGNAL(timeout()), this,
            SLOT(onReportHttpDownloadStatus()));
}

void QSoftware::timeout()
{
    QGenericBinMessage message;
    message.fromVariant(getUpgradeInfo());
    onUpgradeStop(message.serialNumber(), QUpgradeInfo::Timeout);
}

bool QSoftware::initPropertiesMap(const QVariantMap &properties)
{
    if (properties.count() <= 0) {
        setProperty("tagName", "qsoftware");
    }

    return QRserverContainer::initPropertiesMap(properties);
}

bool QSoftware::processCommands(const QString &commandId,
                                const QString &commandType, QVariant commandVar, QVariant *status)
{
    if (!commandType.isEmpty()) {
        qprofileDebug(QtDebugMsg) << commandType;

        if (commandType == QSoftwareModel::CommandUpgrade) {
            upgrade(commandVar);
        }
    }

    //todo
    return true;
}

bool QSoftware::isUpgrading()
{
    return m_upgradeTimer->isActive() && m_reportTimer.isValid();
}

const QString QSoftware::getDeltaImagePathName(const QString serialNumber)
{
    QString upgradeStorage = QRserverEnvironment::StorageImage();
    return QString("%1/%2").arg(upgradeStorage).arg(
               m_softwareModel->getUpgradeInfoDeltaImageName(serialNumber));
}

void QSoftware::onUpgradeStop(const QString &serialNumber, qint32 status)
{
    m_upgradeTimer->stop();
    m_httpDownloadReportTimer->stop();
    m_reportTimer.invalidate();

    if (status != QUpgradeInfo::DownloadFileSuccess) {
        QFile file(getDeltaImagePathName(serialNumber));

        if (file.exists()) {
            file.remove();
        }
    }

    setUpgradeInfoStatus(serialNumber, status);
    m_rserverSoftwareClient->registerMessageCallback(nullptr, nullptr);
    m_rserverSoftwareClient->disconnect();
    delete m_rserverSoftwareClient;
    m_rserverSoftwareClient = new QRserverSoftwareClient(this);
    m_rserverSoftwareClient->registerMessageCallback(this, rserverMessageCallback);
    onModelChanged();
}

bool QSoftware::upgrade(const QVariant argsVar)
{
    if (!isUpgrading()) {
        QVariantList deviceInfoList = argsVar.toList();

        if (!deviceInfoList.isEmpty()) {
            m_upgradeTimer->start();
            m_reportTimer.restart();
            m_softwareModel->setUpgradeInfoList(deviceInfoList);
            QUpgradeInfo localDeviceInfo;
            localDeviceInfo.fromVariant(getUpgradeInfo());

            //in case local modules not refreshed.
            //Attention: Full image upgrade may be needed in the future.
            if (localDeviceInfo.version().isEmpty()) {
                onUpgradeStop(localDeviceInfo.serialNumber(), QUpgradeInfo::ModuleVersionEmpty);
                return true;
            }

            const QRegisterModel &reigsterModel = m_softwareModel->registerModel();
            m_rserverSoftwareClient->ConnectCmd(reigsterModel.serverIp().toLatin1().data(),
                                                (isCompatible(QRserverCompatibility605::FeatureUpgradeViaHttp) ?
                                                 reigsterModel.serverPort() : m_softwareModel->serverPort()),
                                                reigsterModel.networkDevice().toLatin1().data(),
                                                rserver::eConnectPkgUp);
        }
    } else {
        qprofileDebug(QtDebugMsg) << "Currently in upgarding " <<
                                  m_softwareModel->upgradeInfoList();
    }

    return true;
}

bool QSoftware::saveSoftware(QAbstractBinMsg *message)
{
    QMsgTestsetSwBlk *testsetSwBlk = qobject_cast<QMsgTestsetSwBlk *>(message);
    Q_ASSERT(testsetSwBlk != nullptr);
    QFile file(getDeltaImagePathName(message->serialNumber()));
    QFileInfo fileInfo(getDeltaImagePathName(message->serialNumber()));
    QIODevice::OpenMode openMode = QIODevice::WriteOnly;

    if (file.exists()) {
        QDateTime lastModified = fileInfo.lastModified();
        qint64 milliseconds = lastModified.msecsTo(QDateTime::currentDateTime());
        qint64 imageSize = m_softwareModel->getUpgradeInfoImageSize(
                               message->serialNumber());

        if (milliseconds > 0 && milliseconds < QSoftwareModel::PackageTimeout
            && imageSize > fileInfo.size()) {
            openMode |= QIODevice::Append;
        } else {
            openMode |= QIODevice::Truncate;
        }
    } else {
        QDir upgradePath(fileInfo.absolutePath());

        if (!upgradePath.exists()) {
            if (!upgradePath.mkpath(fileInfo.absolutePath())) {
                qprofileDebug(QtWarningMsg) << "Failed to create directory " <<
                                            fileInfo.absolutePath();
                onUpgradeStop(testsetSwBlk->serialNumber(), QUpgradeInfo::SaveFileFailed);
                return false;
            }
        }
    }

    if (!file.open(openMode)) {
        qprofileDebug(QtWarningMsg) << "Failed to open file " <<
                                    fileInfo.absoluteFilePath();
        onUpgradeStop(testsetSwBlk->serialNumber(), QUpgradeInfo::SaveFileFailed);
        return false;
    }

    if (testsetSwBlk->length() != file.write(QByteArray::fromBase64(
                testsetSwBlk->block()))) {
        qprofileDebug(QtWarningMsg) << "Failed to write file " <<
                                    fileInfo.absoluteFilePath();
        file.remove();
        onUpgradeStop(testsetSwBlk->serialNumber(), QUpgradeInfo::SaveFileFailed);
        return false;
    }

    file.close();
    fileInfo.refresh();
    m_softwareModel->setUpgradeInfoDownloadedSize(testsetSwBlk->serialNumber(),
            fileInfo.size() + testsetSwBlk->length());

    if (message->type() == rserver::eTestsetSWEND) {
        if (!file.open(QIODevice::ReadOnly)) {
            qprofileDebug(QtWarningMsg) << "Failed to open file " <<
                                        fileInfo.absoluteFilePath();
            onUpgradeStop(testsetSwBlk->serialNumber(), QUpgradeInfo::SaveFileFailed);
            return false;
        }

        QByteArray localSoftwareMD5 = QDownloadProcess::GetMD5(file.fileName());
        QByteArray serverSoftwareMD5 = m_softwareModel->getUpgradeInfoMd5(
                                           testsetSwBlk->serialNumber());

        if (localSoftwareMD5.toUpper() != serverSoftwareMD5.toUpper()) {
            qprofileDebug(QtWarningMsg) << "MD5<Server/Local>mismatch !" <<
                                        serverSoftwareMD5.toHex() << localSoftwareMD5.toHex();
            onUpgradeStop(testsetSwBlk->serialNumber(), QUpgradeInfo::MD5Mismatch);
            return false;
        }

        const QRserverDeviceType *rserverDeviceType =
            QRserverDevice::SerialNumber2RserverDeviceType(testsetSwBlk->serialNumber());

        if (rserverDeviceType == nullptr) {
            qprofileDebug(QtWarningMsg) << "get QRserverDeviceType failed! " <<
                                        testsetSwBlk->serialNumber();
            onUpgradeStop(testsetSwBlk->serialNumber(), QUpgradeInfo::SaveFileFailed);
            return false;
        }

        onUpgradeStop(testsetSwBlk->serialNumber(),
                      QUpgradeInfo::DownloadFileSuccess);
        executeDeltaUpgrade(QStringList() << fileInfo.absoluteFilePath() <<
                            QRserverDeviceType::DeviceType2String(rserverDeviceType->deviceType()) <<
                            QRserverDeviceType::ImageType2String(rserverDeviceType->imageType()) <<
                            rserverDeviceType->deviceName() << rserverDeviceType->uncompressDir() <<
                            rserverDeviceType->targetDir());
    }

    return true;
}

bool QSoftware::downloadPacViaHttp(QAbstractBinMsg *message)
{
    QMsgTestsetSWVersionAndURL *versionAndURLRecvMsg =
        qobject_cast<QMsgTestsetSWVersionAndURL *>(message);
    Q_ASSERT(versionAndURLRecvMsg != nullptr);
    QUpgradeInfo localDeviceInfo;
    localDeviceInfo.fromVariant(getUpgradeInfo());

    if (localDeviceInfo.version() == versionAndURLRecvMsg->version()) {
        onUpgradeStop(localDeviceInfo.serialNumber(), QUpgradeInfo::VersionIsNewest);
        return false;
    }

    if ((localDeviceInfo.version() != versionAndURLRecvMsg->upgradeFromVersion())
        || versionAndURLRecvMsg->version().isEmpty()) {
        onUpgradeStop(localDeviceInfo.serialNumber(), QUpgradeInfo::VersionCantFind);
        return false;
    }

    QFileInfo fileInfo(getDeltaImagePathName(message->serialNumber()));
    QDir upgradePath(fileInfo.absolutePath());

    if (fileInfo.exists()) {
        QFile::remove(fileInfo.absoluteFilePath());
    }

    if (!upgradePath.exists()) {
        if (!upgradePath.mkpath(fileInfo.absolutePath())) {
            qprofileDebug(QtWarningMsg) << "Failed to create directory " <<
                                        fileInfo.absolutePath();
            onUpgradeStop(localDeviceInfo.serialNumber(), QUpgradeInfo::CreatePathFailed);
            return false;
        }
    }

    qprofileDebug(QtDebugMsg)  << "upgradePath.path() = " << upgradePath.path();
    qprofileDebug(QtDebugMsg)  << "fileInfo.fileName() = " << fileInfo.fileName();
    m_download->wget(versionAndURLRecvMsg->deltaImageURL(), upgradePath.path(),
                     fileInfo.fileName(), 0, versionAndURLRecvMsg->deltaImageChecksum());
    m_httpDownloadReportTimer->start();
    return true;
}

void QSoftware::onReportHttpDownloadStatus()
{
    QUpgradeInfo localDeviceInfo;
    localDeviceInfo.fromVariant(getUpgradeInfo());

    switch (m_download->downloadStatus()) {
    case QDownloadProcess::Idle://the initial status should not be processed.
        return;

    case QDownloadProcess::Downloading:
    case QDownloadProcess::CheckingMD5: {
        m_softwareModel->setUpgradeInfoStatus(localDeviceInfo.serialNumber(),
                                              QUpgradeInfo::FineToUpgrade);
        m_softwareModel->setUpgradeInfoDownloadedPercentage(
            localDeviceInfo.serialNumber(), m_download->downloadedPercentage());
        m_upgradeTimer->start();
        onModelChanged();
        break;
    }

    case QDownloadProcess::NotEnoughSpace:
        onUpgradeStop(localDeviceInfo.serialNumber(), QUpgradeInfo::NotEnoughSpace);
        break;

    case QDownloadProcess::Timeout:
        onUpgradeStop(localDeviceInfo.serialNumber(), QUpgradeInfo::Timeout);
        break;

    case QDownloadProcess::MD5Mismatch:
        onUpgradeStop(localDeviceInfo.serialNumber(), QUpgradeInfo::MD5Mismatch);
        break;

    case QDownloadProcess::StartFailed:
        onUpgradeStop(localDeviceInfo.serialNumber(), QUpgradeInfo::StartFailed);
        break;

    case QDownloadProcess::RemoteFileNotExists:
        onUpgradeStop(localDeviceInfo.serialNumber(),
                      QUpgradeInfo::RemoteFileNotExists);
        break;

    case QDownloadProcess::SaveDirectoryNotExists:
        onUpgradeStop(localDeviceInfo.serialNumber(),
                      QUpgradeInfo::SaveDirectoryNotExists);
        break;

    case QDownloadProcess::Succeed: {
        QString fullPathOfDeltaImag = getDeltaImagePathName(
                                          localDeviceInfo.serialNumber());
        QFileInfo fileInfo(fullPathOfDeltaImag);

        //in case wget returned successfully but files have not been downloaded
        if (!fileInfo.exists()) {
            onUpgradeStop(localDeviceInfo.serialNumber(), QUpgradeInfo::SaveFileFailed);
            return;
        }

        const QRserverDeviceType *rserverDeviceType =
            QRserverDevice::SerialNumber2RserverDeviceType(localDeviceInfo.serialNumber());
        onUpgradeStop(localDeviceInfo.serialNumber(),
                      QUpgradeInfo::DownloadFileSuccess);
        executeDeltaUpgrade(QStringList() << fileInfo.absoluteFilePath() <<
                            QRserverDeviceType::DeviceType2String(rserverDeviceType->deviceType()) <<
                            QRserverDeviceType::ImageType2String(rserverDeviceType->imageType()) <<
                            rserverDeviceType->deviceName() << rserverDeviceType->uncompressDir() <<
                            rserverDeviceType->targetDir());
        break;
    }

    default:
        onUpgradeStop(localDeviceInfo.serialNumber(), QUpgradeInfo::UnknowError);
        break;
    }
}

void QSoftware::executeDeltaUpgrade(const QStringList &upgradeParam)
{
#if !defined(BUILD_X86)
    sleep(2);
    QString cmd = QString("/tmp/rserverdeltaupgrade %1").arg(
                      upgradeParam.join(" "));
    qprofileDebug(QtDebugMsg)  << "\ninstall cmd:" << cmd;
    system("cp -f /usr/local/bin/rserverdeltaupgrade /tmp/rserverdeltaupgrade");
    system("sync");
    pid_t pid = fork();

    if (0 == pid) {
        system(cmd.toStdString().c_str());
        exit(0);
    }

    QApplication::quit();
#endif
}

void QSoftware::setUpgradeInfoStatus(const QString serialNumber,
                                     const quint32 status)
{
    m_softwareModel->setUpgradeInfoStatus(serialNumber, status);
}

bool QSoftware::onTestSwInfo(QAbstractBinMsg *message)
{
    QMsgTestsetSwInfo *testsetSwInfo = qobject_cast<QMsgTestsetSwInfo *>(message);
    Q_ASSERT(testsetSwInfo != nullptr);

    if (testsetSwInfo->softwareStatus() != QUpgradeInfo::FineToUpgrade) {
        onUpgradeStop(testsetSwInfo->serialNumber(), testsetSwInfo->softwareStatus());
        return false;
    }

    QFileInfo imageFile(getDeltaImagePathName(message->serialNumber()));
    qint64 free = getFreeSpace(imageFile.absolutePath());

    if (free <= testsetSwInfo->softwareLength()) {
        qprofileDebug(QtDebugMsg) << imageFile.absolutePath() <<
                                  "NotEnoughSpace, free = " << free << ",image = " <<
                                  testsetSwInfo->softwareLength();
        onUpgradeStop(testsetSwInfo->serialNumber(), QUpgradeInfo::NotEnoughSpace);
        return false;
    }

    if (testsetSwInfo->softwareLength() >
        QSoftwareModel::MaxPacSizeForOldUpgradingMethod) {
        onUpgradeStop(testsetSwInfo->serialNumber(), QUpgradeInfo::ExceedCapOfPacSize);
        return false;
    }

    if (imageFile.exists()) {
        QFile::remove(imageFile.absoluteFilePath());
    }

    m_softwareModel->setUpgradeInfoStatus(testsetSwInfo->serialNumber(),
                                          testsetSwInfo->softwareStatus());
    m_softwareModel->setUpgradeInfoMd5(testsetSwInfo->serialNumber(),
                                       testsetSwInfo->softwareMd5());
    m_softwareModel->setUpgradeInfoImageSize(testsetSwInfo->serialNumber(),
            testsetSwInfo->softwareLength());
    qprofileDebug(QtDebugMsg) << "eTestsetSWinfo:" <<
                              testsetSwInfo->softwareMd5().toHex() << testsetSwInfo->softwareLength();
    return true;
}

void QSoftware::processMessage(QAbstractBinMsg *message)
{
    Q_ASSERT(message != nullptr);

    switch (message->type()) {
    case rserver::eConnectPkgUp: {
        if (isCompatible(QRserverCompatibility605::FeatureUpgradeViaHttp)) {
            QMsgTestsetSWVersionAndURL versionAndURLSendMsg;
            versionAndURLSendMsg.setDeviceType(
                (getUpgradeInfo().toMap()[QDeviceInfo::PropertyDeviceType]).toInt());
            m_rserverSoftwareClient->sendCmd(&versionAndURLSendMsg);
        } else {
            QMsgTestsetSwVersionInfo versionInfo;
            versionInfo.fromVariant(getUpgradeInfo());
            m_rserverSoftwareClient->sendCmd(&versionInfo);
        }
    }
    break;

    case rserver::eCheckAll:
    case rserver::eCheckAll2:
    case rserver::eSoftwareVersion: {
        QTimer::singleShot(QSoftwareModel::PropertyChangedTimeout, this,
                           SLOT(onModelChanged()));
    }

    return;

    case rserver::eTestsetSWinfo: {
        if (!onTestSwInfo(message)) {
            return;
        }
    }
    break;

    case rserver::eTestsetSWBLK:
    case rserver::eTestsetSWEND: {
        if (!saveSoftware(message)) {
            return;
        }
    }
    break;

    case rserver::eTestsetSWVersionAndURL: {
        if (!downloadPacViaHttp(message)) {
            return;
        }

        qprofileDebug(QtDebugMsg) << message->toJson();
    }
    break;

    default:
        onUpgradeStop(message->serialNumber(), QUpgradeInfo::UnknowError);
        return;
    }

    m_upgradeTimer->start();
    qint64 elapsed = m_reportTimer.elapsed();

    if (elapsed > QSoftwareModel::PropertyChangedTimeout) {
        onModelChanged();
        m_reportTimer.restart();
    }
}

void QSoftware::allocModel()
{
    setModel(m_softwareModel = QModelFactory::construct<QSoftwareModel>
                               (this));
    disconnect(m_softwareModel, SIGNAL(propertyChanged()), this,
               SLOT(onModelChanged()));
}

const QVariant QSoftware::getUpgradeInfo()
{
    QVariantList upgradeInfoList = m_softwareModel->upgradeInfoList();

    if (upgradeInfoList.count() > 0) {
        return upgradeInfoList.first();
    }

    return QVariant();
}

void QSoftware::rserverMessageCallback(void *obj, unsigned int type,
                                       const unsigned char *data, unsigned int length)
{
    QAbstractBinMsg *message = (QAbstractBinMsg *)QRserver::QMessageFactory::create(
                                   type, (const char *)data, length);
    QGenericBinMessage messageVar;
    QSoftware *software = (QSoftware *)obj;
    messageVar.fromVariant(software->getUpgradeInfo());
    message->setSerialNumber(messageVar.serialNumber());

    if (message != nullptr) {
        QMessageDispatcher::DispatchMessageEvent(message->type(),
                message->toVariant());
        delete message;
    }
}

