
#include "qcompatibilityfactory.h"
#include "qrservercompatibility405.h"

using namespace QRserver;

QCompatibilitySelfRegisteration<QRserverCompatibility405>
registerQRserverCompatibility405(405);

const QString QRserverCompatibility405::FeatureLongFileName = "longFileName";

QRserverCompatibility405::QRserverCompatibility405(const QString version,
        QObject *parent)
    : QRserverCompatibility(version, parent)
{
}

QRserverCompatibility405::QRserverCompatibility405(const
        QRserverCompatibility405
        &compatibility)
    : QRserverCompatibility(compatibility.version(), compatibility.parent())
{
}

QRserverCompatibility405::~QRserverCompatibility405()
{
}

bool QRserverCompatibility405::longFileName() const
{
    return true;
}
