/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qobjectfactory.h"
#include "qregister.h"
#include "qregistermodel.h"
#include "qabstractbinmsg.h"
#include "qmodelfactory.h"
#include "qmsgregistertargetext.h"
#include "qmsgregistresult.h"
#include "qmsgunregisttarget.h"
#include "qmsgunregistresult.h"
#include "qclientmodel.h"
#include "qprocessmessagecommand.h"
#include "qmsgverifyresult.h"
#include "qgenericbinmessage.h"
#include "qmsgprofileinfoend.h"
#include "qprocessmessagestatus.h"
#include "qtimerstatuswatcher.h"
#include "qmsgcheckall.h"
#include "qrserverdevicetype.h"
#include "qdeviceinfo.h"
#include "qmsggetprofileinfo.h"
#include "qrservercompatibility501.h"
#include "qmsgupdateversioninfo.h"
#include "qmsgdeviceinfo.h"
#include "qmsgregistrationlock.h"
#include "qmsgupdatedeviceinfo.h"
#include "qmsgdevicebondinfo.h"
#include "qmsggetserverinfo.h"
#include "qrservercompatibility503.h"
#include "qmsgregistertargetextlongid.h"
#include "qmsgcheckalllongid.h"
#include "qmsgupdatedeviceinfolongid.h"
#include "qmsgunregisttargetlongid.h"
#include "qmsgtestsetstatus.h"
#include "qrservercompatibility509.h"
#include "qmsglockveexpressupgrade.h"
#include "qmsgblockprofileandchangetechname.h"
#include "qmsgupdatetechname.h"
#include "qmsgupdatetechnameresult.h"
#include "qmsgupdatevconnectinfo.h"
#include "qvconnectclientmodel.h"
#include "qvconnectmodel.h"

using namespace QRserver;

QObjectSelfRegisteration<QRegister> registerQRegister;

QRegister::QRegister(QObject *parent)
    : QRserverContainer(parent, false),
      m_updateDeviceInfoStatus(QMsgUpdateDeviceInfo::Failed),
      m_updataTechNameResult(false)
{
    subscribeMessage(rserver::eRegistResult);
    subscribeMessage(rserver::eUnRegistResult);
    subscribeMessage(rserver::eCheckAll);
    subscribeMessage(rserver::eCheckAll2);
    subscribeMessage(rserver::eCheckAllExt);
    subscribeMessage(rserver::eCheckAllExt2);
    subscribeMessage(rserver::eCheckAllExt3);
    subscribeMessage(rserver::eUpdateDeviceInfo);
    subscribeMessage(rserver::eUpdateDeviceInfo2);
    subscribeMessage(rserver::eUpdateTechName);
}

bool QRegister::processCommands(const QString &commandId,
                                const QString &commandType, QVariant commandVar, QVariant *status)
{
    if (!commandType.isEmpty()) {
        qprofileDebug(QtDebugMsg) << commandType;
        QRegisterStatus registerStatus;
        registerStatus.fromVariant(getServerVersion(commandVar));

        if (registerStatus.commandStatus() == QAbstractStatus::Succeed) {
            if (commandType == QRegisterModel::CommandRegister) {
                registerStatus.fromVariant(registerDevice(commandVar));
            } else if (commandType == QRegisterModel::CommandUnregister) {
                registerStatus.fromVariant(unregisterDevice(commandVar));
            } else if (commandType == QRegisterModel::CommandCheck) {
                registerStatus.fromVariant(checkDevice(commandVar));
            } else if (commandType == QRegisterModel::CommandUpdate) {
                registerStatus.fromVariant(upateDeviceInfo(commandVar));
            } else if (commandType == QRegisterModel::CommandUpdateTechName) {
                registerStatus.fromVariant(updateTechName(commandVar));
            }
        }

        if (status != nullptr) {
            registerStatus.setId(commandId);
            registerStatus.setMethod(commandType);
            registerStatus.setRserverModelVar(m_registerModel->rserverModel().toVariant());
            registerStatus.setRegisterModelVar(m_registerModel->toVariant());
            *status = registerStatus.toVariant();
        }
    }

    return true;
}

