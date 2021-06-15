#include "qrserversoftwareclient.h"
#include "RserverConnecterHelper.h"
#include "qabstractbinmsg.h"

using namespace QRserver;
using namespace QProfile;

QRserverSoftwareClient::QRserverSoftwareClient(QObject *parent)
    : QObject(parent), m_rserverConnecter(new RserverConnecter())
{
}

QRserverSoftwareClient::~QRserverSoftwareClient()
{
}

bool QRserverSoftwareClient::sendCmd(QAbstractBinMsg *message)
{
    Q_ASSERT(message != nullptr);
    const QByteArray &messageArray = message->toByteArray();
    return m_rserverConnecter->sendCmd(message->type(),
                                       (quint8 *)messageArray.data(), messageArray.count());
}

bool QRserverSoftwareClient::Disconnect()
{
    return m_rserverConnecter->disconnect();
}
bool QRserverSoftwareClient::ConnectCmd(const char *IPAddress,
                                        unsigned short port, const char *device, unsigned int type)
{
    return m_rserverConnecter->ConnectCmd(IPAddress, port, device, type);
}

void QRserverSoftwareClient::registerMessageCallback(void *obj,
        rserverMessageCallback callback)
{
    m_rserverConnecter->registerMessageCallback(obj, callback);
}

