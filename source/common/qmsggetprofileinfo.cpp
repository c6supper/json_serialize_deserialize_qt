#include "qmessagefactory.h"
#include "qmsggetprofileinfo.h"

using namespace QRserver;
using namespace rserver;

QMessageSelfRegisteration<QMsgGetProfileInfo> registerQMsgGetProfileInfo(
    rserver::eGetProfileInfo);

QDataStream &QMsgGetProfileInfo::read(QDataStream &in)
{
    qint32 count;

    if ((qint32)sizeof(count) <= in.readRawData((char *)&count, sizeof(count))) {
        m_count = count;
    }

    return in;
}

const QByteArray QMsgGetProfileInfo::toByteArray() const
{
    QByteArray messageArray(sizeof(m_count), m_count);
    QDataStream out(&messageArray, QIODevice::ReadWrite);
    out << *this;
    return messageArray;
}

QDataStream &QMsgGetProfileInfo::write(QDataStream &out) const
{
    return out;
}

QMsgGetProfileInfo::QMsgGetProfileInfo(QObject *parent)
    : QAbstractBinMsg(rserver::eGetProfileInfo, parent), m_count(0)
{
}

QMsgGetProfileInfo::QMsgGetProfileInfo(const QMsgGetProfileInfo &message)
    : QAbstractBinMsg(message), m_count(message.count())
{
}

const qint32 QMsgGetProfileInfo::count() const
{
    return m_count;
}

void QMsgGetProfileInfo::setCount(const qint32 count)
{
    m_count = count;
}

QMsgGetProfileInfo::~QMsgGetProfileInfo()
{
}