//just to compatible with old R-Server, register all the modules
const QVariant QRegister::registerDevice(const QVariant argsVar)
{
    const QRserverModel &rserverModel = m_registerModel->rserverModel();
    QVariantList deviceList = rserverModel.deviceList();
    QVariantList::const_iterator iter;
    m_registerModel->fromVariant(argsVar);
    qprofileDebug(QtDebugMsg) << argsVar;
    QRegisterStatus registerStatus;
    QDeviceInfo::TestsetStatus testsetStatus = QRserverModel::GetTestsetStatus(
                rserverModel);

    if (testsetStatus == QDeviceInfo::Revoked) {
        registerStatus.setCommandStatus((QAbstractStatus::CommandStatus)
                                        QRegisterStatus::TestsetBeRevoked);
        return registerStatus.toVariant();
    }

    if (testsetStatus == QDeviceInfo::Stolen) {
        registerStatus.setCommandStatus((QAbstractStatus::CommandStatus)
                                        QRegisterStatus::TestsetBeStolen);
        return registerStatus.toVariant();
    }

    for (iter = deviceList.constBegin(); iter != deviceList.constEnd(); ++iter) {
        registerStatus.setCommandStatus(QAbstractStatus::Failed);
        quint32 retryCount = 0;

        while (registerStatus.commandStatus() != QAbstractStatus::Succeed
               && retryCount++ < QAbstractCommand::RetryCountLimitation) {
            registerStatus.setCommandStatus(registerSingleDevice(*iter));

            if (registerStatus.commandStatus() != QAbstractStatus::Succeed) {
                qprofileDebug(QtWarningMsg) << "Failed to register device, status = " <<
                                            registerStatus.commandStatus() << "Retry count = " << retryCount;

                if (!registerStatus.canRecover()) {
                    return registerStatus.toVariant();
                }
            }
        }

        if (registerStatus.commandStatus() != QAbstractStatus::Succeed) {
            qprofileDebug(QtWarningMsg) << "Failed to register device, status =  " <<
                                        registerStatus.commandStatus();
            break;
        }
    }

    return registerStatus.toVariant();
}

const QAbstractStatus::CommandStatus QRegister::registerSingleDevice(
    const QVariant deviceVar)
{
    QDeviceInfo deviceInfo;
    deviceInfo.fromVariant(deviceVar);

    if (!deviceInfo.isChassis()) {
        //only register chassis
        return QAbstractStatus::Succeed;
    }

    QProcessMessageCommand command(deviceVar, this, false);
    QMsgRegisterTargetExt *pRegisterMessage = nullptr;
    QMsgRegisterTargetExtLongId msgRegisterTargetExtLongId;
    QMsgRegisterTargetExt msgRegisterTargetExt;

    if (isCompatible(QRserverCompatibility503::FeatureLongTechId)) {
        pRegisterMessage = &msgRegisterTargetExtLongId;
    } else {
        pRegisterMessage = &msgRegisterTargetExt;
    }

    pRegisterMessage->fromVariant(deviceVar);
    pRegisterMessage->fromVariant(model()->toVariant());
    command.appendMessage(0, pRegisterMessage);
    QGenericBinMessage registerResultMessage(rserver::eRegistResult);
    command.appendMessage(pRegisterMessage->id(), &registerResultMessage);
    const QRserverModel &rserverModel = m_registerModel->rserverModel();

    if (rserverModel.deviceCount() > 1) {
        QMsgUpdateVersionInfo updateVersionMsg;
        updateVersionMsg.setSerialNumber(deviceInfo.serialNumber());
        updateVersionMsg.setDeviceList(rserverModel.deviceList());
        command.appendMessage(0, &updateVersionMsg);
        QGenericBinMessage succeedMsg(rserver::eSendCmdSucceed);
        command.appendMessage(updateVersionMsg.id(), &succeedMsg);
    }

    QMsgDeviceBondInfo deviceBondMsg;
    deviceBondMsg.setDeviceList(rserverModel.deviceList());
    command.appendMessage(0, &deviceBondMsg);
    QGenericBinMessage succeedMsg(rserver::eSendCmdSucceed);
    command.appendMessage(deviceBondMsg.id(), &succeedMsg);

    if (0 == publish(command.topic(), command.toJson())) {
        qprofileDebug(QtWarningMsg) << "publish command failed!";
        return QAbstractStatus::IPCError;
    } else {
        QTimerStatusWatcher watcher(this, QClientModel::RserverClientStatusTopic,
                                    command.id(), QRegisterModel::RegisterSingleDeviceTimeout);

        if (watcher.exec() == QTimerStatusWatcher::Timeout) {
            qprofileDebug(QtWarningMsg) << "Register single device timeout ";
            return QAbstractStatus::Timeout;
        }

        QProcessMessageStatus status(watcher.status());
        return status.commandStatus();
    }
}

