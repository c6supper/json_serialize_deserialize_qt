/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qprocessmessagecommand.h"
#include "qclientmodel.h"
#include "qmodelfactory.h"
#include "qmsgdeviceinfo.h"
#include "qgenericbinmessage.h"
#include "qmsgdeviceinfolongid.h"
#include "qrservercompatibility503.h"
#include "qmsgupdateversioninfo.h"

using namespace QRserver;

QVariant QProcessMessageCommand::m_bindedDevice = QVariant();

QProcessMessageCommand::QProcessMessageCommand(const QVariant device,
        QObject *parent, bool needBind)
    : QAbstractCommand(parent)
{
    setMethod(QClientModel::CommandProcessMessage);

    if (device.isValid()) {
        m_messageSeries.setDevice(device);
    } else {
        QRserverModel *model = (QRserverModel *)
                               QModelFactory::constModel<QRserverModel>(this);

        if (model != nullptr) {
            m_messageSeries.setDevice(model->getChassis());
        } else {
            qprofileDebug(QtWarningMsg) <<
                                        "Failed to get rserver model, please check your source code.";
        }
    }

    if (needBind) {
        bindDevice(m_messageSeries.device());
    }

    setTopic(QClientModel::RserverClientCommandTopic);
}

void QProcessMessageCommand::bindDevice(const QVariant &device)
{
    if (m_bindedDevice != device) {
        QMsgRegisterTarget *pDeviceInfoSendMessage = nullptr;
        QMsgDeviceInfoLongId msgDeviceInfoLongId;
        QMsgDeviceInfo msgDeviceInfo;

        if (isCompatible(QRserverCompatibility503::FeatureLongTechId)) {
            pDeviceInfoSendMessage = &msgDeviceInfoLongId;
        } else {
            pDeviceInfoSendMessage = &msgDeviceInfo;
        }

        pDeviceInfoSendMessage->fromVariant(device);
        QRegisterModel *model = (QRegisterModel *)
                                QModelFactory::constModel<QRegisterModel>(this);

        if (model != nullptr) {
            pDeviceInfoSendMessage->setCompany(model->company());
            pDeviceInfoSendMessage->setTechId(model->techId());
            pDeviceInfoSendMessage->setTechName(model->techName());
        }

        if (!m_messageSeries.appendMessage(0, pDeviceInfoSendMessage)) {
            qprofileDebug(QtWarningMsg) << "Failed to bind device = " << device;
            return;
        }

        QGenericBinMessage deviceInfoReceiveMessage(rserver::eVerifyResult);

        if (!m_messageSeries.appendMessage(pDeviceInfoSendMessage->id(),
                                           &deviceInfoReceiveMessage)) {
            qprofileDebug(QtWarningMsg) << "Failed to bind device = " << device;
            return;
        }

        const QRserverModel *pRserverModel = (QRserverModel *)
                                             QModelFactory::constModel<QRserverModel>(this);
        QMsgUpdateVersionInfo updateVersionMsg;

        if (pRserverModel != nullptr) {
            QDeviceInfo deviceInfo;
            deviceInfo.fromVariant(pRserverModel->getChassis());
            updateVersionMsg.setSerialNumber(deviceInfo.serialNumber());
            updateVersionMsg.setDeviceList(pRserverModel->deviceList());
        }

        if (!m_messageSeries.appendMessage(deviceInfoReceiveMessage.id(),
                                           &updateVersionMsg)) {
            qprofileDebug(QtWarningMsg) << "Failed to add update version info MSG";
            return;
        }

        QGenericBinMessage succeedMsg(rserver::eSendCmdSucceed);

        if (!m_messageSeries.appendMessage(updateVersionMsg.id(),
                                           &succeedMsg)) {
            qprofileDebug(QtWarningMsg) << "Failed to add update version info MSG";
            return;
        }
    }

    m_bindedDevice = device;
    setResource(m_messageSeries.toVariant());
}

void QProcessMessageCommand::RemoveBindDevice()
{
    m_bindedDevice = QVariant();
}

QProcessMessageCommand::~QProcessMessageCommand()
{
}

bool QProcessMessageCommand::isValid()
{
    return (QAbstractCommand::isValid() && m_messageSeries.isValid());
}

/*const QVariant QProcessMessageCommand::messageSeries() const
{
    return resource();
}

void QProcessMessageCommand::setMessageSeries(const QVariant &messageSeries)
{
    m_messageSeries.fromVariant(messageSeries);
    setResource(messageSeries);
}*/

bool QProcessMessageCommand::appendMessage(const quint32 parentMessageId,
        QAbstractBinMsg *message)
{
    quint32 previousIdx = parentMessageId;

    if (parentMessageId == 0) {
        previousIdx = m_messageSeries.messageIdx();
    }

    bool ok = m_messageSeries.appendMessage(previousIdx, message);

    if (ok)
        setResource(m_messageSeries.toVariant());

    return ok;
}

