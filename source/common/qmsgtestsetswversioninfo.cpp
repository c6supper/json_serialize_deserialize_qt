/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qmessagefactory.h"
#include "qmsgtestsetswversioninfo.h"

using namespace QRserver;
using namespace QProfile;
using namespace rserver;

QMessageSelfRegisteration<QMsgTestsetSwVersionInfo>
registerQMsgTestsetSwVersionInfo(
    rserver::eTestsetSWverionInfo);

QDataStream &QMsgTestsetSwVersionInfo::read(QDataStream &in)
{
    st_TestsetSWVersioninfo stMessage;

    if ((qint32)sizeof(stMessage) <= in.readRawData((char *)&stMessage,
            sizeof(stMessage))) {
        m_imageType = stMessage.Type;
        m_version->setValue(QVariant(stMessage.CurrentVersion));
    }

    return in;
}

const QByteArray QMsgTestsetSwVersionInfo::toByteArray() const
{
    st_TestsetSWVersioninfo swInfo;
    bzero(&swInfo, sizeof(swInfo));
    QByteArray messageArray((const char *)&swInfo, sizeof(swInfo));
    QDataStream out(&messageArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_6);
    out << *this;
    return messageArray;
}

QDataStream &QMsgTestsetSwVersionInfo::write(QDataStream &out) const
{
    st_TestsetSWVersioninfo stMessage;
    bzero(&stMessage, sizeof(stMessage));
    m_version->toChar(stMessage.CurrentVersion, sizeof(stMessage.CurrentVersion));
    stMessage.Type = m_imageType;
    out.writeRawData((const char *)&stMessage, sizeof(stMessage));
    return out;
}

QMsgTestsetSwVersionInfo::QMsgTestsetSwVersionInfo(QObject *parent)
    : QAbstractBinMsg(eTestsetSWverionInfo, parent)
{
    initAttributes();
}

QMsgTestsetSwVersionInfo::QMsgTestsetSwVersionInfo(const
        QMsgTestsetSwVersionInfo
        &message)
    : QAbstractBinMsg(message)
{
    initAttributes();
}

void QMsgTestsetSwVersionInfo::initAttributes()
{
    m_version = new QStringAttribute(this);
}

QMsgTestsetSwVersionInfo::~QMsgTestsetSwVersionInfo()
{
    delete m_version;
}


