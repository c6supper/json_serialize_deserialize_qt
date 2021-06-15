
#include "qgetdeviceserverstatuscmd.h"
#include "qcmdfactory.h"
#include "qpongstatus.h"
#include "qabstractexecutor.h"
#include "qgenericcommand.h"
#include "qrservermodel.h"

using namespace QProfile;
using namespace QRserver;

QCMDSelfRegisteration<QGetDeviceServerStatusCMD>
registerQGetDeviceServerStatusCMD("QGetDeviceServerStatusCMD");

QGetDeviceServerStatusCMD::QGetDeviceServerStatusCMD(QObject *parent)
    : QAbstractRemoteCommand(parent)
{
}

QGetDeviceServerStatusCMD::QGetDeviceServerStatusCMD(const
        QGetDeviceServerStatusCMD &cmd)
    : QAbstractRemoteCommand(cmd)
{
}

QGetDeviceServerStatusCMD::~QGetDeviceServerStatusCMD()
{
}

const QVariant QGetDeviceServerStatusCMD::parseExecOutput(
    const QVariant &output)
{
    QPongStatus status(output);
    qprofileDebug(QtDebugMsg) << output;

    switch ((int)status.commandStatus()) {
    case QPongStatus::NotChecked:
        exit(QAbstractExecutor::NotChecked);

    case QPongStatus::RegisteredAuthorized:
        exit(QAbstractExecutor::Succeed);

    case QPongStatus::RegisteredUnauthorized:
        exit(QAbstractExecutor::RegisteredUnauthorized);

    case QPongStatus::Unregistered:
        exit(QAbstractExecutor::Unregistered);

    default:
        exit(QAbstractExecutor::UnknownError);
    }
}

const QVariant QGetDeviceServerStatusCMD::toExecInput()
{
    QGenericCommand pingCommand(this, QRserverModel::CommandPing,
                                QRserverModel::PingTopic);
    return pingCommand.toVariant();
}
