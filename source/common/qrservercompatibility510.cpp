#include "qcompatibilityfactory.h"
#include "qrservercompatibility510.h"

using namespace QRserver;

QCompatibilitySelfRegisteration<QRserverCompatibility510>
registerQRserverCompatibility510(510);

const QString QRserverCompatibility510::FeatureUploadProfileAck =
    "uploadProfileAck";

QRserverCompatibility510::QRserverCompatibility510(const QString version,
        QObject *parent)
    : QRserverCompatibility509(version, parent)
{
}

QRserverCompatibility510::QRserverCompatibility510(const
        QRserverCompatibility510
        &compatibility)
    : QRserverCompatibility509(compatibility.version(), compatibility.parent())
{
}

QRserverCompatibility510::~QRserverCompatibility510()
{
}

bool QRserverCompatibility510::uploadProfileAck() const
{
    return true;
}