//just to compatible with old R-Server, unregister all the modules
const QVariant QRegister::unregisterDevice(const QVariant argsVar)
{
    const QRserverModel &rserverModel = m_registerModel->rserverModel();
    QVariantList deviceList = rserverModel.deviceList();
    QVariantList::const_iterator iter;
    m_registerModel->fromVariant(argsVar);
    qprofileDebug(QtDebugMsg) << argsVar;
    QRegisterStatus registerStatus;
    QDeviceInfo::TestsetStatus testsetStatus = QRserverModel::GetTestsetStatus(
                rserverModel);

    if (testsetStatus == QDeviceInfo::Revoked) {
        registerStatus.setCommandStatus((QAbstractStatus::CommandStatus)
                                        QRegisterStatus::TestsetBeRevoked);
        return registerStatus.toVariant();
    }

    if (testsetStatus == QDeviceInfo::Stolen) {
        registerStatus.setCommandStatus((QAbstractStatus::CommandStatus)
                                        QRegisterStatus::TestsetBeStolen);
        return registerStatus.toVariant();
    }

    for (iter = deviceList.constBegin(); iter != deviceList.constEnd(); ++iter) {
        registerStatus.setCommandStatus(QAbstractStatus::Failed);
        quint32 retryCount = 0;

        while (registerStatus.commandStatus() != QAbstractStatus::Succeed
               && retryCount++ < QAbstractCommand::RetryCountLimitation) {
            registerStatus.setCommandStatus(unregisterSingleDevice(*iter));

            if (registerStatus.commandStatus() != QAbstractStatus::Succeed) {
                qprofileDebug(QtWarningMsg) << "Failed to unregister Device, status = " <<
                                            registerStatus.commandStatus() << "Retry count = " << retryCount;

                if (!registerStatus.canRecover()) {
                    return registerStatus.toVariant();
                }
            }
        }

        if (registerStatus.commandStatus() != QAbstractStatus::Succeed) {
            qprofileDebug(QtWarningMsg) << "Failed to unregister device, status =  " <<
                                        registerStatus.commandStatus();
            break;
        }
    }

    return registerStatus.toVariant();
}


void QRegister::onUnregister(QAbstractBinMsg *message)
{
    QMsgRegisterResult *registerResult = qobject_cast<QMsgRegisterResult *>
                                         (message);

    if (registerResult != nullptr) {
        qprofileDebug(QtDebugMsg) << registerResult->toJson();
    }
}

const QAbstractStatus::CommandStatus QRegister::unregisterSingleDevice(
    const QVariant deviceVar)
{
    QDeviceInfo deviceInfo;
    deviceInfo.fromVariant(deviceVar);

    if (!deviceInfo.isChassis()) {
        //only unregister chassis
        return QAbstractStatus::Succeed;
    }

    QProcessMessageCommand command(deviceVar, this, false);
    QAbstractBinMsg *pUnregisterMessage = nullptr;
    QMsgUnregistTargetLongId msgUnregistTargetLongId;
    QMsgUnregistTarget msgUnregistTarget;

    if (isCompatible(QRserverCompatibility503::FeatureLongTechId)) {
        pUnregisterMessage = &msgUnregistTargetLongId;
    } else {
        pUnregisterMessage = &msgUnregistTarget;
    }

    pUnregisterMessage->fromVariant(deviceVar);
    pUnregisterMessage->fromVariant(model()->toVariant());
    command.appendMessage(0, pUnregisterMessage);
    QGenericBinMessage unregisterResultMessage(rserver::eUnRegistResult);
    command.appendMessage(pUnregisterMessage->id(), &unregisterResultMessage);

    if (0 == publish(command.topic(), command.toJson())) {
        qprofileDebug(QtWarningMsg) << "publish command failed!";
        return QAbstractStatus::IPCError;
    } else {
        QTimerStatusWatcher watcher(this, QClientModel::RserverClientStatusTopic,
                                    command.id(), QRegisterModel::UnregisterSingleDeviceTimeout);

        if (watcher.exec() == QTimerStatusWatcher::Timeout) {
            qprofileDebug(QtWarningMsg) << "Unregister single device timeout ";
            return QAbstractStatus::Timeout;
        }

        QProcessMessageStatus status(watcher.status());
        return status.commandStatus();
    }
}

