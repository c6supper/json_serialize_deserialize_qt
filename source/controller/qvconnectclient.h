#ifndef QVCONNECTCLIENT_H
#define QVCONNECTCLIENT_H
#include "qrserverclient.h"
#include "qvconnectclientmodel.h"

namespace QRserver
{
    class QVconnectClient: public QRserverClient
    {
            Q_OBJECT

        public:
            QVconnectClient(QObject *parent = nullptr);
            ~QVconnectClient();

        protected slots:
            virtual bool initPropertiesMap(const QVariantMap &properties =
                                               QVariantMap());
            virtual void allocModel();
        protected:
            virtual void allocTopic();
            virtual const QString getServerIP();

        private:
            QPointer<QVconnectClientModel>				m_vconnectClientModel;
    };
}


#endif /* QVCONNECTCLIENT_H */
