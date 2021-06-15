/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qmessagefactory.h"
#include "qmsgtestsetswinfo.h"

using namespace QRserver;
using namespace QProfile;
using namespace rserver;

QMessageSelfRegisteration<QMsgTestsetSwInfo>
registerQMsgTestsetSwInfo(
    rserver::eTestsetSWinfo);

QDataStream &QMsgTestsetSwInfo::read(QDataStream &in)
{
    st_TestsetSWinfo stMessage;

    if ((qint32)sizeof(stMessage) <= in.readRawData((
                char *)&stMessage, sizeof(stMessage))) {
        m_softwareStatus = stMessage.Status;
        m_softwareTime = stMessage.Time;
        m_softwareLength = stMessage.Length;
        m_softwareMd5.clear();
        m_softwareMd5.append((const char *)stMessage.MD5, sizeof(stMessage.MD5));
    }

    return in;
}

const QByteArray QMsgTestsetSwInfo::toByteArray() const
{
    st_TestsetSWinfo swInfo;
    bzero(&swInfo, sizeof(st_TestsetSWinfo));
    QByteArray messageArray((const char *)&swInfo, sizeof(swInfo));
    QDataStream out(&messageArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_6);
    out << *this;
    return messageArray;
}

QDataStream &QMsgTestsetSwInfo::write(QDataStream &out) const
{
    st_TestsetSWinfo stMessage;
    bzero(&stMessage, sizeof(stMessage));
    stMessage.Length = m_softwareLength;
    memcpy(stMessage.MD5, m_softwareMd5.data(), sizeof(stMessage.MD5));
    stMessage.Status = m_softwareStatus;
    stMessage.Time = m_softwareTime;
    out.writeRawData((const char *)&stMessage, sizeof(stMessage));
    return out;
}

QMsgTestsetSwInfo::QMsgTestsetSwInfo(QObject *parent)
    : QAbstractBinMsg(eTestsetSWinfo, parent)
{
    initAttributes();
}

QMsgTestsetSwInfo::QMsgTestsetSwInfo(const
                                     QMsgTestsetSwInfo
                                     &message)
    : QAbstractBinMsg(message)
{
    initAttributes();
}

void QMsgTestsetSwInfo::initAttributes()
{
}

QMsgTestsetSwInfo::~QMsgTestsetSwInfo() {}