const QVariant QRegister::checkDevice(const QVariant argsVar)
{
    const QRserverModel &rserverModel = m_registerModel->rserverModel();
    QVariantList deviceList = rserverModel.deviceList();
    QVariantList::const_iterator iter;
    m_registerModel->fromVariant(argsVar);
    qprofileDebug(QtDebugMsg) << argsVar;
    QRegisterStatus registerStatus;

    for (iter = deviceList.constBegin(); iter != deviceList.constEnd(); ++iter) {
        registerStatus.setCommandStatus(QAbstractStatus::Failed);
        quint32 retryCount = 0;

        while (registerStatus.commandStatus() != QAbstractStatus::Succeed
               && retryCount++ < QAbstractCommand::RetryCountLimitation) {
            registerStatus.setCommandStatus(checkSingleDevice(*iter));

            if (registerStatus.commandStatus() != QAbstractStatus::Succeed) {
                if (!registerStatus.canRecover()) {
                    qprofileDebug(QtWarningMsg) <<
                                                "Failed to check Device(can not recover), status = " <<
                                                registerStatus.commandStatus();
                    return registerStatus.toVariant();
                }

                qprofileDebug(QtWarningMsg) << "Failed to check Device, status = " <<
                                            registerStatus.commandStatus() << "Retry count = " << retryCount;
            }
        }

        if (registerStatus.commandStatus() != QAbstractStatus::Succeed) {
            qprofileDebug(QtWarningMsg) << "Failed to check device, status =  " <<
                                        registerStatus.commandStatus();
            break;
        }
    }

    registerStatus.setCompatibilityClassName(
        QAbstractCompatibility::GetCompatibilityClassName());
    return registerStatus.toVariant();
}

const QAbstractStatus::CommandStatus QRegister::checkSingleDevice(
    const QVariant deviceVar)
{
    QProcessMessageCommand command(deviceVar, this, false);
    QDeviceInfo deviceInfo;
    deviceInfo.fromVariant(deviceVar);

    if (!deviceInfo.isChassis()) {
        //        QMsgGetSoftwareByType getVersionByTypeMsg;
        //        getVersionByTypeMsg.setSerialNumber(deviceInfo.serialNumber());
        //        getVersionByTypeMsg.setDeviceType(deviceInfo.deviceType());
        //        command.appendMessage(0, &getVersionByTypeMsg);
        //        QGenericBinMessage succeedMsg(rserver::eSendCmdSucceed);
        //        command.appendMessage(0, &succeedMsg);
        QMsgDeviceInfo deviceInfoMsg;
        deviceInfoMsg.setDeviceType(deviceInfo.deviceType());
        command.appendMessage(0, &deviceInfoMsg);
        QGenericBinMessage verifyResult(rserver::eVerifyResult);
        command.appendMessage(0, &verifyResult);
        QGenericBinMessage getSoftwareVersionMsg(rserver::eGetSoftwareVer);
        command.appendMessage(0, &getSoftwareVersionMsg);
        QGenericBinMessage softwareVersionMsg(rserver::eSoftwareVersion);
        command.appendMessage(0, &softwareVersionMsg);
    } else {
        if (isCompatible(QRserverCompatibility509::FeatureTestsetStatus)) {
            QMsgTestsetStatus testsetStatusMsg;
            testsetStatusMsg.setSerialNumber(deviceInfo.serialNumber());
            command.appendMessage(0, &testsetStatusMsg);
            QGenericBinMessage succeedMsg(rserver::eSendCmdSucceed);
            command.appendMessage(testsetStatusMsg.id(), &succeedMsg);
        }

        QMsgCheckAll *pCheckAllSendMessage = nullptr;
        QMsgCheckAllLongId msgCheckAllLongId;
        QMsgCheckAll msgCheckAll;

        if (isCompatible(QRserverCompatibility503::FeatureLongTechId)) {
            pCheckAllSendMessage = &msgCheckAllLongId;
        } else {
            pCheckAllSendMessage = &msgCheckAll;
        }

        pCheckAllSendMessage->setDeviceType(deviceInfo.deviceType());
        pCheckAllSendMessage->setSerialNumber(deviceInfo.serialNumber());
        pCheckAllSendMessage->setVersion(deviceInfo.version());
        pCheckAllSendMessage->setMac(deviceInfo.mac());
        pCheckAllSendMessage->setCompany(m_registerModel->company());
        pCheckAllSendMessage->setTechId(m_registerModel->techId());
        pCheckAllSendMessage->setTechName(m_registerModel->techName());
        pCheckAllSendMessage->setSupervisorId(m_registerModel->supervisorId());
        command.appendMessage(0, pCheckAllSendMessage);
        QMsgVerifyResult checkAllReceiveMessage;

        if (isCompatible(QRserverCompatibility503::FeatureLongTechId)) {
            checkAllReceiveMessage.setType(rserver::eCheckAll2);
        } else {
            checkAllReceiveMessage.setType(rserver::eCheckAll);
        }

        checkAllReceiveMessage.setStatus(rserver::NOT_REGISTER);
        command.appendMessage(pCheckAllSendMessage->id(), &checkAllReceiveMessage);
        const QRserverModel &rserverModel = m_registerModel->rserverModel();
        QMsgUpdateVersionInfo updateVersionMsg;
        updateVersionMsg.setSerialNumber(deviceInfo.serialNumber());
        updateVersionMsg.setDeviceList(rserverModel.deviceList());
        command.appendMessage(checkAllReceiveMessage.id(), &updateVersionMsg);
        QGenericBinMessage succeedMsg(rserver::eSendCmdSucceed);
        command.appendMessage(updateVersionMsg.id(), &succeedMsg);
        checkAllReceiveMessage.setStatus(rserver::REGISTER_NOT_AUTHORIZE);
        command.appendMessage(pCheckAllSendMessage->id(), &checkAllReceiveMessage);
        command.appendMessage(checkAllReceiveMessage.id(), &updateVersionMsg);
        command.appendMessage(updateVersionMsg.id(), &succeedMsg);
        //only RMA and registered test set will return sn
        checkAllReceiveMessage.setSerialNumber(deviceInfo.serialNumber());
        checkAllReceiveMessage.setStatus(rserver::REGISTER_RMA);
        command.appendMessage(pCheckAllSendMessage->id(), &checkAllReceiveMessage);
        command.appendMessage(checkAllReceiveMessage.id(), &updateVersionMsg);
        command.appendMessage(updateVersionMsg.id(), &succeedMsg);
        checkAllReceiveMessage.setStatus(rserver::REGISTER_AUTHORIZE);
        command.appendMessage(pCheckAllSendMessage->id(), &checkAllReceiveMessage);
        command.appendMessage(checkAllReceiveMessage.id(), &updateVersionMsg);
        command.appendMessage(updateVersionMsg.id(), &succeedMsg);

        if (rserverModel.supportProfile()) {
            QGenericBinMessage profileInfoMessage(rserver::eGetProfileInfo);
            command.appendMessage(succeedMsg.id(), &profileInfoMessage);
            /* If there is no profile on server, eGetProfileInfo returned */
            QMsgGetProfileInfo profileInfoReceiveMessage;
            command.appendMessage(profileInfoMessage.id(), &profileInfoReceiveMessage);
            QGenericBinMessage profileInfoEndMessage(rserver::eProfileInfoEnd);
            command.appendMessage(profileInfoMessage.id(), &profileInfoEndMessage);
        }

        QMsgDeviceBondInfo deviceBondMsg;
        deviceBondMsg.setDeviceList(rserverModel.deviceList());
        command.appendMessage(0, &deviceBondMsg);
        command.appendMessage(deviceBondMsg.id(), &succeedMsg);
    }

    if (0 == publish(command.topic(), command.toJson())) {
        qprofileDebug(QtWarningMsg) << "publish command failed!";
        return QRegisterStatus::IPCError;
    } else {
        QTimerStatusWatcher watcher(this, QClientModel::RserverClientStatusTopic,
                                    command.id(), QRegisterModel::CheckSingleDeviceTimeout);

        if (watcher.exec() == QTimerStatusWatcher::Timeout) {
            qprofileDebug(QtWarningMsg) << "Check single device timeout ";
            return QRegisterStatus::Timeout;
        }

        QProcessMessageStatus status(watcher.status());
        return status.commandStatus();
    }
}

