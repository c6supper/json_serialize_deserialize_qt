/*****************************************************************************************
** Eventqueue.cpp: Implementation of the client class.
**
** Author: Leo.Tang
** Copyright (C) 2007 VeTronics(BeiJing) Ltd.
**
** Create date: 08-13-2007
*****************************************************************************************/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>

//#include "be_drv.h"
#include "eventexecuter.h"
#include "clientbase.h"

#include "qprofile_debug.h"

//receive data from server
void *ReceiveThread(void *para)
{
    ClientBase		*client = (ClientBase *)para;
    int				ret;
    unsigned char	buffer[MAX_SIZE_OF_PACKET * 2];
    unsigned int	type;
    unsigned int	datalength;
    unsigned int    readlength;
    FrameHeader	*header;
    unsigned char	*md5hashcode;
    fd_set			set;
    struct timeval	timeout;
    ret = pthread_detach(pthread_self());

    if (0 != ret) {
        qprofileDebug(QtDebugMsg) << "ClientBase(" << client->GetSocket() <<
                                  "): Detach thread failed, error:" << ret;
    }

    while (client->KeepReceiving()) {
        //receive frame header
        readlength = 0;

        while (readlength < sizeof(FrameHeader)) {
            if (!client->KeepReceiving()) {
                qprofileDebug(QtDebugMsg) << "ClientBase(" << client->GetSocket() <<
                                          "): Exit receiving thread.";
                client->StopDisconnectWaiting();
                return 0;
            }

            FD_ZERO(&set);
            FD_SET(client->GetSocket(), &set);
            timeout.tv_sec  = 0;
            timeout.tv_usec = 200 * 1000;
            //check if there is any data to read
            ret = select(client->GetSocket() + 1, &set, 0, 0, &timeout);

            if (0 == ret) { //select time out
                continue;
            }

            if (ret < 0) { //server closed
                qprofileDebug(QtDebugMsg) << "ClientBase(" << client->GetSocket() <<
                                          "): Server closed when select, Exit receiving thread.";
                client->DisconnectForcibly();
                client->StopDisconnectWaiting();
                client->RaiseServerCloseEvent();
                return 0;
            }

            ret = recv(client->GetSocket(), buffer + readlength,
                       sizeof(FrameHeader) - readlength, 0);

            //if error, break
            if ((0 == ret) || (SOCKET_ERROR == ret)) {
                qprofileDebug(QtDebugMsg) << "ClientBase(" << client->GetSocket() <<
                                          "):Failed when receiving frame header! errno =" << errno;

                if (SOCKET_ERROR == ret) {
                    qprofileDebug(QtDebugMsg) << "ClientBase(" << client->GetSocket() <<
                                              "): Socket error when receive frame header, Exit receiving thread.";
                    client->DisconnectForcibly();
                    client->StopDisconnectWaiting();
                    client->RaiseSocketErrorEvent();
                } else {
                    qprofileDebug(QtDebugMsg) << "ClientBase(" << client->GetSocket() <<
                                              "): Socket closed when receive frame header, Exit receiving thread.";
                    client->DisconnectForcibly();
                    client->StopDisconnectWaiting();
                    client->RaiseServerCloseEvent();
                }

                return 0;
            }

            readlength += ret;
        }

        header = (FrameHeader *)buffer;

        //check app flag
        if (0 != strncmp(APP_FLAG, header->AppFlag, strlen(APP_FLAG))) {
            qprofileDebug(QtDebugMsg) << "ClientBase(" << client->GetSocket() <<
                                      "): Unknown frame received.";
            continue;
        }

        //get length and type
        //	datalength = ReverseInt32(header->DataLength);
        //	type = ReverseInt32(header->Type);
        datalength = header->DataLength;
        type = header->Type;

        //check reply
        if (type == MSG_DATA_CORRECT) {
            client->DealReplyMessage(header->MD5Hash, true);
            continue;
        }

        if (type == MSG_DATA_ERROR) {
            client->DealReplyMessage(header->MD5Hash, false);
            continue;
        }

        if (type == MSG_CONNECT) {
            client->StopConnectWaiting();
            continue;
        }

        //receive data of the frame
        readlength = 0;

        while (datalength > readlength) {
            if (!client->KeepReceiving()) {
                qprofileDebug(QtDebugMsg) << "ClientBase(" << client->GetSocket() <<
                                          "):  Exit receiving thread.";
                client->StopDisconnectWaiting();
                return 0;
            }

            FD_ZERO(&set);
            FD_SET(client->GetSocket(), &set);
            timeout.tv_sec = 1;
            timeout.tv_usec = 0;
            //check if there is any data to read
            ret = select(client->GetSocket() + 1, &set, 0, 0, &timeout);

            if (0 == ret
                || (client->_executer->getQueueCount() >=
                    MAX_EVENTS_IN_QUEUE)) { //select time out or queue is full
                continue;
            }

            if (ret < 0) { //server closed
                qprofileDebug(QtDebugMsg) << "ClientBase(" << client->GetSocket() <<
                                          "): Socket closed when receive frame header, Exit receiving thread.";
                client->DisconnectForcibly();
                client->StopDisconnectWaiting();
                client->RaiseServerCloseEvent();
                return 0;
            }

            ret = recv(client->GetSocket(), buffer + sizeof(FrameHeader) + readlength,
                       datalength - readlength, 0);

            //if error, break
            if ((0 == ret) || (SOCKET_ERROR == ret)) {
                qprofileDebug(QtDebugMsg) << "ClientBase(" << client->GetSocket() <<
                                          "):Failed when receiving frame data! errno =" << errno;

                //check exitcode.
                if (SOCKET_ERROR == ret) {
                    qprofileDebug(QtDebugMsg) << "ClientBase(" << client->GetSocket() <<
                                              "): Socket error when receive frame header, Exit receiving thread.";
                    client->DisconnectForcibly();
                    client->StopDisconnectWaiting();
                    client->RaiseSocketErrorEvent();
                } else {
                    qprofileDebug(QtDebugMsg) << "ClientBase(" << client->GetSocket() <<
                                              "): Socket closed when receive frame header, Exit receiving thread.";
                    client->DisconnectForcibly();
                    client->StopDisconnectWaiting();
                    client->RaiseServerCloseEvent();
                }

                return 0;
            }

            readlength += ret;
        }

        //ok, all the data has been received, check
        md5hashcode = MDBytes((unsigned char *)(buffer + 16), datalength + 16,
                              md5hashcode);
        //get reply hashcode
        client->SetReplyHashcode(header->MD5Hash);

        if (0 == memcmp(md5hashcode, header->MD5Hash, 16)) {
            if (type == client->GetReceiveType()) {
                client->SetReceivedDataAndType((unsigned char *)(buffer + 32), datalength);
            } else {
                if (client->isDirectCall()) {
                    client->DirectCall(type, (const unsigned char *)(buffer + 32), datalength);
                } else {
                    client->Execute(type, (unsigned char *)(buffer + 32), datalength);
                }
            }

            client->Reply(true);
        } else {
            qprofileDebug(QtDebugMsg) << "ClientBase(" << client->GetSocket() <<
                                      "): Hashcode is not correct.";
            client->Reply(false);
        }

        delete[] md5hashcode;
    }

    qprofileDebug(QtDebugMsg) << "ClientBase(" << client->GetSocket() <<
                              "): Exit receiving thread.";
    client->StopDisconnectWaiting();
    return 0;
}

