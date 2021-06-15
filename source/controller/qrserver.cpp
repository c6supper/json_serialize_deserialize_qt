/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qobjectfactory.h"
#include "qmodelfactory.h"
#include "qrserver.h"
#include "qrservermodel.h"
#include "qmsgregistresult.h"
#include "qmsgunregistresult.h"
#include "qrserverenvironment.h"
#include "qabstracttopic.h"
#include "qmsgcheckall.h"
#include "qrserverdevicetype.h"
#include "qprocessmessagecommand.h"
#include "qdeviceinfo.h"
#include "qmsgchecktarget.h"
#include "qprocessmessagestatus.h"
#include "qgenericbinmessage.h"
#include "qtimerstatuswatcher.h"
#include "qclientmodel.h"
#include "qgenericstatus.h"
#include "qmsgupdatevconnectinfo.h"
#include "qpongstatus.h"
#include "qrservercompatibility.h"
#include "qmsgserverversion.h"
#include "qcompatibilityfactory.h"
#include "qmsgupdateversioninfo.h"
#include "qrservercompatibility501.h"
#include "qmsggetsoftwarebytype.h"
#include "qmsgtestsetstatus.h"
#include "qvconnectclientmodel.h"

QObjectSelfRegisteration<QRserver::QRserver> registerQRserver;

QRserver::QRserver::QRserver(QObject *parent)
    : QRserverContainer(parent)
{
    //set default compatible module
    QAbstractCompatibility::SetInstance(new QRserverCompatibility());
    subscribeMessage(rserver::eRegistResult);
    subscribeMessage(rserver::eUnRegistResult);
    subscribeMessage(rserver::eCheckAll);
    subscribeMessage(rserver::eCheckAll2);
    subscribeMessage(rserver::eUpdateVConnectInfo);
    subscribeMessage(rserver::eServerVersion);
    subscribeMessage(rserver::eGetSoftwareByType);
    subscribeMessage(rserver::eSoftwareVersion);
    subscribeMessage(rserver::eTestsetStatus);
    m_register = QSharedPointer<QRegister>(new QRegister(this));
    m_upload = QSharedPointer<QUpload>(new QUpload(this));
    m_download = QSharedPointer<QDownload>(new QDownload(this));
    m_software = QSharedPointer<QSoftware>(new QSoftware(this));
    //todo should remove invalid code
    m_networkCheckTimer = new QTimer(this);
    m_networkCheckTimer->setInterval(checkNetworkInterval);
    m_networkCheckTimer->setSingleShot(false);
    connect(m_networkCheckTimer, SIGNAL(timeout()), this, SLOT(onCheckNetwork()));
    updateVconnectInfoTimer = new QTimer(this);
    updateVconnectInfoTimer->setInterval(updateVconnectInfoInterval);
    updateVconnectInfoTimer->setSingleShot(false);
    connect(updateVconnectInfoTimer, SIGNAL(timeout()), this,
            SLOT(onUpdateVconnectInfo()));
}

bool QRserver::QRserver::processCommands(const QString &commandId,
        const QString &commandType, QVariant commandVar, QVariant *status)
{
    if (!commandType.isEmpty()) {
        qprofileDebug(QtDebugMsg) << commandType;
        QGenericStatus updateStatus;

        if (commandType == QVConnectModel::CommandUpdateVConnectInfo) {
            updateStatus.fromVariant(updateVConnectInfo(commandVar));
        }

        if (commandType == QRserverModel::CommandPing) {
            updateStatus.fromVariant(pong());
        }

        if (status != nullptr) {
            updateStatus.setId(commandId);
            updateStatus.setMethod(commandType);
            *status = updateStatus.toVariant();
        }
    }

    return true;
}

const QVariant QRserver::QRserver::pong()
{
    QPongStatus pongStatus;
    QDeviceInfo::DeviceStatus status = QRserverModel::GetDeviceStatus(
                                           *m_rserverModel);

    if (status == QDeviceInfo::Unregistered) {
        pongStatus.setCommandStatus((QAbstractStatus::CommandStatus)
                                    QPongStatus::Unregistered);
    } else if (status == QDeviceInfo::RegisteredUnauthorized) {
        pongStatus.setCommandStatus((QAbstractStatus::CommandStatus)
                                    QPongStatus::RegisteredUnauthorized);
    } else if (status == QDeviceInfo::RegisteredAuthorized) {
        pongStatus.setCommandStatus((QAbstractStatus::CommandStatus)
                                    QPongStatus::RegisteredAuthorized);
    } else {
        pongStatus.setCommandStatus((QAbstractStatus::CommandStatus)
                                    QPongStatus::NotChecked);
    }

    if (m_vconnectModel->rserverMode() == QVConnectModel::ModeVConnect) {
        pongStatus.setCommandStatus((QAbstractStatus::CommandStatus)
                                    QPongStatus::VConnectMode);
    }

    return pongStatus.toVariant();
}

