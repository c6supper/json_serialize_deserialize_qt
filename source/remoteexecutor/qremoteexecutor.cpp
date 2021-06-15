
#include "qremoteexecutor.h"
#include "qprofile_debug.h"
#include "qmqttipcclient.h"
#include "qabstractremotecommand.h"
#include "qcmdfactory.h"
#include "qabstractexecutor.h"
#include "qexecutorfactory.h"
#include <QCoreApplication>
#include <QEventLoop>
#include <QRegExp>
#include <QStringList>
#include "qrserverversion.h"

using namespace QProfile;
using namespace QRserver;
using namespace QMQTT;

QRemoteExecutor::QRemoteExecutor(QObject *parent)
    : QObject(parent)
{
    QMqttIpcClient::Create(this);
}

QRemoteExecutor::~QRemoteExecutor()
{
}
#if 0 // TODO:
const bool QRemoteExecutor::checkCache()
{
    //clear invalid cache files.
}
#endif
const QVariant QRemoteExecutor::Argument()
{
    QStringList args = QCoreApplication::arguments();
    qprofileDebug(QtDebugMsg) << args.size() << args;

    if (args.size() <= 1) {
        fprintf(stdout, "\n=================help=================\n");
        QString version = QString("version = %1 \n").arg(RSERVER_VERSION);
        fprintf(stdout, "%s", version.toStdString().c_str());
        fprintf(stdout,
                "\nremoteexecutor --cmd='' --input='' --output='' --inTopic='' --outTopic='' --timeout='' --isCache=''\n");
        fprintf(stdout, "=======================================\n");
        return QVariant();
    }

    QRegExp rxArg(QString("--(.*)=(.*)"));
    QVariantMap argvMap;

    for (int i = 1; i < args.size(); ++i) {
        if (rxArg.indexIn(args.at(i)) != -1) {
            argvMap[rxArg.cap(1)] = rxArg.cap(2);
        }
    }

    return QVariant(argvMap);
}

int QRemoteExecutor::executor()
{
    //argu
    QVariant argu = Argument();

    if (argu.toMap().isEmpty()) {
        qprofileDebug(QtWarningMsg) << "invalid argument !";
        return QAbstractExecutor::ArgumentError;
    }

    //CMD
    QAbstractRemoteCommand *cmd = (QAbstractRemoteCommand *)QCMDFactory::create(
                                      argu);

    if (cmd == nullptr || !cmd->isValid()) {
        qprofileDebug(QtWarningMsg) <<
                                    "command is invalid,please check your arguments. argu = " << argu;

        if (cmd != nullptr) {
            qprofileDebug(QtWarningMsg) << cmd->toVariant();
        }

        fprintf(stdout, "%s\n",
                QString("{\"error\":\"Please check your arguments\"}").toStdString().c_str());
        return QAbstractExecutor::ArgumentError;
    }

    //Executor
    QAbstractExecutor *executor = (QAbstractExecutor *)QExecutorFactory::create(
                                      cmd->toVariant());

    if (executor == nullptr) {
        qprofileDebug(QtWarningMsg) << "create Executor failed, argu = " << argu;
        fprintf(stdout, "%s\n",
                QString("{\"error\":\"Please check your arguments\"}").toStdString().c_str());
        return QAbstractExecutor::ArgumentError;
    }

    return executor->exec(cmd);
}

