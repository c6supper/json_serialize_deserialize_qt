/*
 * qmsguploadprofileinfoend.cpp
 *
 *  Created on: Sep 14, 2020
 *      Author: duyi
 */

#include "qmessagefactory.h"
#include "qmsguploadprofileinfoend.h"

using namespace QRserver;
using namespace QProfile;
using namespace rserver;

QMessageSelfRegisteration<QMsgUploadProfileInfoEnd>
registerQMsgUploadProfileInfoEnd(
    rserver::eUploadProfileInfoEnd);

QMsgUploadProfileInfoEnd::QMsgUploadProfileInfoEnd(QObject *parent) :
    QAbstractBinMsg(rserver::eUploadProfileInfoEnd, parent), m_profileInfoCount(0)
{
}

QMsgUploadProfileInfoEnd::QMsgUploadProfileInfoEnd(const
        QMsgUploadProfileInfoEnd
        &message)
    : QAbstractBinMsg(message)
{
}

QMsgUploadProfileInfoEnd::~QMsgUploadProfileInfoEnd() {}

qint32 QMsgUploadProfileInfoEnd::profileInfoCount() const
{
    return m_profileInfoCount;
}

void QMsgUploadProfileInfoEnd::setProfileInfoCount(qint32 profileInfoCount)
{
    m_profileInfoCount = profileInfoCount;
}


QDataStream &QMsgUploadProfileInfoEnd::read(QDataStream &in)
{
    return in;
}

QDataStream &QMsgUploadProfileInfoEnd::write(QDataStream &out) const
{
    int profileInfoCount = 0;
    profileInfoCount = m_profileInfoCount;
    out.writeRawData((const char *)&profileInfoCount, sizeof(profileInfoCount));
    return out;
}

const QByteArray QMsgUploadProfileInfoEnd::toByteArray() const
{
    int profileInfoCount = 0;
    QByteArray messageArray((const char *)&profileInfoCount,
                            sizeof(profileInfoCount));
    QDataStream out(&messageArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_6);
    out << *this;
    return messageArray;
}
