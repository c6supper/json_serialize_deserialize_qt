#ifndef QRSERVERNETWORKCHECKER_H
#define QRSERVERNETWORKCHECKER_H

#include <QObject>
#include <QTimer>
#include <QThread>
#include <QMutexLocker>
#include "qprofile_global.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QRserver
{
    class QRserverNetworkChecker : public QThread
    {
            Q_OBJECT

        public:
            QRserverNetworkChecker(QObject *parent, bool singleShot = true);
            ~QRserverNetworkChecker();

            static QRserverNetworkChecker *Instance();
            bool checkConnective(const QString &hostName, const quint16 port);
            bool connected();
            bool isVaild();
            void stop();

        private:
            static const quint32 SocketTimeout = 6 * 1000;
            static const quint32 CheckNetworkInterval = 15 * 1000;

        protected:
            virtual void run();

        signals:
            void networkError();

        public slots:
            void onNetworkParamChanged(const QString &ipaddr, const quint16 port);
            void checkNetwork();

        private:
            static QRserverNetworkChecker *m_instance;
            QString m_destHost;
            quint16 m_destPort;
            QTimer m_timer;
            QMutex m_mutex;
            bool	m_singleShot;
            quint32	m_disconnectCounter;
    };
}

#endif // QRSERVERNETWORKCHECKER_H
