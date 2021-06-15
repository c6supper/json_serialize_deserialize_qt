
#include "qgetresultcmd.h"
#include "qcmdfactory.h"
#include "qgetresultstatus.h"
#include "qgenericcommand.h"

using namespace QProfile;
using namespace QRserver;

QCMDSelfRegisteration<QGetResultCMD> registerQGetResultCMD("QGetResultCMD");

QGetResultCMD::QGetResultCMD(QObject *parent)
    : QAbstractRemoteCommand(parent)
{
}

QGetResultCMD::QGetResultCMD(const QGetResultCMD &cmd)
    : QAbstractRemoteCommand(cmd)
{
}

QGetResultCMD::~QGetResultCMD()
{
}

bool QGetResultCMD::isValid()
{
    return QAbstractRemoteCommand::isValid();
}

const QVariant QGetResultCMD::parseExecOutput(const QVariant &output)
{
    QGetResultStatus getStatus(output);
    return getStatus.vConnectResultModel();
}

const QVariant QGetResultCMD::toExecInput()
{
    QGenericCommand command(this);
    command.setTopic(inTopic());
    command.setMethod(method());
    return command.toVariant();
}
