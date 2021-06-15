#include "qcompatibilityfactory.h"
#include "qrservercompatibility518.h"

using namespace QRserver;

QCompatibilitySelfRegisteration<QRserverCompatibility518>
registerQRserverCompatibility518(518);

const QString QRserverCompatibility518::FeatureUpdateTechName =
    "updateTechName";
const QString QRserverCompatibility518::FeatureUploadProfileInfoList =
    "uploadProfileInfoList";

QRserverCompatibility518::QRserverCompatibility518(const QString version,
        QObject *parent)
    : QRserverCompatibility510(version, parent)
{
}

QRserverCompatibility518::QRserverCompatibility518(const
        QRserverCompatibility518
        &compatibility)
    : QRserverCompatibility510(compatibility.version(), compatibility.parent())
{
}

QRserverCompatibility518::~QRserverCompatibility518()
{
}

bool QRserverCompatibility518::updateTechName() const
{
    return true;
}

bool QRserverCompatibility518::uploadProfileInfoList() const
{
    return true;
}
