
#include "qcompatibilityfactory.h"
#include "qrservercompatibility501.h"

using namespace QRserver;

QCompatibilitySelfRegisteration<QRserverCompatibility501>
registerQRserverCompatibility501(501);

const QString QRserverCompatibility501::FeatureRegisterChassisOnly =
    "registerChassisOnly";

QRserverCompatibility501::QRserverCompatibility501(const QString version,
        QObject *parent)
    : QRserverCompatibility500(version, parent)
{
}

QRserverCompatibility501::QRserverCompatibility501(const
        QRserverCompatibility501
        &compatibility)
    : QRserverCompatibility500(compatibility.version(), compatibility.parent())
{
}

QRserverCompatibility501::~QRserverCompatibility501()
{
}

bool QRserverCompatibility501::registerChassisOnly() const
{
    return true;
}

