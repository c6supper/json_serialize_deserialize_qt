#include "qrservernetworkchecker.h"
#include <QTcpSocket>

using namespace QRserver;

/*
    QRserverNetworkChecker will never be activated, just using the SIGNAL: void networkError() as a bridges to transmission signals.
*/

QRserverNetworkChecker *QRserverNetworkChecker::m_instance = nullptr;

QRserverNetworkChecker *QRserverNetworkChecker::Instance()
{
    if (m_instance == nullptr) {
        m_instance = new QRserverNetworkChecker(nullptr);
    }

    return m_instance;
}

QRserverNetworkChecker::QRserverNetworkChecker(QObject *parent, bool singleShot)
    : QThread(parent), m_destPort(0), m_singleShot(singleShot),
      m_disconnectCounter(0)
{
    m_timer.setInterval(CheckNetworkInterval);
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(checkNetwork()));
}

QRserverNetworkChecker::~QRserverNetworkChecker()
{
}

void QRserverNetworkChecker::onNetworkParamChanged(const QString &hostName,
        const quint16 port)
{
    QMutexLocker locker(&m_mutex);
    m_destHost = hostName;
    m_destPort = port;
    m_disconnectCounter = 0;

    if (isVaild() && !m_singleShot && !isRunning()) {
        start();
    }
}

bool QRserverNetworkChecker::checkConnective(const QString &hostName,
        const quint16 port)
{
    QTcpSocket socket(this);
    socket.connectToHost(hostName, port);

    if (!socket.waitForConnected(SocketTimeout)) {
        return false;
    } else {
        socket.disconnectFromHost();
    }

    return true;
}

bool QRserverNetworkChecker::isVaild()
{
    return (!m_destHost.isEmpty() && m_destPort != 0);
}

bool QRserverNetworkChecker::connected()
{
    if (!isVaild()) {
        return false;
    }

    if (!m_singleShot) {
        return m_disconnectCounter < 2;
    }

    return !m_timer.isActive();
}

void QRserverNetworkChecker::checkNetwork()
{
    QMutexLocker locker(&m_mutex);

    if (!checkConnective(m_destHost, m_destPort)) {
        if (m_disconnectCounter++ > 0) {
            if (m_singleShot) {
                m_timer.stop();
                emit networkError();
                QThread::quit();
            } else {
                if (m_disconnectCounter == 1) {
                    emit networkError();
                }
            }
        } else {
            m_timer.setInterval(CheckNetworkInterval / 2);
        }
    } else {
        m_disconnectCounter = 0;
        m_timer.setInterval(CheckNetworkInterval);
    }
}

void QRserverNetworkChecker::run()
{
    m_disconnectCounter = 0;
    m_timer.setInterval(CheckNetworkInterval);
    m_timer.start();
    QThread::exec();
}

void QRserverNetworkChecker::stop()
{
    m_timer.stop();
    QThread::quit();
}