const QVariant QRserver::QRserver::updateVConnectInfo(QVariant argsVar)
{
    m_vconnectModel->fromVariant(argsVar);
    qprofileDebug(QtDebugMsg) << m_vconnectModel->toJson();
    QGenericStatus updateStatus;

    if (m_vconnectModel->rserverMode() != QVConnectModel::ModeVConnect) {
        updateStatus.setCommandStatus(QAbstractStatus::Succeed);
        return updateStatus.toVariant();
    }

    updateStatus.setCommandStatus(updateVConnectInfo2Server(
                                      m_vconnectModel->toVariant()));
    return updateStatus.toVariant();
}

const QAbstractStatus::CommandStatus
QRserver::QRserver::updateVConnectInfo2Server(
    const QVariant VConnectInfoVar)
{
    QProcessMessageCommand command(QVariant(), this, false);
    QMsgUpdateVConnectInfo sendVConnectInfo;
    sendVConnectInfo.fromVariant(VConnectInfoVar);

    if (QDeviceInfo::RegisteredAuthorized
        != QRserverModel::GetDeviceStatus(*m_rserverModel)) {
        sendVConnectInfo.setSerialNumber("");
    }

    //vconnect app use this value to verify the register status of the device.
    qprofileDebug(QtDebugMsg) << sendVConnectInfo.toJson();
    command.appendMessage(0, &sendVConnectInfo);
    QGenericBinMessage receiveVConnectInfo(rserver::eUpdateVConnectInfo);
    command.appendMessage(sendVConnectInfo.id(), &receiveVConnectInfo);
    command.setTopic(QVconnectClientModel::RserverVconnectCommandTopic);

    if (0 == publish(command.topic(), command.toJson())) {
        qprofileDebug(QtWarningMsg) << "publish command failed!";
        return QAbstractStatus::IPCError;
    } else {
        QTimerStatusWatcher watcher(this,
                                    QVconnectClientModel::RserverVconnectStatusTopic,
                                    command.id(), QVConnectModel::UpdateVConnectInfoTimeout);

        if (watcher.exec() == QTimerStatusWatcher::Timeout) {
            qprofileDebug(QtWarningMsg) << "UpdateVConnectInfo2Server timeout ";
            return QAbstractStatus::Timeout;
        }

        QProcessMessageStatus status(watcher.status());
        return status.commandStatus();
    }
}

//todo should remove invalid code
void QRserver::QRserver::onCheckNetwork()
{
    if (QRserverModel::GetDeviceStatus(*m_rserverModel) !=
        QDeviceInfo::RegisteredAuthorized) {
        return;
    }

    const QVariant chassisVar = m_rserverModel->getChassis();

    if (!chassisVar.isValid()) {
        return;
    }

    QProcessMessageCommand command(chassisVar, this, false);
    QMsgCheckTarget checkTargetMessage;
    checkTargetMessage.fromVariant(chassisVar);
    command.appendMessage(0, &checkTargetMessage);
    QGenericBinMessage checkReceiveMessage(rserver::eVerifyResult);
    command.appendMessage(checkTargetMessage.id(), &checkReceiveMessage);

    if (isCompatible(QRserverCompatibility501::FeatureRegisterChassisOnly) &&
        m_rserverModel->deviceCount() > 1) {
        QDeviceInfo deviceInfo;
        deviceInfo.fromVariant(chassisVar);
        QMsgUpdateVersionInfo updateVersionMsg;
        updateVersionMsg.setSerialNumber(deviceInfo.serialNumber());
        updateVersionMsg.setDeviceList(m_rserverModel->deviceList());
        command.appendMessage(0, &updateVersionMsg);
        QGenericBinMessage succeedMsg(rserver::eSendCmdSucceed);
        command.appendMessage(updateVersionMsg.id(), &succeedMsg);
    }

    quint32 retryCount = 0;
    QProcessMessageStatus status;

    while (status.commandStatus() != QAbstractStatus::Succeed
           && retryCount++ < QAbstractCommand::RetryCountLimitation) {
        if (0 == publish(command.topic(), command.toJson())) {
            qprofileDebug(QtWarningMsg) << "publish command failed!";
            status.setCommandStatus(QAbstractStatus::IPCError);
            break;
        } else {
            QTimerStatusWatcher watcher(this, QClientModel::RserverClientStatusTopic,
                                        command.id(), networkcheckTimeout);

            if (watcher.exec() == QTimerStatusWatcher::Timeout) {
                status.setCommandStatus(QAbstractStatus::Timeout);
            } else {
                status.fromVariant(watcher.status());
            }
        }
    }
}

