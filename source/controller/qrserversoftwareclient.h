#ifndef QRSERVERSOFTWARECLIENT_H
#define QRSERVERSOFTWARECLIENT_H

#include "qprofile_global.h"
#include <QObject>
#include <QSharedPointer>
#include "RServerClient.h"

QT_BEGIN_NAMESPACE
namespace QRserver
{
    class QAbstractBinMsg;
}
namespace rserver
{
    class RserverConnecter;
}
QT_END_NAMESPACE

using namespace rserver;

namespace QRserver
{
    class QRserverSoftwareClient: public QObject
    {
            Q_OBJECT

        public:
            QRserverSoftwareClient(QObject *parent = nullptr);
            ~QRserverSoftwareClient();

            bool Disconnect();
            bool sendCmd(QAbstractBinMsg *message);
            bool ConnectCmd(const char *IPAddress, unsigned short port, const char *device,
                            unsigned int type);

            void registerMessageCallback(void *obj, rserverMessageCallback callback);

        private:
            QSharedPointer<rserver::RserverConnecter>	m_rserverConnecter;
    };
}


#endif /* QRSERVERSOFTWARECLIENT_H */
