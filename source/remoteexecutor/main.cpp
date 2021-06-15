#include <QApplication>
#include "qremoteexecutor.h"
#include "qmsghandle.h"
#include "qrserverenvironment.h"

using namespace QRserver;
using namespace QProfile;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv, false);
    qInstallMsgHandler(qMessageOutput);
    QAbstractEnvironment::SetInstance(new QRserverEnvironment());
    QAbstractEnvironment::PrintEnvironmentVarList();
    QAbstractEnvironment::CheckEnvironment();
    //
    QRserver::QRemoteExecutor *pExecutor = new QRserver::QRemoteExecutor();
    return pExecutor->executor();
}