const QVariant QRegister::updateTechName(const QVariant argsVar)
{
    const QRserverModel &rserverModel = m_registerModel->rserverModel();
    QVariant deviceVar = rserverModel.getChassis();
    m_registerModel->fromVariant(argsVar);
    QRegisterStatus registerStatus;
    registerStatus.setCommandStatus(QAbstractStatus::Failed);
    m_updataTechNameResult = false;
    quint32 retryCount = 0;

    while (registerStatus.commandStatus() != QAbstractStatus::Succeed
           && retryCount++ < QAbstractCommand::RetryCountLimitation) {
        registerStatus.setCommandStatus(sendUpdateTechNameMsg(deviceVar));

        if (registerStatus.commandStatus() != QAbstractStatus::Succeed) {
            qprofileDebug(QtWarningMsg) << "Failed to update tech name, status = " <<
                                        registerStatus.commandStatus() << "Retry count = " << retryCount;

            if (!registerStatus.canRecover()) {
                return registerStatus.toVariant();
            }
        }
    }

    return registerStatus.toVariant();
}

const QAbstractStatus::CommandStatus QRegister::sendUpdateTechNameMsg(
    const QVariant deviceVar)
{
    QDeviceInfo chassis;
    chassis.fromVariant(deviceVar);
    QProcessMessageCommand command(deviceVar, this, false);
    QMsgUpdateTechName msgUpdateTechName;
    msgUpdateTechName.setSerialNumber(chassis.serialNumber());
    msgUpdateTechName.setTechName(m_registerModel->techName());
    command.appendMessage(0, &msgUpdateTechName);
    QMsgUpdateTechNameResult updateTechNameResult;
    updateTechNameResult.setUpdataTechNameResult(true);
    command.appendMessage(msgUpdateTechName.id(), &updateTechNameResult);
    updateTechNameResult.setUpdataTechNameResult(false);
    command.appendMessage(msgUpdateTechName.id(), &updateTechNameResult);

    if (0 == publish(command.topic(), command.toJson())) {
        qprofileDebug(QtWarningMsg) << "publish command failed!";
        return QRegisterStatus::IPCError;
    } else {
        QTimerStatusWatcher watcher(this, QClientModel::RserverClientStatusTopic,
                                    command.id(), QRegisterModel::UpdateTechNameTimeout);

        if (watcher.exec() == QTimerStatusWatcher::Timeout) {
            qprofileDebug(QtWarningMsg) << "Update Tech Name timeout ";
            return QRegisterStatus::Timeout;
        }

        if (watcher.status() == QAbstractStatus::Succeed) {
            if (!m_updataTechNameResult) {
                return (QAbstractStatus::CommandStatus)QRegisterStatus::UptadeTechNameFailed;
            }
        }

        QProcessMessageStatus status(watcher.status());
        return status.commandStatus();
    }
}

