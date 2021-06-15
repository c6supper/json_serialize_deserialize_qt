#include "qcompatibilityfactory.h"
#include "qrservercompatibility509.h"

using namespace QRserver;

QCompatibilitySelfRegisteration<QRserverCompatibility509>
registerQRserverCompatibility509(509);

// FeatureLongTechId include long TechId,TechName,CompanyName
const QString QRserverCompatibility509::FeatureTestsetStatus =
    "testsetStatus";

QRserverCompatibility509::QRserverCompatibility509(const QString version,
        QObject *parent)
    : QRserverCompatibility503(version, parent)
{
}

QRserverCompatibility509::QRserverCompatibility509(const
        QRserverCompatibility509
        &compatibility)
    : QRserverCompatibility503(compatibility.version(), compatibility.parent())
{
}

QRserverCompatibility509::~QRserverCompatibility509()
{
}

bool QRserverCompatibility509::testsetStatus() const
{
    return true;
}

