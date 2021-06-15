
#include "qcompatibilityfactory.h"
#include "qrservercompatibility500.h"

using namespace QRserver;

QCompatibilitySelfRegisteration<QRserverCompatibility500>
registerQRserverCompatibility500(500);

const QString QRserverCompatibility500::FeatureGetSWVersionByType =
    "getSWVersionByType";

QRserverCompatibility500::QRserverCompatibility500(const QString version,
        QObject *parent)
    : QRserverCompatibility405(version, parent)
{
}

QRserverCompatibility500::QRserverCompatibility500(const
        QRserverCompatibility500
        &compatibility)
    : QRserverCompatibility405(compatibility.version(), compatibility.parent())
{
}

QRserverCompatibility500::~QRserverCompatibility500()
{
}

bool QRserverCompatibility500::getSWVersionByType() const
{
    return true;
}