void ProcessSIGPIPESignal(int signo)
{
    qprofileDebug(QtDebugMsg) << "ClientBase: Socket error, broken pipe.";
}

//create a new client
ClientBase::ClientBase(int bDirectCall)
{
    //handle pipe signal
    signal(SIGPIPE, ProcessSIGPIPESignal);
    m_bDirectCall = bDirectCall;
    _executer = new EventExecuter(this);
    _socket = INVALID_SOCKET;
    _receive_thread = 0;
    _keep_running_thread = false;
    _last_sent_bytes = 0;
    _last_sent_length = 0;
    _received_data = 0;
    _received_length = 0;
    _received_type = 0;
    _reply_hashcode = 0;
    _send_succeeded = true;
    _isconnecting = false;
    _isconnected = false;
    _last_sent_time = time(0);
    pthread_mutex_init(&_last_sent_bytes_obj, 0);
    pthread_mutex_init(&_send_succeeded_obj, 0);
    pthread_mutex_init(&_keep_running_thread_obj, 0);
    pthread_mutex_init(&_reply_hashcode_obj, 0);
    pthread_mutex_init(&_isconnecting_obj, 0);
    pthread_mutex_init(&_send_obj, 0);
    pthread_mutex_init(&_receive_obj, 0);
    pthread_mutex_init(&_cond_obj, 0);
    pthread_mutex_init(&_cond_exit_obj, 0);
    pthread_mutex_init(&_cond_connect_obj, 0);
    pthread_mutex_init(&_cond_receive_obj, 0);
    pthread_cond_init(&_cond, 0);
    pthread_cond_init(&_cond_exit, 0);
    pthread_cond_init(&_cond_connect, 0);
    pthread_cond_init(&_cond_receive, 0);
}

ClientBase::~ClientBase()
{
    Disconnect();
    delete _executer;
    _executer = 0;

    if (0 != _received_data) {
        delete[] _received_data;
        _received_data = 0;
    }

    if (0 != _last_sent_bytes) {
        delete[] _last_sent_bytes ;
        _last_sent_bytes  = 0;
    }

    if (0 != _reply_hashcode) {
        delete[] _reply_hashcode;
        _reply_hashcode  = 0;
    }

    pthread_mutex_destroy(&_last_sent_bytes_obj);
    pthread_mutex_destroy(&_send_succeeded_obj);
    pthread_mutex_destroy(&_keep_running_thread_obj);
    pthread_mutex_destroy(&_reply_hashcode_obj);
    pthread_mutex_destroy(&_isconnecting_obj);
    pthread_mutex_destroy(&_send_obj);
    pthread_mutex_destroy(&_receive_obj);
    pthread_mutex_destroy(&_cond_obj);
    pthread_mutex_destroy(&_cond_exit_obj);
    pthread_mutex_destroy(&_cond_connect_obj);
    pthread_mutex_destroy(&_cond_receive_obj);
    pthread_cond_destroy(&_cond);
    pthread_cond_destroy(&_cond_exit);
    pthread_cond_destroy(&_cond_connect);
    pthread_cond_destroy(&_cond_receive);
}


int ClientBase::isDirectCall(void)
{
    return m_bDirectCall;
}

void ClientBase::DirectCall(unsigned int type, const unsigned char *data,
                            unsigned int length)
{
    OnReceived(type, data, length);
}