void QRegister::updateTechNameResult(QAbstractBinMsg *message)
{
    QMsgUpdateTechName *updateTechNameResult =
        qobject_cast<QMsgUpdateTechName *>
        (message);

    if (updateTechNameResult == nullptr) {
        qprofileDebug(QtWarningMsg) << "Got invalid message " << message->toJson();
        return;
    }

    m_updataTechNameResult = updateTechNameResult->updataTechNameResult();
    const QVConnectModel *pVConnectModel =
        QModelFactory::constModel<QVConnectModel>(this);

    if (QVConnectModel::ModeVConnect == pVConnectModel->rserverMode()
        && m_updataTechNameResult) {
        QProcessMessageCommand command(QVariant(), this, false);
        QMsgUpdateVConnectInfo sendVConnectInfo;
        sendVConnectInfo.fromVariant(pVConnectModel->toVariant());
        //vconnect app use this value to verify the register status of the device.
        command.appendMessage(0, &sendVConnectInfo);
        QGenericBinMessage succeedMsg(rserver::eSendCmdSucceed);
        command.appendMessage(sendVConnectInfo.id(), &succeedMsg);
        command.setTopic(QVconnectClientModel::RserverVconnectCommandTopic);

        if (0 == publish(command.topic(), command.toJson())) {
            qprofileDebug(QtWarningMsg) << "Update vonnect info failed!";
        }
    }
}

const QVariant QRegister::upateDeviceInfo(const QVariant argsVar)
{
    m_updateDeviceInfoStatus = QMsgUpdateDeviceInfo::Failed;
    const QRserverModel &rserverModel = m_registerModel->rserverModel();
    QVariant chassis = rserverModel.getChassis();
    m_registerModel->fromVariant(argsVar);
    qprofileDebug(QtDebugMsg) << argsVar;
    QRegisterStatus registerStatus;
    registerStatus.setCommandStatus(QAbstractStatus::Failed);
    quint32 retryCount = 0;

    while (registerStatus.commandStatus() != QAbstractStatus::Succeed
           && retryCount++ < QAbstractCommand::RetryCountLimitation) {
        registerStatus.setCommandStatus(upateChassisDeviceInfo(chassis));

        if (registerStatus.commandStatus() != QAbstractStatus::Succeed) {
            qprofileDebug(QtWarningMsg) << "Failed to update chassis info, status = " <<
                                        registerStatus.commandStatus() << "Retry count = " << retryCount;

            if (!registerStatus.canRecover()) {
                return registerStatus.toVariant();
            }
        }
    }

    return registerStatus.toVariant();
}

