
#include "qcompatibilityfactory.h"
#include "qrservercompatibility503.h"

using namespace QRserver;

QCompatibilitySelfRegisteration<QRserverCompatibility503>
registerQRserverCompatibility503(503);

// FeatureLongTechId include long TechId,TechName,CompanyName
const QString QRserverCompatibility503::FeatureLongTechId =
    "longTechId";

QRserverCompatibility503::QRserverCompatibility503(const QString version,
        QObject *parent)
    : QRserverCompatibility501(version, parent)
{
}

QRserverCompatibility503::QRserverCompatibility503(const
        QRserverCompatibility503
        &compatibility)
    : QRserverCompatibility501(compatibility.version(), compatibility.parent())
{
}

QRserverCompatibility503::~QRserverCompatibility503()
{
}

bool QRserverCompatibility503::longTechId() const
{
    return true;
}