void QRserver::QRserver::onSubscribeFinished()
{
    m_rserverModel->fromFile(QRserverEnvironment::DeviceInfo());
    m_rserverModel->initialDeviceStatus();
    overwriteRetainModelMessage(QAbstractTopic::put);
    //R-Server get too much log message diable
    //m_networkCheckTimer->start();
}

void QRserver::QRserver::postponeInit()
{
    QRserverContainer::postponeInit();

    if (!m_topic.isNull()) {
        connect(m_topic.data(), SIGNAL(subscribeFinished()), this,
                SLOT(onSubscribeFinished()));
    }
}

bool QRserver::QRserver::initPropertiesMap(const QVariantMap &properties)
{
    if (properties.count() <= 0) {
        setProperty("tagName", "qrserver");
    }

    return QRserverContainer::initPropertiesMap(properties);
}

void QRserver::QRserver::allocModel()
{
    m_rserverModel = QPointer<QRserverModel>
                     (QModelFactory::construct<QRserverModel>(this));
    setModel(m_rserverModel.data());
    m_vconnectModel = QModelFactory::construct<QVConnectModel>(this);
    connect(m_vconnectModel, SIGNAL(rserverModeChanged(qint32)), this,
            SLOT(onRServerModeChanged(qint32)));
}

void QRserver::QRserver::onRegister(QAbstractBinMsg *message)
{
    QMsgRegisterResult *registerResult = qobject_cast<QMsgRegisterResult *>
                                         (message);

    if (registerResult != nullptr) {
        if (registerResult->status() == QMsgRegisterResult::Registered) {
            m_rserverModel->updateDeviceStatus(registerResult->serialNumber(),
                                               QDeviceInfo::RegisterSucceed);
        } else {
            m_rserverModel->updateDeviceStatus(registerResult->serialNumber(),
                                               QDeviceInfo::RegisterFailed);
        }
    }
}

void QRserver::QRserver::onUnregister(QAbstractBinMsg *message)
{
    QMsgRegisterResult *registerResult = qobject_cast<QMsgRegisterResult *>
                                         (message);

    if (registerResult != nullptr) {
        if (registerResult->status() == QMsgUnregistResult::notRegistered) {
            m_rserverModel->updateDeviceStatus(registerResult->serialNumber(),
                                               QDeviceInfo::UnregisterSucceed);
        } else {
            m_rserverModel->updateDeviceStatus(registerResult->serialNumber(),
                                               QDeviceInfo::UnregisterFailed);
        }
    }
}

void QRserver::QRserver::getDeviceVersion(QAbstractBinMsg *message)
{
    if (message->type() == eGetSoftwareByType) {
        QMsgGetSoftwareByType *version = dynamic_cast<QMsgGetSoftwareByType *>(message);

        if (version == nullptr) {
            qprofileDebug(QtCriticalMsg) << "Get wrong device version message " <<
                                         message->toJson();
            return;
        }

        m_rserverModel->updateRemoteVersion(version->deviceType(),
                                            version->softwareVersion());
    }

    if (message->type() == eSoftwareVersion) {
        QMsgGetSoftwareVersion *version = dynamic_cast<QMsgGetSoftwareVersion *>
                                          (message);

        if (version == nullptr) {
            qprofileDebug(QtCriticalMsg) << "Get wrong device version message " <<
                                         message->toJson();
            return;
        }

        m_rserverModel->updateRemoteVersion(version->serialNumber(),
                                            version->softwareVersion());
    }
}

void QRserver::QRserver::getServerVersion(QAbstractBinMsg *message)
{
    QMsgServerVersion *serverVersion = dynamic_cast<QMsgServerVersion *>(message);

    if (serverVersion == nullptr) {
        qprofileDebug(QtCriticalMsg) << "Get wrong server version message " <<
                                     message->toJson();
        return;
    }

    QAbstractCompatibility *compatibility = (QAbstractCompatibility *)
                                            QCompatibilityFactory::Create(serverVersion->serverVersion());

    if (compatibility == nullptr) {
        qprofileDebug(QtCriticalMsg) << "Failed to create compatibility for version " <<
                                     serverVersion->serverVersion();
    }

    qprofileDebug(QtDebugMsg) << "Create compatibility version " <<
                              compatibility->toJson();
    QAbstractCompatibility::SetInstance(compatibility);
}

