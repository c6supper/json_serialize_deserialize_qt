/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qmessagefactory.h"
#include "qmsggetserverinfo.h"

using namespace QRserver;
using namespace QProfile;
using namespace rserver;

QMessageSelfRegisteration<QMsgGetServerInfo> registerQMsgGetServerInfo(
    rserver::eGetServerInfo);

QDataStream &QMsgGetServerInfo::read(QDataStream &in)
{
    st_SyncServerInfo stMessage;

    if ((qint32)sizeof(stMessage) <= in.readRawData((char *)&stMessage,
            sizeof(stMessage))) {
        m_serverName->setValue(QVariant(stMessage.szServerName));
        m_serverLocation->setValue(QVariant(stMessage.szServerLocation));
#if defined(Q_OS_LINUX)
        struct tm  tmDateTime = {0};
        tmDateTime.tm_sec  = stMessage.nArrDate[5];
        tmDateTime.tm_min  = stMessage.nArrDate[4];
        tmDateTime.tm_hour = stMessage.nArrDate[3];
        tmDateTime.tm_mday = stMessage.nArrDate[2];
        tmDateTime.tm_mon  = stMessage.nArrDate[1] - 1;
        tmDateTime.tm_year = stMessage.nArrDate[0] - 1900;
        tmDateTime.tm_wday  = 0;
        tmDateTime.tm_yday  = 0;
        tmDateTime.tm_isdst = 0;
        time_t time_new = timelocal((struct tm *)&tmDateTime);
        m_serverDateTime->setValue(QVariant(QDateTime::fromTime_t(time_new)));
#else
        m_serverDateTime->setValue(QVariant(QDateTime::currentDateTime()));
#endif
    }

    return in;
}

const QByteArray QMsgGetServerInfo::toByteArray() const
{
    st_SyncServerInfo serverInfo;
    bzero(&serverInfo, sizeof(serverInfo));
    QByteArray messageArray((const char *)&serverInfo, sizeof(serverInfo));
    QDataStream out(&messageArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_6);
    out << *this;
    return messageArray;
}

QDataStream &QMsgGetServerInfo::write(QDataStream &out) const
{
    return out;
}

QMsgGetServerInfo::QMsgGetServerInfo(QObject *parent)
    : QAbstractBinMsg(rserver::eGetServerInfo, parent)
{
    initAttributes();
}

QMsgGetServerInfo::QMsgGetServerInfo(const QMsgGetServerInfo &message)
    : QAbstractBinMsg(message)
{
    initAttributes();
}

void QMsgGetServerInfo::initAttributes()
{
    m_serverName = QSharedPointer<QStringAttribute>(new QStringAttribute(this));
    m_serverLocation = QSharedPointer<QStringAttribute>(new QStringAttribute(this));
    m_serverDateTime = QSharedPointer<QDateTimeAttribute>(new QDateTimeAttribute(
                           this));
}

QMsgGetServerInfo::~QMsgGetServerInfo() {}

QString QMsgGetServerInfo::serverName() const
{
    return m_serverName->value().toString();
}
void QMsgGetServerInfo::setServerName(QString serverName)
{
    m_serverName->setValue(serverName);
}

QString QMsgGetServerInfo::serverLocation() const
{
    return m_serverLocation->value().toString();
}
void QMsgGetServerInfo::setServerLocation(QString serverLocation)
{
    m_serverLocation->setValue(serverLocation);
}

QDateTime QMsgGetServerInfo::serverDateTime() const
{
    return m_serverDateTime->value().toDateTime();
}
void QMsgGetServerInfo::setServerDateTime(QDateTime dateTime)
{
    m_serverDateTime->setValue(dateTime);
}


