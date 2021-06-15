#ifdef BUILD_X86
#include "qmsghandle.h"
#include "main.h"

using namespace QRserver;
QRserverUi *pRServerUi;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    qInstallMsgHandler(qMessageOutput);
    pRServerUi = new QRserverUi;
    pRServerUi->show();
    PostPoneInitializer initializer;
    initializer.start();
    return app.exec();
}
#endif
