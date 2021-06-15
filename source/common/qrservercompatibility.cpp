
#include "qcompatibilityfactory.h"
#include "qrservercompatibility.h"
#include "qserverversion.h"
#include <QTimer>

using namespace QRserver;

//any version before 4.0.5.x should be the same to the client
//only major minor patch should be consider as the key
QCompatibilitySelfRegisteration<QRserverCompatibility>
registerQRserverCompatibility(404);

const QString QRserverCompatibility::defaultServerVersion = "4.0.4.0";

QRserverCompatibility::QRserverCompatibility(const QString version,
        QObject *parent)
    : QAbstractCompatibility(parent), m_serverVersion(nullptr)
{
    m_serverVersion = new QServerVersion(version, this);
    QTimer::singleShot(0, this, SLOT(initialize()));
    qprofileDebug(QtDebugMsg) << "Create compatible module version = " << version;
}

QRserverCompatibility::QRserverCompatibility(const QRserverCompatibility
        &compatibility)
    : QAbstractCompatibility(compatibility.parent()), m_serverVersion(nullptr)
{
    m_serverVersion = new QServerVersion(compatibility.version(), this);
    QTimer::singleShot(0, this, SLOT(initialize()));
}

void QRserverCompatibility::initialize()
{
}

const QString QRserverCompatibility::version() const
{
    if (m_serverVersion != nullptr) {
        return m_serverVersion->versionString();
    }

    return QString("%1").arg(QServerVersion::invalidVersionIntger);
}

QRserverCompatibility::~QRserverCompatibility()
{
    if (m_serverVersion != nullptr) {
        delete m_serverVersion;
    }
}
