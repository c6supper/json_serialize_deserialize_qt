/*
 * qmqtt_client_p.cpp - qmqtt client private
 *
 * Copyright (c) 2013  Ery Lee <ery.lee at gmail dot com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of mqttc nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "qmqtt_client_p.h"
#include "qmqtt_message.h"
#include "qmqtt_logging_p.h"
#include "qmqtt_string_p.h"
#include <QUuid>
#ifndef QT_NO_SSL
#include <QFile>
#include <QSslConfiguration>
#include <QSslKey>
#endif // QT_NO_SSL

Q_LOGGING_CATEGORY(client, "qmqtt.client")

static const quint8 QOS0 = 0x00;
static const quint8 QOS1 = 0x01;
static const quint8 QOS2 = 0x02;

QMQTT::ClientPrivate::ClientPrivate(Client* qq_ptr)
    : _host(QHostAddress::LocalHost)
    , _port(1883)
    , _gmid(1)
    , _clientId(QUuid::createUuid().toString())
    , _cleanSession(false)
    , _connectionState(STATE_INIT)
    , _willQos(0)
    , _willRetain(false)
    , q_ptr(qq_ptr)
{
    setKeepAlive(300);
}

QMQTT::ClientPrivate::~ClientPrivate()
{
}

void QMQTT::ClientPrivate::init(const QHostAddress& host, const quint16 port, NetworkInterface* network)
{
    _host = host;
    _port = port;
    if(network == NULL)
    {
        init(new Network);
    }
    else
    {
        init(network);
    }
}

#ifndef QT_NO_SSL
void QMQTT::ClientPrivate::init(const QString& hostName, const quint16 port,
                                const QSslConfiguration &config, const bool ignoreSelfSigned)
{
    _hostName = hostName;
    _port = port;
    init(new SslNetwork(config, ignoreSelfSigned));
}
#endif // QT_NO_SSL

void QMQTT::ClientPrivate::init(const QString& hostName, const quint16 port, const bool ssl,
                                const bool ignoreSelfSigned)
{
    _hostName = hostName;
    _port = port;
    if (ssl)
    {
#ifndef QT_NO_SSL
        QSslConfiguration sslConf = QSslConfiguration::defaultConfiguration();
        QList<QSslCertificate> certs = QSslCertificate::fromPath(QStringLiteral("./cert.crt"));
        if (!certs.isEmpty())
            sslConf.setLocalCertificate(certs.first());
        QFile file(QStringLiteral("./cert.key"));
        if (file.open(QIODevice::ReadOnly)) {
            sslConf.setPrivateKey(QSslKey(file.readAll(), QSsl::Rsa));
        }
        sslConf.setPeerVerifyMode(QSslSocket::VerifyNone);
#if QT_VERSION < 0x050000
        sslConf.setProtocol(QSsl::TlsV1);
#endif
        init(hostName, port, sslConf, ignoreSelfSigned);
#else
        Q_UNUSED(ignoreSelfSigned)
        qCritical() << "SSL not supported in this QT build";
#endif // QT_NO_SSL
    }
    else
    {
        init(new Network);
    }
}

void QMQTT::ClientPrivate::init(NetworkInterface* network)
{
    Q_Q(Client);

    _network.reset(network);
    _timer.setSingleShot(true);
    _pingResponseTimer.setSingleShot(true);
    initializeErrorHash();

    QObject::connect(&_timer, SIGNAL(timeout()), q, SLOT(onTimerPingReq()));
    QObject::connect(&_pingResponseTimer, SIGNAL(timeout()), q, SLOT(onPingTimeout()));
    QObject::connect(_network.data(), SIGNAL(connected()),
                     q, SLOT(onNetworkConnected()));
    QObject::connect(_network.data(), SIGNAL(disconnected()),
                     q, SLOT(onNetworkDisconnected()));
    QObject::connect(_network.data(), SIGNAL(received(const QMQTT::Frame&)),
                     q, SLOT(onNetworkReceived(const QMQTT::Frame&)));
    QObject::connect(_network.data(), SIGNAL(error(QAbstractSocket::SocketError)),
                     q, SLOT(onNetworkError(QAbstractSocket::SocketError)));
}

void QMQTT::ClientPrivate::initializeErrorHash()
{
    _socketErrorHash.insert(QAbstractSocket::ConnectionRefusedError, SocketConnectionRefusedError);
    _socketErrorHash.insert(QAbstractSocket::RemoteHostClosedError, SocketRemoteHostClosedError);
    _socketErrorHash.insert(QAbstractSocket::HostNotFoundError, SocketHostNotFoundError);
    _socketErrorHash.insert(QAbstractSocket::SocketAccessError, SocketAccessError);
    _socketErrorHash.insert(QAbstractSocket::SocketResourceError, SocketResourceError);
    _socketErrorHash.insert(QAbstractSocket::SocketTimeoutError, SocketTimeoutError);
    _socketErrorHash.insert(QAbstractSocket::DatagramTooLargeError, SocketDatagramTooLargeError);
    _socketErrorHash.insert(QAbstractSocket::NetworkError, SocketNetworkError);
    _socketErrorHash.insert(QAbstractSocket::AddressInUseError, SocketAddressInUseError);
    _socketErrorHash.insert(QAbstractSocket::SocketAddressNotAvailableError, SocketAddressNotAvailableError);
    _socketErrorHash.insert(QAbstractSocket::UnsupportedSocketOperationError, SocketUnsupportedSocketOperationError);
    _socketErrorHash.insert(QAbstractSocket::UnfinishedSocketOperationError, SocketUnfinishedSocketOperationError);
    _socketErrorHash.insert(QAbstractSocket::ProxyAuthenticationRequiredError, SocketProxyAuthenticationRequiredError);
    _socketErrorHash.insert(QAbstractSocket::SslHandshakeFailedError, SocketSslHandshakeFailedError);
    _socketErrorHash.insert(QAbstractSocket::ProxyConnectionRefusedError, SocketProxyConnectionRefusedError);
    _socketErrorHash.insert(QAbstractSocket::ProxyConnectionClosedError, SocketProxyConnectionClosedError);
    _socketErrorHash.insert(QAbstractSocket::ProxyConnectionTimeoutError, SocketProxyConnectionTimeoutError);
    _socketErrorHash.insert(QAbstractSocket::ProxyNotFoundError, SocketProxyNotFoundError);
    _socketErrorHash.insert(QAbstractSocket::ProxyProtocolError, SocketProxyProtocolError);
#if QT_VERSION >= 0x050000
    _socketErrorHash.insert(QAbstractSocket::OperationError, SocketOperationError);
    _socketErrorHash.insert(QAbstractSocket::SslInternalError, SocketSslInternalError);
    _socketErrorHash.insert(QAbstractSocket::SslInvalidUserDataError, SocketSslInvalidUserDataError);
    _socketErrorHash.insert(QAbstractSocket::TemporaryError, SocketTemporaryError);
#endif
}

void QMQTT::ClientPrivate::connectToHost()
{
    if (_hostName.isEmpty())
    {
        _network->connectToHost(_host, _port);
    }
    else
    {
        _network->connectToHost(_hostName, _port);
    }
}

void QMQTT::ClientPrivate::onNetworkConnected()
{
    sendConnect();
}

void QMQTT::ClientPrivate::sendConnect()
{
    quint8 header = CONNECT;
    quint8 flags = 0;

    //header
    Frame frame(header);

    //flags
    flags = FLAG_CLEANSESS(flags, _cleanSession ? 1 : 0 );
    flags = FLAG_WILL(flags, willTopic().isEmpty() ? 0 : 1);
    if (!willTopic().isEmpty())
    {
        flags = FLAG_WILLQOS(flags, willQos());
        flags = FLAG_WILLRETAIN(flags, willRetain() ? 1 : 0);
    }
    if (!username().isEmpty())
    {
        flags = FLAG_USERNAME(flags, 1);
        flags = FLAG_PASSWD(flags, !password().isEmpty() ? 1 : 0);
    }

    //payload
    frame.writeString(QStringLiteral(PROTOCOL_MAGIC));
    frame.writeChar(PROTOCOL_VERSION_MAJOR);
    frame.writeChar(flags);
    frame.writeInt(keepAlive());
    frame.writeString(_clientId);
    if(!willTopic().isEmpty())
    {
        frame.writeString(willTopic());
        // According to the specs (http://docs.oasis-open.org/mqtt/mqtt/v3.1.1/os/mqtt-v3.1.1-os.html#_Toc398718031)
        // the will message gets always sent together with the topic, also when it is empty which is perfectly valid.
        frame.writeByteArray(_willMessage); 
    }
    if (!_username.isEmpty())
    {
        frame.writeString(_username);
        if (!_password.isEmpty())
        {
            frame.writeByteArray(_password);
        }
    }
    sendFrame(frame);
}

quint16 QMQTT::ClientPrivate::sendPublish(const Message &message)
{
    quint16 msgid = message.id();

    quint8 header = PUBLISH;
    header = SETRETAIN(header, message.retain() ? 1 : 0);
    header = SETQOS(header, message.qos());
    header = SETDUP(header, message.dup() ? 1 : 0);
    Frame frame(header);
    frame.writeString(message.topic());
    if(message.qos() > QOS0) {
        if (msgid == 0)
            msgid = nextmid();
        frame.writeInt(msgid);
    }
    if(!message.payload().isEmpty()) {
        frame.writeRawData(message.payload());
    }
    sendFrame(frame);
    return msgid;
}

void QMQTT::ClientPrivate::sendPuback(const quint8 type, const quint16 mid)
{
    Frame frame(type);
    frame.writeInt(mid);
    sendFrame(frame);
}

quint16 QMQTT::ClientPrivate::sendSubscribe(const QString & topic, const quint8 qos)
{
    quint16 mid = nextmid();
    Frame frame(SETQOS(SUBSCRIBE, QOS1));
    frame.writeInt(mid);
    frame.writeString(topic);
    frame.writeChar(qos);
    sendFrame(frame);
    return mid;
}

quint16 QMQTT::ClientPrivate::sendUnsubscribe(const QString &topic)
{
    quint16 mid = nextmid();
    Frame frame(SETQOS(UNSUBSCRIBE, QOS1));
    frame.writeInt(mid);
    frame.writeString(topic);
    sendFrame(frame);
    return mid;
}

void QMQTT::ClientPrivate::onTimerPingReq()
{
    Frame frame(PINGREQ);
    sendFrame(frame);
    _pingResponseTimer.start();
}

void QMQTT::ClientPrivate::onPingTimeout()
{
    Q_Q(Client);
    emit q->error(MqttNoPingResponse);
    disconnectFromHost();
}

void QMQTT::ClientPrivate::disconnectFromHost()
{
    sendDisconnect();
    _network->disconnectFromHost();
}

void QMQTT::ClientPrivate::sendDisconnect()
{
    Frame frame(DISCONNECT);
    sendFrame(frame);
}

void QMQTT::ClientPrivate::sendFrame(const Frame &frame)
{
    _network->sendFrame(frame);
    _timer.start();
}

void QMQTT::ClientPrivate::stopKeepAlive()
{
    _timer.stop();
    _pingResponseTimer.stop();
}

quint16 QMQTT::ClientPrivate::nextmid()
{
    return _gmid++;
}

quint16 QMQTT::ClientPrivate::publish(const Message& message)
{
    Q_Q(Client);
    quint16 msgid = sendPublish(message);

    // Emit published only at QOS0
    if (message.qos() == QOS0)
        emit q->published(message, msgid);
    else
        _midToMessage[msgid] = message;

    return msgid;
}

void QMQTT::ClientPrivate::puback(const quint8 type, const quint16 msgid)
{
    sendPuback(type, msgid);
}

void QMQTT::ClientPrivate::subscribe(const QString& topic, const quint8 qos)
{
    quint16 msgid = sendSubscribe(topic, qos);
    _midToTopic[msgid] = topic;
}

void QMQTT::ClientPrivate::unsubscribe(const QString& topic)
{
    quint16 msgid = sendUnsubscribe(topic);
    _midToTopic[msgid] = topic;
}

void QMQTT::ClientPrivate::onNetworkDisconnected()
{
    Q_Q(Client);

    stopKeepAlive();
    _midToTopic.clear();
    _midToMessage.clear();
    emit q->disconnected();
}

void QMQTT::ClientPrivate::onNetworkReceived(const QMQTT::Frame& frm)
{
    QMQTT::Frame frame(frm);
    quint8 qos = 0;
    bool retain, dup;
    QString topic;
    quint16 mid = 0;
    quint8 header = frame.header();
    quint8 type = GETTYPE(header);

    switch(type)
    {
    case CONNACK:
        frame.readChar();
        handleConnack(frame.readChar());
        break;
    case PUBLISH:
        qos = GETQOS(header);
        retain = GETRETAIN(header);
        dup = GETDUP(header);
        topic = frame.readString();
        if( qos > QOS0) {
            mid = frame.readInt();
        }
        handlePublish(Message(mid, topic, frame.data(), qos, retain, dup));
        break;
    case PUBACK:
    case PUBREC:
    case PUBREL:
    case PUBCOMP:
        mid = frame.readInt();
        handlePuback(type, mid);
        break;
    case SUBACK:
        mid = frame.readInt();
        topic = _midToTopic.take(mid);
        qos = frame.readChar();
        handleSuback(topic, qos);
        break;
    case UNSUBACK:
        mid = frame.readInt();
        topic = _midToTopic.take(mid);
        handleUnsuback(topic);
        break;
    case PINGRESP:
        handlePingresp();
        break;
    default:
        break;
    }
}

void QMQTT::ClientPrivate::handleConnack(const quint8 ack)
{
    Q_Q(Client);

    switch (ack)
    {
    case 0:
        emit q->connected();
        break;
    case 1:
        emit q->error(MqttUnacceptableProtocolVersionError);
        break;
    case 2:
        emit q->error(MqttIdentifierRejectedError);
        break;
    case 3:
        emit q->error(MqttServerUnavailableError);
        break;
    case 4:
        emit q->error(MqttBadUserNameOrPasswordError);
        break;
    case 5:
        emit q->error(MqttNotAuthorizedError);
        break;
    default:
        emit q->error(UnknownError);
        break;
    }
}

void QMQTT::ClientPrivate::handlePublish(const Message& message)
{
    Q_Q(Client);

    if(message.qos() == QOS1)
    {
        sendPuback(PUBACK, message.id());
    }
    else if(message.qos() == QOS2)
    {
        sendPuback(PUBREC, message.id());
    }
    emit q->received(message);
}

void QMQTT::ClientPrivate::handlePuback(const quint8 type, const quint16 msgid)
{
    Q_Q(Client);

    switch (type)
    {
    case PUBREC:
        sendPuback(SETQOS(PUBREL, QOS1), msgid);
        break;
    case PUBREL:
        sendPuback(PUBCOMP, msgid);
        break;
    case PUBACK:
    case PUBCOMP:
        // Emit published on PUBACK at QOS1 and on PUBCOMP at QOS2
        emit q->published(_midToMessage.take(msgid), msgid);
        break;
    }
}

void QMQTT::ClientPrivate::handlePingresp()
{
    // Stop the ping response timer to prevent disconnection. It will be restarted when the next
    // ping request has been sent.
    _pingResponseTimer.stop();
    Q_Q(Client);
    emit q->pingresp();
}

void QMQTT::ClientPrivate::handleSuback(const QString &topic, const quint8 qos)
{
    Q_Q(Client);
    emit q->subscribed(topic, qos);
}

void QMQTT::ClientPrivate::handleUnsuback(const QString &topic) {
    Q_Q(Client);
    emit q->unsubscribed(topic);
}

bool QMQTT::ClientPrivate::autoReconnect() const
{
    return _network->autoReconnect();
}

void QMQTT::ClientPrivate::setAutoReconnect(const bool autoReconnect)
{
    _network->setAutoReconnect(autoReconnect);
}

int QMQTT::ClientPrivate::autoReconnectInterval() const
{
    return _network->autoReconnectInterval();
}

void QMQTT::ClientPrivate::setAutoReconnectInterval(const int autoReconnectInterval)
{
    _network->setAutoReconnectInterval(autoReconnectInterval);
}

bool QMQTT::ClientPrivate::isConnectedToHost() const
{
    return _network->isConnectedToHost();
}

QMQTT::ConnectionState QMQTT::ClientPrivate::connectionState() const
{
    return _connectionState;
}

void QMQTT::ClientPrivate::setCleanSession(const bool cleanSession)
{
    _cleanSession = cleanSession;
}

bool QMQTT::ClientPrivate::cleanSession() const
{
    return _cleanSession;
}

void QMQTT::ClientPrivate::setKeepAlive(const quint16 keepAlive)
{
    // _timer will be started when a message is sent.
    _timer.setInterval(keepAlive*1000);
    // The MQTT specification does not mention a timeout value in this case, so we use 10% of the
    // keep alive interval.
    _pingResponseTimer.setInterval(qBound(keepAlive * 100, 10000, keepAlive * 1000));
}

quint16 QMQTT::ClientPrivate::keepAlive() const
{
    return _timer.interval() / 1000;
}

void QMQTT::ClientPrivate::setPassword(const QByteArray& password)
{
    _password = password;
}

QByteArray QMQTT::ClientPrivate::password() const
{
    return _password;
}

void QMQTT::ClientPrivate::setUsername(const QString& username)
{
    _username = username;
}

QString QMQTT::ClientPrivate::username() const
{
    return _username;
}

void QMQTT::ClientPrivate::setClientId(const QString& clientId)
{
    if(clientId.isEmpty())
    {
        _clientId = QUuid::createUuid().toString();
    }
    else
    {
        _clientId = clientId;
    }
}

QString QMQTT::ClientPrivate::clientId() const
{
    return _clientId;
}

void QMQTT::ClientPrivate::setPort(const quint16 port)
{
    _port = port;
}

quint16 QMQTT::ClientPrivate::port() const
{
    return _port;
}

void QMQTT::ClientPrivate::setHost(const QHostAddress& host)
{
    _host = host;
}

QHostAddress QMQTT::ClientPrivate::host() const
{
    return _host;
}

void QMQTT::ClientPrivate::setHostName(const QString& hostName)
{
    _hostName = hostName;
}

QString QMQTT::ClientPrivate::hostName() const
{
    return _hostName;
}

QString QMQTT::ClientPrivate::willTopic() const
{
    return _willTopic;
}

void QMQTT::ClientPrivate::setWillTopic(const QString& willTopic)
{
    _willTopic = willTopic;
}

quint8 QMQTT::ClientPrivate::willQos() const
{
    return _willQos;
}

void QMQTT::ClientPrivate::setWillQos(const quint8 willQos)
{
    _willQos = willQos;
}

bool QMQTT::ClientPrivate::willRetain() const
{
    return _willRetain;
}

void QMQTT::ClientPrivate::setWillRetain(const bool willRetain)
{
    _willRetain = willRetain;
}

QByteArray QMQTT::ClientPrivate::willMessage() const
{
    return _willMessage;
}

void QMQTT::ClientPrivate::setWillMessage(const QByteArray& willMessage)
{
    _willMessage = willMessage;
}

void QMQTT::ClientPrivate::onNetworkError(QAbstractSocket::SocketError socketError)
{
    Q_Q(Client);
    emit q->error(_socketErrorHash.value(socketError, UnknownError));
}
