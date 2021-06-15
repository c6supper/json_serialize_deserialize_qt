#include "qcompatibilityfactory.h"
#include "qrservercompatibility605.h"

using namespace QRserver;

QCompatibilitySelfRegisteration<QRserverCompatibility605>
registerQRserverCompatibility605(605);

const QString QRserverCompatibility605::FeatureUpgradeViaHttp =
    "upgradeViaHttp";

QRserverCompatibility605::QRserverCompatibility605(const QString version,
        QObject *parent)
    : QRserverCompatibility518(version, parent)
{
}

QRserverCompatibility605::QRserverCompatibility605(const
        QRserverCompatibility605
        &compatibility)
    : QRserverCompatibility518(compatibility.version(), compatibility.parent())
{
}

QRserverCompatibility605::~QRserverCompatibility605()
{
}

bool QRserverCompatibility605::upgradeViaHttp() const
{
    return true;
}
