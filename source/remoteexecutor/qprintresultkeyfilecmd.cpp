
#include "qprintresultkeyfilecmd.h"
#include "qcmdfactory.h"
#include "qrserverresult.h"

using namespace QProfile;
using namespace QRserver;

QCMDSelfRegisteration<QPrintResultKeyFileCMD>
registerQPrintResultKeyFileCMD("QPrintResultKeyFileCMD");

QPrintResultKeyFileCMD::QPrintResultKeyFileCMD(QObject *parent)
    : QAbstractRemoteCommand(parent)
{
}

QPrintResultKeyFileCMD::QPrintResultKeyFileCMD(const QPrintResultKeyFileCMD
        &cmd)
    : QAbstractRemoteCommand(cmd)
{
}

bool QPrintResultKeyFileCMD::isValid()
{
    return true;
}

QPrintResultKeyFileCMD::~QPrintResultKeyFileCMD()
{
}

const QVariant QPrintResultKeyFileCMD::toExecInput()
{
    return QRserverResult::GetResultKeyFileVar();
}