const QAbstractStatus::CommandStatus QRegister::upateChassisDeviceInfo(
    const QVariant deviceVar)
{
    QDeviceInfo deviceInfo;
    deviceInfo.fromVariant(deviceVar);

    if (!deviceInfo.isChassis()) {
        //only update chassis
        return QAbstractStatus::Failed;
    }

    //todo all of the message have the variable like tech_id should be input
    QProcessMessageCommand command(deviceVar, this, false);
    QMsgUpdateDeviceInfo *pUpdateMessage = nullptr;
    QMsgUpdateDeviceInfoLongId msgUpdateDeviceInfoLongId;
    QMsgUpdateDeviceInfo msgUpdateDeviceInfo;

    if (isCompatible(QRserverCompatibility503::FeatureLongTechId)) {
        pUpdateMessage = &msgUpdateDeviceInfoLongId;
    } else {
        pUpdateMessage = &msgUpdateDeviceInfo;
    }

    pUpdateMessage->fromVariant(deviceVar);
    pUpdateMessage->fromVariant(model()->toVariant());
    command.appendMessage(0, pUpdateMessage);
    QGenericBinMessage updateAckMessage;
    updateAckMessage.setType(rserver::eUpdateDeviceInfo);
    command.appendMessage(pUpdateMessage->id(), &updateAckMessage);
    const QRserverModel &rserverModel = m_registerModel->rserverModel();

    if (rserverModel.deviceCount() > 1) {
        QMsgUpdateVersionInfo updateVersionMsg;
        updateVersionMsg.setSerialNumber(deviceInfo.serialNumber());
        updateVersionMsg.setDeviceList(rserverModel.deviceList());
        command.appendMessage(0, &updateVersionMsg);
        QGenericBinMessage succeedMsg(rserver::eSendCmdSucceed);
        command.appendMessage(updateVersionMsg.id(), &succeedMsg);
    }

    if (0 == publish(command.topic(), command.toJson())) {
        qprofileDebug(QtWarningMsg) << "publish command failed!";
        return QAbstractStatus::IPCError;
    } else {
        QTimerStatusWatcher watcher(this, QClientModel::RserverClientStatusTopic,
                                    command.id(), QRegisterModel::UpdateDeviceInfoTimeout);

        if (watcher.exec() == QTimerStatusWatcher::Timeout) {
            qprofileDebug(QtWarningMsg) << "Update chassis info timeout ";
            return QAbstractStatus::Timeout;
        }

        if (watcher.status() == QAbstractStatus::Succeed) {
            if (m_updateDeviceInfoStatus == QMsgUpdateDeviceInfo::Failed) {
                return QAbstractStatus::Failed;
            }
        }

        QProcessMessageStatus status(watcher.status());
        return status.commandStatus();
    }
}

bool QRegister::initPropertiesMap(const QVariantMap &properties)
{
    if (properties.count() <= 0) {
        setProperty("tagName", "qregister");
    }

    return QRserverContainer::initPropertiesMap(properties);
}

void QRegister::allocModel()
{
    setModel(m_registerModel = QModelFactory::construct<QRegisterModel>(this));
    connect(m_registerModel.data(), SIGNAL(serverIpChanged()), this,
            SLOT(serverIpChanged()));
}

void QRegister::serverIpChanged()
{
    QAbstractCompatibility::SetInstance(new QRserverCompatibility());
}

void QRegister::updateRegistrationBomb(QAbstractBinMsg *message)
{
    QMsgRegistrationLock *registrationLock = qobject_cast<QMsgRegistrationLock *>
            (message);
    qprofileDebug(QtDebugMsg) << message->toJson();

    if (registrationLock == nullptr) {
        qprofileDebug(QtWarningMsg) << "Got invalid message " << message->toJson();
        return;
    }

    if (isNeedSettingAdminProfile()) {
        m_registerModel->setAdminSettingProfile(registrationLock->toVariant());
    }
}

void QRegister::updateLockVeexpressUpgrade(QAbstractBinMsg *message)
{
    QMsgLockVeexpressUpgrade *lockVeexpressUpgrade =
        qobject_cast<QMsgLockVeexpressUpgrade *>(message);

    if (lockVeexpressUpgrade == nullptr) {
        qprofileDebug(QtWarningMsg) << "Got invalid message " << message->toJson();
        return;
    }

    if (isNeedSettingAdminProfile()) {
        m_registerModel->setAdminSettingProfile(lockVeexpressUpgrade->toVariant());
    }
}

void QRegister::updateBlockProfileAndChangeTechName(QAbstractBinMsg *message)
{
    QMsgBlockProfileAndChangeTechName *blockProfileAndChangeTechName =
        qobject_cast<QMsgBlockProfileAndChangeTechName *>(message);

    if (blockProfileAndChangeTechName == nullptr) {
        qprofileDebug(QtWarningMsg) << "Got invalid message " << message->toJson();
        return;
    }

    if (isNeedSettingAdminProfile()) {
        m_registerModel->setAdminSettingProfile(
            blockProfileAndChangeTechName->toVariant());
    }
}

