#include <QApplication>
#include "qrserverengine.h"
#include "qmessagefactory.h"
#include "qmsgregistertargetext.h"
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
    QRserver::QRserverEngine *pRServer = new QRserver::QRserverEngine;
    Q_UNUSED(pRServer);
    return app.exec();
}