/* Noticed by Leo, have bug
 * bool ClientBase::Receive(unsigned int type, OUT unsigned char *data,
                         OUT unsigned int *length, int secondsTimeout)
{
    pthread_mutex_lock(&_cond_receive_obj);
    InitReceiveDataAndType(type);
    struct timespec waittime;
    time_t now = time(0);

    if ((time_t)(-1) == now) {
        qprofileDebug(QtDebugMsg) << "ClientBase(" << _socket <<
                                  "): Receive failed, can not get current time.";
        pthread_mutex_unlock(&_cond_receive_obj);
        return false;
    }

    waittime.tv_sec = now + secondsTimeout;
    waittime.tv_nsec = 0;
    //check, if data has been received, return ture
    unsigned int tmptype;
    unsigned char *tmpdata = NULL;
    tmptype = GetReceiveType();

    if (0 == tmptype) {
        tmpdata = GetReceivedData(tmpdata, length);

        if (*length > 0) {
            memcpy(data, tmpdata, *length);
        }

        if (0 != tmpdata) {
            delete[] tmpdata;
        }

        pthread_mutex_unlock(&_cond_receive_obj);
        return true;
    }

    if (0 != pthread_cond_timedwait(&_cond_receive, &_cond_receive_obj,
                                    &waittime)) {
        pthread_mutex_unlock(&_cond_receive_obj);
        tmptype = GetReceiveType();

        if (0 == tmptype) {
            tmpdata = GetReceivedData(tmpdata, length);

            if (*length > 0) {
                memcpy(data, tmpdata, *length);
            }

            if (0 != tmpdata) {
                delete[] tmpdata;
            }

            return true;
        } else {
            qprofileDebug(QtDebugMsg) << "ClientBase(" << _socket <<
                                      "): Receive failed, time out.";
            *length = 0;
            return false;
        }
    }

    pthread_mutex_unlock(&_cond_receive_obj);
    tmpdata = GetReceivedData(tmpdata, length);

    if (*length > 0) {
        memcpy(data, tmpdata, *length);
    }

    if (0 != tmpdata) {
        delete[] tmpdata;
    }

    return true;
} */

void ClientBase::InitReceiveDataAndType(unsigned int type)
{
    pthread_mutex_lock(&_receive_obj);
    _received_type = type;

    if (0 != _received_data) {
        delete[] _received_data;
        _received_data = 0;
    }

    _received_length = 0;
    pthread_mutex_unlock(&_receive_obj);
}

void ClientBase::SetReceivedDataAndType(unsigned char *data,
                                        unsigned int length)
{
    pthread_mutex_lock(&_receive_obj);
    _received_type = 0;

    //clear old data
    if (0 != _received_data) {
        delete[] _received_data;
        _received_data = 0;
    }

    //set new data
    if (0 != data) {
        if (length > 0) {
            _received_data = new unsigned char[length];
            memcpy(_received_data, data, length);
            _received_length = length;
        } else {
            _received_length = 0;
        }
    } else {
        _received_length = 0;
    }

    pthread_mutex_unlock(&_receive_obj);
    pthread_mutex_lock(&_cond_receive_obj);

    if (0 != pthread_cond_broadcast(&_cond_receive)) {
        qprofileDebug(QtDebugMsg) << "ClientBase(" << _socket <<
                                  "): Error in resuming blocked receive thread.";
    }

    pthread_mutex_unlock(&_cond_receive_obj);
}

unsigned char *ClientBase::GetReceivedData(OUT unsigned char *data,
        OUT unsigned int *length)
{
    pthread_mutex_lock(&_receive_obj);

    if ((0 == _received_data) || (0 == _received_length)) {
        data = 0;
        *length = 0;
    } else {
        data = new unsigned char[_received_length];
        memcpy(data, _received_data, _received_length);
        *length = _received_length;
    }

    pthread_mutex_unlock(&_receive_obj);
    return data;
}

unsigned int ClientBase::GetReceiveType()
{
    unsigned int ret;
    pthread_mutex_lock(&_receive_obj);
    ret = _received_type;
    pthread_mutex_unlock(&_receive_obj);
    return ret;
}

// Socket hNewSocket is the new socket handle
// Return: 0--succeeded, others--error
int ClientBase::SetSocket(Socket hNewSocket)
{
    int ret;

    if (INVALID_SOCKET != _socket) { //check connection
        ret = Disconnect();

        if (0 == ret) {
            qprofileDebug(QtDebugMsg) << "ClientBase(" << _socket <<
                                      "): Set socket failed, connection is already exists.";
            return DISCONNECT_FAILED;
        }
    }

    _socket = hNewSocket;
    //set buffer
    int rev_buff_size = RECEIVE_BUFFER_SIZE;

    if (setsockopt(_socket, SOL_SOCKET, SO_RCVBUF, (const char *)&rev_buff_size,
                   sizeof(int))) {
        qprofileDebug(QtDebugMsg) << "ClientBase(" << _socket <<
                                  "): Set socket receive buffer failed.";
        return SET_SOCKET_OPT_FAILED;
    }

    int snd_buff_size = SEND_BUFFER_SIZE;

    if (setsockopt(_socket, SOL_SOCKET, SO_SNDBUF, (const char *)&snd_buff_size,
                   sizeof(int))) {
        qprofileDebug(QtDebugMsg) << "ClientBase(" << _socket <<
                                  "): Set socket send buffer failed.";
        return SET_SOCKET_OPT_FAILED;
    }

    //set keepalive
    int keepalive = 1;
    int keepidle = KEEPALIVE_IDLE;
    int keepinterval = KEEPALIVE_INTERVAL;
    int keepcount = KEEPALIVE_COUNT;

    if (setsockopt(_socket, SOL_SOCKET, SO_KEEPALIVE, (const char *)&keepalive,
                   sizeof(int))) {
        qprofileDebug(QtDebugMsg) << "ClientBase(" << _socket <<
                                  "): Set socket keepalive failed.";
        return SET_SOCKET_OPT_FAILED;
    }

    if (setsockopt(_socket, SOL_TCP, TCP_KEEPIDLE, (const char *)&keepidle,
                   sizeof(int))) {
        qprofileDebug(QtDebugMsg) << "ClientBase(" << _socket <<
                                  "): Set socket keepalive idle failed.";
        return SET_SOCKET_OPT_FAILED;
    }

    if (setsockopt(_socket, SOL_TCP, TCP_KEEPINTVL, (const char *)&keepinterval,
                   sizeof(int))) {
        qprofileDebug(QtDebugMsg) << "ClientBase(" << _socket <<
                                  "): Set socket keepalive interval failed.";
        return SET_SOCKET_OPT_FAILED;
    }

    if (setsockopt(_socket, SOL_TCP, TCP_KEEPCNT, (const char *)&keepcount,
                   sizeof(int))) {
        qprofileDebug(QtDebugMsg) << "ClientBase(" << _socket <<
                                  "): Set socket keepalive count failed.";
        return SET_SOCKET_OPT_FAILED;
    }

    //set socket to blocking mode
    int flags;

    if ((flags = fcntl(_socket, F_GETFL, 0)) < 0) {
        qprofileDebug(QtDebugMsg) << "ClientBase(" << _socket <<
                                  "): Read socket flag failed.";
        return SET_SOCKET_OPT_FAILED;
    }

    if (fcntl(_socket, F_SETFL, flags & (~O_NONBLOCK)) < 0) {
        qprofileDebug(QtDebugMsg) << "ClientBase(" << _socket <<
                                  "): Set socket blocking mode failed.";
        return SET_SOCKET_OPT_FAILED;
    }

    SetKeepReceiving(true);
    //begin receive
    ret = pthread_create(&(_receive_thread), 0, ReceiveThread, this);

    if (0 != ret) {
        _receive_thread = 0;
        _socket = INVALID_SOCKET;
        qprofileDebug(QtDebugMsg) << "ClientBase(" << _socket <<
                                  "): Create receiving thread failed, connect failed, error=" << ret;
        return CREATE_THREAD_FAILED;
    }

    return 0;
}