void QRegister::updateCheckAllInfo(QAbstractBinMsg *message)
{
    QMsgCheckAll *checkAllInfo = qobject_cast<QMsgCheckAll *>(message);

    if (checkAllInfo != nullptr) {
        qprofileDebug(QtDebugMsg) << checkAllInfo->toJson();
        const QRserverModel &rserverModel = m_registerModel->rserverModel();
        QDeviceInfo deviceInfo;
        deviceInfo.fromVariant(rserverModel.getChassis());

        if (deviceInfo.serialNumber() == checkAllInfo->serialNumber()) {
            model()->fromVariant(
                checkAllInfo->toVariant());//register model must be updated even in stolen status

            if (isNeedSettingAdminProfile()) {
                m_registerModel->setAdminSettingProfile(checkAllInfo->toVariant());
            } else { //not registered status should be force updated
                QVariantMap statusMap;
                statusMap["status"] = checkAllInfo->status();
                m_registerModel->setAdminSettingProfile(QVariant::fromValue(statusMap));
            }
        }
    }
}

void QRegister::updateDeviceInfoStatus(QAbstractBinMsg *message)
{
    QMsgUpdateDeviceInfo *updateDeviceInfo = qobject_cast<QMsgUpdateDeviceInfo *>
            (message);
    qprofileDebug(QtDebugMsg) << message->toJson();

    if (updateDeviceInfo != nullptr) {
        m_updateDeviceInfoStatus = updateDeviceInfo->updateStatus();
    }
}

void QRegister::processMessage(QAbstractBinMsg *message)
{
    Q_ASSERT(message != nullptr);

    switch (message->type()) {
    case rserver::eCheckAll:
    case rserver::eCheckAll2:
        updateCheckAllInfo(message);
        break;

    case rserver::eCheckAllExt:
        updateRegistrationBomb(message);
        break;

    case rserver::eCheckAllExt2:
        updateLockVeexpressUpgrade(message);
        break;

    case rserver::eCheckAllExt3:
        updateBlockProfileAndChangeTechName(message);
        break;

    case rserver::eUpdateTechName:
        updateTechNameResult(message);
        break;

    case rserver::eUpdateDeviceInfo:
    case rserver::eUpdateDeviceInfo2:
        updateDeviceInfoStatus(message);
        break;

    case rserver::eUnRegistResult:
        onUnregister(message);
        break;

    default:
        qprofileDebug(QtDebugMsg) << message->toJson();
        break;
    }
}

const QVariant QRegister::getServerVersion(const QVariant argsVar)
{
    const QRserverModel &rserverModel = m_registerModel->rserverModel();
    QVariantList deviceList = rserverModel.deviceList();
    QVariantList::const_iterator iter;
    m_registerModel->fromVariant(argsVar);
    qprofileDebug(QtDebugMsg) << argsVar;
    QDeviceInfo deviceInfo;
    QRegisterStatus registerStatus;
    registerStatus.setCommandStatus(QAbstractStatus::Failed);

    for (iter = deviceList.constBegin(); iter != deviceList.constEnd(); ++iter) {
        deviceInfo.fromVariant(*iter);

        if (!deviceInfo.isChassis()) {
            continue;
        }

        QProcessMessageCommand command(*iter, this, false);
        // QMsgGetServerInfo will get both QMsgGetServerInfo and QMsgServerVersion back after 4.0.5
        QMsgGetServerInfo serverInfoGet;
        command.appendMessage(0, &serverInfoGet);
        QGenericBinMessage serverInfoRecv(rserver::eGetServerInfo);
        command.appendMessage(serverInfoGet.id(), &serverInfoRecv);
        quint32 retryCount = 0;

        while (registerStatus.commandStatus() != QAbstractStatus::Succeed
               && retryCount++ < QAbstractCommand::RetryCountLimitation) {
            if (0 == publish(command.topic(), command.toJson())) {
                qprofileDebug(QtWarningMsg) << "publish command failed!";
                registerStatus.setCommandStatus(QRegisterStatus::IPCError);
            } else {
                QTimerStatusWatcher watcher(this, QClientModel::RserverClientStatusTopic,
                                            command.id(), QRegisterModel::GetServerVersionTimeout);

                if (watcher.exec() == QTimerStatusWatcher::Timeout) {
                    qprofileDebug(QtWarningMsg) << "Get server version timeout ";
                    registerStatus.setCommandStatus(QRegisterStatus::Timeout);
                } else {
                    QProcessMessageStatus status(watcher.status());
                    registerStatus.setCommandStatus(status.commandStatus());
                }
            }

            if (!registerStatus.canRecover()) {
                return registerStatus.toVariant();
            }
        }

        return registerStatus.toVariant();
    }

    return registerStatus.toVariant();
}

bool QRegister::isNeedSettingAdminProfile()
{
    const QRserverModel &rserverModel = m_registerModel->rserverModel();
    QDeviceInfo::TestsetStatus testsetStatus = QRserverModel::GetTestsetStatus(
                rserverModel);
    return ((testsetStatus == QDeviceInfo::Approved)
            || (testsetStatus == QDeviceInfo::Loaned));
}