void QRserver::QRserver::processMessage(QAbstractBinMsg *message)
{
    Q_ASSERT(message != nullptr);

    switch (message->type()) {
    case rserver::eRegistResult:
        onRegister(message);
        break;

    case rserver::eUnRegistResult:
        onUnregister(message);
        break;

    case rserver::eCheckAll:
    case rserver::eCheckAll2:
        updateCheckAllInfo(message);
        break;

    case rserver::eServerVersion:
        getServerVersion(message);
        break;

    case rserver::eGetSoftwareByType:
        getDeviceVersion(message);
        break;

    case rserver::eSoftwareVersion:
        getDeviceVersion(message);

    case rserver::eTestsetStatus:
        updateTestsetStatus(message);

    default:
        qprofileDebug(QtDebugMsg) << message->toJson();
        break;
    }
}

//todo should check later
void QRserver::QRserver::updateCheckAllInfo(QAbstractBinMsg *message)
{
    QMsgCheckAll *checkAllInfo = qobject_cast<QMsgCheckAll *>(message);

    if (checkAllInfo != nullptr) {
        qprofileDebug(QtDebugMsg) << checkAllInfo->toJson();
        bool isChassis = m_rserverModel->isChassis(checkAllInfo->serialNumber());

        if (isChassis
            || !isCompatible(QRserverCompatibility500::FeatureGetSWVersionByType)) {
            if (checkAllInfo->status() < QDeviceInfo::Unknown) {
                m_rserverModel->updateDeviceStatus(checkAllInfo->serialNumber(),
                                                   checkAllInfo->status());
            } else {
                m_rserverModel->updateDeviceStatus(checkAllInfo->serialNumber(),
                                                   QDeviceInfo::Unregistered);
            }

            m_rserverModel->updateRemoteVersion(checkAllInfo->serialNumber(),
                                                checkAllInfo->remoteVersion());
        }

        if (isChassis) {
            setLocalDateTime(checkAllInfo->serverDateTime());

            if ((QVConnectModel::ModeVConnect == m_vconnectModel->rserverMode())
                && (QDeviceInfo::RegisteredAuthorized
                    == checkAllInfo->status())) {
                onUpdateVconnectInfo();
            }
        }
    }
}

void QRserver::QRserver::updateTestsetStatus(QAbstractBinMsg *message)
{
    QMsgTestsetStatus *testsetStatus = qobject_cast<QMsgTestsetStatus *>(message);

    if (testsetStatus != nullptr) {
        qprofileDebug(QtDebugMsg) << testsetStatus->toJson();

        if (m_rserverModel->isChassis(testsetStatus->serialNumber())) {
            m_rserverModel->updateTestsetStatus(testsetStatus->serialNumber(),
                                                testsetStatus->testsetStatus());
        }
    }
}

void QRserver::QRserver::setLocalDateTime(const QDateTime dateTime)
{
#if defined(Q_OS_LINUX)
    /* set system time */
    time_t localDateTime = (time_t)dateTime.toTime_t();
    stime(&localDateTime);
#if !defined(BUILD_X86)
    /* set flag for sync hw clock time. */
    QRserverEnvironment::TouchTmpFile(
        QString(".rserver_sync_hwdatetime_indicator"));
#endif
#endif
}

void QRserver::QRserver::onUpdateVconnectInfo()
{
    QProcessMessageCommand command(QVariant(), this, false);
    QMsgUpdateVConnectInfo sendVConnectInfo;
    sendVConnectInfo.fromVariant(m_vconnectModel->toVariant());

    if (QDeviceInfo::RegisteredAuthorized
        != QRserverModel::GetDeviceStatus(*m_rserverModel)) {
        sendVConnectInfo.setSerialNumber("");
    }

    //vconnect app use this value to verify the register status of the device.
    command.appendMessage(0, &sendVConnectInfo);
    QGenericBinMessage succeedMsg(rserver::eSendCmdSucceed);
    command.appendMessage(sendVConnectInfo.id(), &succeedMsg);
    command.setTopic(QVconnectClientModel::RserverVconnectCommandTopic);

    if (0 == publish(command.topic(), command.toJson())) {
        qprofileDebug(QtWarningMsg) << "Update vonnect info failed!";
    }
}

void QRserver::QRserver::onRServerModeChanged(qint32 rserverMode)
{
    if (QVConnectModel::ModeVConnect == rserverMode) {
        updateVconnectInfoTimer->start();
    } else {
        updateVconnectInfoTimer->stop();
    }
}
