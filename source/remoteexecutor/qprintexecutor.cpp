
#include "qprintexecutor.h"
#include "qexecutorfactory.h"
#include <QMetaProperty>
#include <QCoreApplication>
#include <QTime>
#include "qeventprogress.h"
#include "qabstractstatus.h"
#include "qrserverstatuswatcher.h"
#include "qabstractstatus.h"
#include "unistd.h"
#if defined(BUILD_X86)
#include "qjson/serializer.h"
#else
#include <QJson/Serializer>
#endif
#include "qabstractcommand.h"
#include "qabstractremotecommand.h"

using namespace QProfile;
using namespace QRserver;

QExecutorSelfRegisteration<QPrintExecutor>
registerQPrintExecutor("QPrintExecutor");

QPrintExecutor::QPrintExecutor(QObject *parent)
    : QAbstractExecutor(parent)
{
}

QPrintExecutor::QPrintExecutor(const QPrintExecutor &executor)
    : QAbstractExecutor(executor)
{
}

int QPrintExecutor::exec(QAbstractRemoteCommand *cmd)
{
    QVariant toExecInput = cmd->toExecInput();

    if (toExecInput.isNull() || !toExecInput.isValid()) {
        return InvalidCommand;
    }

    fprintf(stdout, "%-*s", 50, "\nMeasure");
    fprintf(stdout, "%-*s", 50, "Key File Pattern\n");
    QVariantList::const_iterator i, si;
    QVariantList keyFileVarList = toExecInput.toList();;
    fprintf(stdout, "\n");

    for (i = keyFileVarList.constBegin(); i != keyFileVarList.constEnd();
         ++i) {
        QVariantList singleKeyFileVarList = (*i).toList();
        qprofileDebug(QtDebugMsg) << singleKeyFileVarList;

        for (si = singleKeyFileVarList.constBegin();
             si != singleKeyFileVarList.constEnd();
             ++si) {
            fprintf(stdout, "%-*s", 25, (*si).toString().toStdString().c_str());
        }

        fprintf(stdout, "\n");
    }

    return Succeed;
}

bool QPrintExecutor::isFinished()
{
    return true;
}

QPrintExecutor::~QPrintExecutor()
{
}