bool ClientBase::IsSocketConnected()
{
    if (GetIsConnecting()) {
        return false;
    }

    if (INVALID_SOCKET != _socket) {
        return true;
    }

    return false;
}

//get socket of client
Socket ClientBase::GetSocket()
{
    return _socket;
}

//connect to a server
bool ClientBase::Connect(const char *IPAddress, unsigned short port,
                         const char *ethType)
{
    //set status flag
    SetIsConnecting(true);

    if (INVALID_SOCKET != _socket) { //check connection
        qprofileDebug(QtDebugMsg) << "ClientBase(" << _socket <<
                                  "): Connect failed, connection is already exists";
        SetIsConnecting(false);
        return false;
    }

    //initialize
    SetLastSentBytes(0, 0);
    //
    SetIsSendSucceeded(true);
    //
    SetReplyHashcode(0);
    //
    SetKeepReceiving(true);

    if ((_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        qprofileDebug(QtDebugMsg) << "ClientBase(" << _socket <<
                                  "): Create socket failed.";
        SetIsConnecting(false);
        return false;
    }

    //set buffer
    int rev_buff_size = RECEIVE_BUFFER_SIZE;

    if (setsockopt(_socket, SOL_SOCKET, SO_RCVBUF, (const char *)&rev_buff_size,
                   sizeof(int))) {
        qprofileDebug(QtDebugMsg) << "ClientBase(" << _socket <<
                                  "): Set socket receive buffer failed.";
        close(_socket);
        _socket = INVALID_SOCKET;
        SetIsConnecting(false);
        return false;
    }

    int snd_buff_size = SEND_BUFFER_SIZE;

    if (setsockopt(_socket, SOL_SOCKET, SO_SNDBUF, (const char *)&snd_buff_size,
                   sizeof(int))) {
        qprofileDebug(QtDebugMsg) << "ClientBase(" << _socket <<
                                  "): Set socket send buffer failed.";
        close(_socket);
        _socket = INVALID_SOCKET;
        SetIsConnecting(false);
        return false;
    }

    /* bind to device */
    if (strlen(ethType) > 0) {
        if (setsockopt(_socket, SOL_SOCKET, SO_BINDTODEVICE, ethType,
                       strlen(ethType))) {
            qprofileDebug(QtDebugMsg) << "ClientBase(" << _socket <<
                                      "):  Set socket bind to device failed. ethType =" << ethType;
            close(_socket);
            _socket = INVALID_SOCKET;
            SetIsConnecting(false);
            return false;
        }
    }

    //set keepalive
    int keepalive = 1;
    int keepidle = KEEPALIVE_IDLE;
    int keepinterval = KEEPALIVE_INTERVAL;
    int keepcount = KEEPALIVE_COUNT;

    if (setsockopt(_socket, SOL_SOCKET, SO_KEEPALIVE, (const char *)&keepalive,
                   sizeof(int))) {
        qprofileDebug(QtDebugMsg) << "ClientBase(" << _socket <<
                                  "):  Set socket keepalive failed.";
        close(_socket);
        _socket = INVALID_SOCKET;
        SetIsConnecting(false);
        return false;
    }

    if (setsockopt(_socket, SOL_TCP, TCP_KEEPIDLE, (const char *)&keepidle,
                   sizeof(int))) {
        qprofileDebug(QtDebugMsg) << "ClientBase(" << _socket <<
                                  "): Set socket keepalive idle failed.";
        close(_socket);
        _socket = INVALID_SOCKET;
        SetIsConnecting(false);
        return false;
    }

    if (setsockopt(_socket, SOL_TCP, TCP_KEEPINTVL, (const char *)&keepinterval,
                   sizeof(int))) {
        qprofileDebug(QtDebugMsg) << "ClientBase(" << _socket <<
                                  "): Set socket keepalive interval failed.";
        close(_socket);
        _socket = INVALID_SOCKET;
        SetIsConnecting(false);
        return false;
    }

    if (setsockopt(_socket, SOL_TCP, TCP_KEEPCNT, (const char *)&keepcount,
                   sizeof(int))) {
        qprofileDebug(QtDebugMsg) << "ClientBase(" << _socket <<
                                  "): Set socket keepalive count failed.";
        close(_socket);
        _socket = INVALID_SOCKET;
        SetIsConnecting(false);
        return false;
    }

    struct sockaddr_in   serveraddr;

    //fill serveraddr
    bzero(&serveraddr, sizeof(sockaddr_in));

    serveraddr.sin_family = AF_INET;

    serveraddr.sin_addr.s_addr = inet_addr(IPAddress);

    serveraddr.sin_port = htons(port);

    int flags;

    //set socket to non-blocking mode
    if ((flags = fcntl(_socket, F_GETFL, 0)) < 0) {
        qprofileDebug(QtDebugMsg) << "ClientBase(" << _socket <<
                                  "):Read socket flag failed.";
        close(_socket);
        _socket = INVALID_SOCKET;
        SetIsConnecting(false);
        return false;
    }

    if (fcntl(_socket, F_SETFL, flags | O_NONBLOCK) < 0) {
        qprofileDebug(QtDebugMsg) << "ClientBase(" << _socket <<
                                  "): Set socket non-blocking mode failed.";
        close(_socket);
        _socket = INVALID_SOCKET;
        SetIsConnecting(false);
        return false;
    }

    //connect
    if (SOCKET_ERROR == connect(_socket, (sockaddr *)&serveraddr,
                                sizeof(sockaddr_in))) {
        if (errno == EINPROGRESS) { // it is in the connect process
            struct timeval tv;
            fd_set writefds;
            tv.tv_sec = CONECT_TIMEOUT_SECONDS;
            tv.tv_usec = 0;
            FD_ZERO(&writefds);
            FD_SET(_socket, &writefds);

            if (select(_socket + 1, 0, &writefds, 0,
                       &tv) <= 0) { //timeout or error happen
                qprofileDebug(QtDebugMsg) << "ClientBase(" << _socket << "):  Connect failed.";
                close(_socket);
                _socket = INVALID_SOCKET;
                SetIsConnecting(false);
                return false;
            }
        } else {
            qprofileDebug(QtDebugMsg) << "ClientBase(" << _socket << "):  Connect failed.";
            close(_socket);
            _socket = INVALID_SOCKET;
            SetIsConnecting(false);
            return false;
        }
    }

    //set socket to blocking mode
    if ((flags = fcntl(_socket, F_GETFL, 0)) < 0) {
        qprofileDebug(QtDebugMsg) << "ClientBase(" << _socket <<
                                  "): Connect succeeded, but read socket flag failed.";
        close(_socket);
        _socket = INVALID_SOCKET;
        SetIsConnecting(false);
        return false;
    }

    if (fcntl(_socket, F_SETFL, flags & (~O_NONBLOCK)) < 0) {
        qprofileDebug(QtDebugMsg) << "ClientBase(" << _socket <<
                                  "): Set socket blocking mode failed.";
        close(_socket);
        _socket = INVALID_SOCKET;
        SetIsConnecting(false);
        return false;
    }

    pthread_mutex_lock(&_cond_connect_obj);
    //begin receive
    int ret = pthread_create(&(_receive_thread), 0, ReceiveThread, this);

    if (0 != ret) {
        pthread_mutex_unlock(&_cond_connect_obj);
        _receive_thread = 0;
        qprofileDebug(QtDebugMsg) << "ClientBase(" << _socket <<
                                  "):Create receiving thread failed, connect failed, error=" << ret;
        Disconnect();
        SetIsConnecting(false);
        return false;
    }

    //wait for server reply
    struct timespec waittime;
    time_t now = time(0);

    if ((time_t)(-1) == now) {
        pthread_mutex_unlock(&_cond_connect_obj);
        qprofileDebug(QtDebugMsg) << "ClientBase(" << _socket <<
                                  "): Connect failed, can not get current time.";
        DisconnectForcibly();
        SetIsConnecting(false);
        return false;
    }

    waittime.tv_sec = now + CONECT_TIMEOUT_SECONDS;
    waittime.tv_nsec = 0;

    if (0 != pthread_cond_timedwait(&_cond_connect, &_cond_connect_obj,
                                    &waittime)) {
        pthread_mutex_unlock(&_cond_connect_obj);
        qprofileDebug(QtDebugMsg) << "ClientBase(" << _socket <<
                                  "): Connect failed, can not get server's reply.";
        DisconnectForcibly();
        SetIsConnecting(false);
        return false;
    }

    pthread_mutex_unlock(&_cond_connect_obj);
    SetIsConnecting(false);
    _isconnected = true;
    return true;
}

bool ClientBase::getCurrentConnectStatus()
{
    return _isconnected;
}
//disconnect from server
bool ClientBase::Disconnect()
{
    if (INVALID_SOCKET == _socket) {
        return false;
    }

    StopSendWaiting();

    if (0 != _receive_thread) {
        struct timespec waittime;
        time_t now = time(0);

        if ((time_t)(-1) == now) {
            qprofileDebug(QtDebugMsg) << "ClientBase(" << _socket <<
                                      "): Disconnect failed, can not get current time.";
            return false;
        }

        waittime.tv_sec = now + DISCONECT_TIMEOUT_SECONDS;
        waittime.tv_nsec = 0;
        pthread_mutex_lock(&_cond_exit_obj);
        //end receiving thread
        SetKeepReceiving(false);

        if (0 != pthread_cond_timedwait(&_cond_exit, &_cond_exit_obj, &waittime)) {
            pthread_mutex_unlock(&_cond_exit_obj);
            qprofileDebug(QtDebugMsg) << "ClientBase(" << _socket <<
                                      "):  Disconnect failed, time out.";
            return false;
        }

        pthread_mutex_unlock(&_cond_exit_obj);
    }

    Reply(true);
    //end all reading and writing operation
    shutdown(_socket, 2);
    close(_socket);
    _socket = INVALID_SOCKET;
    return true;
}

//disconnect from server, without waiting
bool ClientBase::DisconnectForcibly()
{
    if (INVALID_SOCKET == _socket) {
        return false;
    }

    StopSendWaiting();

    if (0 != _receive_thread) {
        //end receiving thread
        SetKeepReceiving(false);
    }

    Reply(true);
    //end all reading and writing operation
    shutdown(_socket, 2);
    close(_socket);
    _socket = INVALID_SOCKET;
    return true;
}

//send data to server
bool ClientBase::SimpleSend(unsigned int type)
{
    if (INVALID_SOCKET == _socket) {
        qprofileDebug(QtDebugMsg) << "ClientBase(" << _socket <<
                                  "):Sending failed, invalid socket.";
        return false;
    }

    unsigned char *buffer;
    buffer = CreateFrame(APP_FLAG, 0, 0, type, buffer);

    if (0 == buffer) {
        qprofileDebug(QtDebugMsg) << "ClientBase(" << _socket <<
                                  "): Send failed, error in creating frame.";
        return false;
    }

    int bytesSent = CoreSend(_socket, buffer, sizeof(FrameHeader), 0);
    delete []buffer;

    if (bytesSent < 0) {
        qprofileDebug(QtDebugMsg) << "ClientBase(" << _socket <<
                                  "):Send error, socket error.";
        return false;
    }

    return true;
}

bool ClientBase::SimpleSend(unsigned int type, const unsigned char *data,
                            unsigned int length)
{
    if (INVALID_SOCKET == _socket) {
        qprofileDebug(QtDebugMsg) << "ClientBase(" << _socket <<
                                  "): Sending failed, invalid socket.";
        return false;
    }

    unsigned char *buffer;
    buffer = CreateFrame(APP_FLAG, data, length, type, buffer);

    if (0 == buffer) {
        qprofileDebug(QtDebugMsg) << "ClientBase(" << _socket <<
                                  "):Send failed, error in creating frame.";
        return false;
    }

    SetLastSentBytes(0, 0);
    int bytesSent = CoreSend(_socket, buffer, length + sizeof(FrameHeader), 0);
    delete []buffer;

    if (bytesSent < 0) {
        qprofileDebug(QtDebugMsg) << "ClientBase(" << _socket <<
                                  "): Send error, socket error.";
        return false;
    }

    return true;
}

bool ClientBase::Send(unsigned int type)
{
    return Send(type, 0, 0, -1);
}
bool ClientBase::Send(unsigned int type, int secondsTimeout)
{
    return Send(type, 0, 0, secondsTimeout);
}
bool ClientBase::Send(unsigned int type, const unsigned char *data,
                      unsigned int length)
{
    return Send(type, data, length, -1);
}
bool ClientBase::Send(unsigned int type, const unsigned char *data,
                      unsigned int length, int secondsTimeout)
{
    if (INVALID_SOCKET == _socket) {
        qprofileDebug(QtDebugMsg) << "ClientBase(" << _socket <<
                                  "):Sending failed, invalid socket.";
        return false;
    }

    bool ret = IsSendSucceeded();

    if (!ret) {
        time_t now = time(0);

        if (difftime(now, _last_sent_time) >= SEND_TIMEOUT_SECONDS) {
            qprofileDebug(QtDebugMsg) << "ClientBase(" << _socket <<
                                      "): Socket failed, time out.";
            DisconnectForcibly();
            RaiseSocketErrorEvent();
        } else {
            qprofileDebug(QtDebugMsg) << "ClientBase(" << _socket <<
                                      "):Send failed, you must wait for the last sending.";
        }

        return false;
    }

    //create frame
    unsigned char *tmpdata = NULL;
    tmpdata = CreateFrame(APP_FLAG, data, length, type, tmpdata);

    if (0 == tmpdata) {
        qprofileDebug(QtDebugMsg) << "ClientBase(" << _socket <<
                                  "):Send failed, error in creating frame.";
        return false;
    }

    SetLastSentBytes(tmpdata, length + sizeof(FrameHeader));
    SetIsSendSucceeded(false);
    pthread_mutex_lock(&_cond_obj);
    //send
    int bytesSent = CoreSend(_socket, tmpdata, length + sizeof(FrameHeader), 0);
    _last_sent_time = time(0);
    delete[] tmpdata;

    if (bytesSent < 0) {
        qprofileDebug(QtDebugMsg) << "ClientBase(" << _socket <<
                                  "): Send failed, socket error.";
        pthread_mutex_unlock(&_cond_obj);
        return false;
    }

    //wait
    if (secondsTimeout > 0) {
        struct timespec waittime;
        time_t now = time(0);

        if ((time_t)(-1) == now) {
            qprofileDebug(QtDebugMsg) << "ClientBase(" << _socket <<
                                      "):Send failed, can not get current time.";
            pthread_mutex_unlock(&_cond_obj);
            return false;
        }

        waittime.tv_sec = now + secondsTimeout;
        waittime.tv_nsec = 0;
        //check, if replay has been received, return ture
        ret = IsSendSucceeded();

        if (ret) {
            pthread_mutex_unlock(&_cond_obj);
            return true;
        }

        if (0 != pthread_cond_timedwait(&_cond, &_cond_obj, &waittime)) {
            pthread_mutex_unlock(&_cond_obj);
            ret = IsSendSucceeded();

            if (!ret) {
                qprofileDebug(QtDebugMsg) << "ClientBase(" << _socket <<
                                          "):Send failed, time out.";
                DisconnectForcibly();
                RaiseSocketErrorEvent();
                return false;
            } else {
                return true;
            }
        }
    } else {
        //check, if replay has been received, return ture
        ret = IsSendSucceeded();

        if (ret) {
            pthread_mutex_unlock(&_cond_obj);
            return true;
        }

        if (0 != pthread_cond_wait(&_cond, &_cond_obj)) {
            pthread_mutex_unlock(&_cond_obj);
            qprofileDebug(QtDebugMsg) << "ClientBase(" << _socket <<
                                      "):Send failed, error in waiting.";
            return false;
        }
    }

    pthread_mutex_unlock(&_cond_obj);
    ret = IsSendSucceeded();
    return ret;
}

//get the last sending status
bool ClientBase::IsSendSucceeded()
{
    if (INVALID_SOCKET == _socket) {
        return false;
    }

    bool ret;
    pthread_mutex_lock(&_send_succeeded_obj);
    ret = _send_succeeded;
    pthread_mutex_unlock(&_send_succeeded_obj);
    return ret;
}

//set the last sending status
void ClientBase::SetIsSendSucceeded(bool value)
{
    pthread_mutex_lock(&_send_succeeded_obj);
    _send_succeeded = value;
    pthread_mutex_unlock(&_send_succeeded_obj);
}

//get receiving thread status
bool ClientBase::KeepReceiving()
{
    if (INVALID_SOCKET == _socket) {
        return false;
    }

    bool ret;
    pthread_mutex_lock(&_keep_running_thread_obj);
    ret = _keep_running_thread;
    pthread_mutex_unlock(&_keep_running_thread_obj);
    return ret;
}

//set receiving thread status
void ClientBase::SetKeepReceiving(bool value)
{
    pthread_mutex_lock(&_keep_running_thread_obj);
    _keep_running_thread = value;
    pthread_mutex_unlock(&_keep_running_thread_obj);
}

//set connection status
void ClientBase::SetIsConnecting(bool value)
{
    pthread_mutex_lock(&_isconnecting_obj);
    _isconnecting = value;
    pthread_mutex_unlock(&_isconnecting_obj);
}

//get connection status
bool ClientBase::GetIsConnecting()
{
    bool ret;
    pthread_mutex_lock(&_isconnecting_obj);
    ret = _isconnecting;
    pthread_mutex_unlock(&_isconnecting_obj);
    return ret;
}

unsigned char *ClientBase::GetLastSentBytes(unsigned char *data,
        unsigned int *length)
{
    pthread_mutex_lock(&_last_sent_bytes_obj);

    if (0 == _last_sent_bytes) {
        data = 0;
        *length = 0;
    } else {
        data = new unsigned char[_last_sent_length];
        memcpy(data, _last_sent_bytes, _last_sent_length);
        *length = _last_sent_length;
    }

    pthread_mutex_unlock(&_last_sent_bytes_obj);
    return data;
}

void	ClientBase::SetLastSentBytes(unsigned char *data, unsigned int length)
{
    pthread_mutex_lock(&_last_sent_bytes_obj);

    if (0 != _last_sent_bytes) {
        delete[] _last_sent_bytes;
    }

    if (0 != data) {
        _last_sent_length = length;
        _last_sent_bytes = new unsigned char[_last_sent_length];
        memcpy(_last_sent_bytes, data, _last_sent_length);
    } else {
        _last_sent_length = 0;
        _last_sent_bytes = 0;
    }

    pthread_mutex_unlock(&_last_sent_bytes_obj);
}

unsigned char *ClientBase::GetReplyHashcode(unsigned char *data)
{
    pthread_mutex_lock(&_reply_hashcode_obj);

    if (0 == _reply_hashcode) {
        data = 0;
    } else {
        data = new unsigned char[16];
        memcpy(data, _reply_hashcode, 16);
    }

    pthread_mutex_unlock(&_reply_hashcode_obj);
    return data;
}

void	ClientBase::SetReplyHashcode(unsigned char *data)
{
    pthread_mutex_lock(&_reply_hashcode_obj);

    if (0 != data) {
        if (0 == _reply_hashcode) {
            _reply_hashcode = new unsigned char[16];
        }

        memcpy(_reply_hashcode, data, 16);
    } else {
        if (0 != _reply_hashcode) {
            delete[] _reply_hashcode;
            _reply_hashcode = 0;
        }
    }

    pthread_mutex_unlock(&_reply_hashcode_obj);
}

void ClientBase::Execute(unsigned int type, const unsigned char *data,
                         unsigned int length)
{
    ReceivedData *tmpdata = new ReceivedData;
    tmpdata->Type = type;
    tmpdata->Length = length;

    if ((0 != data) && (length > 0)) {
        tmpdata->Data = new unsigned char[length];
        memcpy(tmpdata->Data, data, length);
    }

    if (0 != _executer) {
        _executer->Execute(tmpdata);
    }

    delete tmpdata;
}

//raise events
void ClientBase::RaiseRecievedEvent(unsigned int type,
                                    const unsigned char *data, unsigned int length)
{
    OnReceived(type, data, length);
}
void ClientBase::RaiseServerCloseEvent()
{
    bool ret = GetIsConnecting();

    if (!ret) {
        OnServerClose();
    }
}
void ClientBase::RaiseSocketErrorEvent()
{
    bool ret = GetIsConnecting();

    if (!ret) {
        OnSocketError();
    }
}

//stop waiting in sending data
void ClientBase::StopSendWaiting()
{
    pthread_mutex_lock(&_cond_obj);

    if (0 != pthread_cond_broadcast(&_cond)) {
        qprofileDebug(QtDebugMsg) << "ClientBase(" << _socket <<
                                  "):Error in resuming blocked sending thread.";
    }

    pthread_mutex_unlock(&_cond_obj);
}

//stop waiting in disconnect
void ClientBase::StopDisconnectWaiting()
{
    pthread_mutex_lock(&_cond_exit_obj);

    if (0 != pthread_cond_broadcast(&_cond_exit)) {
        qprofileDebug(QtDebugMsg) << "ClientBase(" << _socket <<
                                  "):Error in resuming blocked disconnecting thread.";
    }

    pthread_mutex_unlock(&_cond_exit_obj);
}

//stop waiting in connection
void ClientBase::StopConnectWaiting()
{
    pthread_mutex_lock(&_cond_connect_obj);

    if (0 != pthread_cond_broadcast(&_cond_connect)) {
        qprofileDebug(QtDebugMsg) << "ClientBase(" << _socket <<
                                  "):Error in resuming blocked connecting thread.";
    }

    pthread_mutex_unlock(&_cond_connect_obj);
}

//deal reply message, if correct, resume the blocking thread, else, re-send data
void ClientBase::DealReplyMessage(const unsigned char *md5Hash,
                                  bool isSucceeded)
{
    unsigned char *tmpdata;
    unsigned int tmplength;
    tmpdata = GetLastSentBytes(tmpdata, &tmplength);

    if (0 != tmpdata) {
        if (isSucceeded) {
            if (0 == memcmp(md5Hash, tmpdata, 16)) { //receive server reply, data correct
                SetIsSendSucceeded(true);
                StopSendWaiting();
            } else {
                qprofileDebug(QtDebugMsg) << "ClientBase(" << _socket <<
                                          "):md5 hash is not correct when receive MSG_DATA_CORRECT";
                printf("ClientBase(%d): last sent bytes' md5 hash is:", _socket);

                for (int i = 0; i < 16; i++) {
                    printf("%X", tmpdata[i]);
                }

                printf("\nClientBase(%d): received md5 hash is:", _socket);

                for (int i = 0; i < 16; i++) {
                    printf("%X", md5Hash[i]);
                }

                printf("\n");
            }
        } else {
            if (0 == memcmp(md5Hash, tmpdata,
                            16)) { //receive server reply data error, re-send data
                int bytesSent = CoreSend(_socket, tmpdata, tmplength, 0);

                if (bytesSent < 0) {
                    qprofileDebug(QtDebugMsg) << "ClientBase(" << _socket << "):Sending error.";
                }
            } else {
                qprofileDebug(QtDebugMsg) << "ClientBase(" << _socket <<
                                          "):md5 hash is not correct when receive MSG_DATA_ERROR";
                printf("ClientBase(%d): last sent bytes' md5 hash is:", _socket);

                for (int i = 0; i < 16; i++) {
                    printf("%X", tmpdata[i]);
                }

                printf("\nClientBase(%d): received md5 hash is:", _socket);

                for (int i = 0; i < 16; i++) {
                    printf("%X", md5Hash[i]);
                }

                printf("\n");
            }
        }
    }

    if (0 != tmpdata) {
        delete[] tmpdata;
    }
}

//reply the receive status to server
void ClientBase::Reply(bool successful)
{
    if (INVALID_SOCKET == _socket) {
        qprofileDebug(QtDebugMsg) << "ClientBase(" << _socket <<
                                  "):Reply failed, invalid socket.";
        return;
    }

    unsigned char *tmpdata = NULL;
    tmpdata = GetReplyHashcode(tmpdata);

    if (0 == tmpdata) {
        return;
    }

    FrameHeader *fh = new FrameHeader;
    strncpy(fh->AppFlag, APP_FLAG, strlen(APP_FLAG));
    fh->DataLength = 0;
    //	fh->Type = successful ? ReverseInt32(MSG_DATA_CORRECT) : ReverseInt32( MSG_DATA_ERROR);
    fh->Type = successful ? MSG_DATA_CORRECT : MSG_DATA_ERROR;
    memcpy(fh->MD5Hash, tmpdata, 16);
    int bytesSent = CoreSend(_socket, (unsigned char *)fh, sizeof(FrameHeader), 0);

    if (bytesSent < 0) {
        qprofileDebug(QtDebugMsg) << "ClientBase(" << _socket <<
                                  "):Reply failed, failed to send data.";
    }

    delete fh;
    delete[] tmpdata;
    SetReplyHashcode(0);
}

//create a frame
unsigned char *ClientBase::CreateFrame(const char *appFlag,
                                       const unsigned char *data, unsigned int length, unsigned int type,
                                       unsigned char *buffer)
{
    //check para
    if ((0 ==  data) && (0 != length)) {
        buffer = 0;
        return 0;
    }

    if (length < 0) {
        buffer = 0;
        return 0;
    }

    FrameHeader *fh = new FrameHeader;
    buffer = new unsigned char[sizeof(FrameHeader) + length];
    unsigned char *md5hash;
    //init frame header
    strncpy(fh->AppFlag, APP_FLAG, strlen(APP_FLAG));
    //	fh->DataLength = ReverseInt32(length);
    //	fh->Type = ReverseInt32(type);
    fh->DataLength = length;
    fh->Type = type;
    //copy header and data to buffer
    memcpy(buffer, fh, sizeof(FrameHeader));

    if ((0 != data) && (length > 0)) {
        memcpy((unsigned char *)(buffer + sizeof(FrameHeader)), data, length);
    }

    //caculate hashcode
    md5hash = MDBytes((unsigned char *)(buffer + 16), length + 16, md5hash);
    memcpy(buffer, md5hash, 16);
    delete fh;
    delete[] md5hash;
    return buffer;
}

int ClientBase::CoreSend(Socket socket, unsigned char *data,  int length,
                         int flag)
{
    int ret = 0;
    pthread_mutex_lock(&_send_obj);
    ret = send(socket, data, length, flag);
    pthread_mutex_unlock(&_send_obj);
    return ret;
}

void ClientBase::OnReceived(unsigned int type, const unsigned char *data,
                            unsigned int length)
{
    qprofileDebug(QtDebugMsg) << "ClientBase: OnReceived.";
}

void ClientBase::OnServerClose()
{
    _isconnected = false;
    qprofileDebug(QtDebugMsg) << "ClientBase: OnServerClose.";
}

void ClientBase::OnSocketError()
{
    _isconnected = false;
    qprofileDebug(QtDebugMsg) << "ClientBase: OnSocketError.";
}

