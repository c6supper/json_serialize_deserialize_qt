/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qmessagefactory.h"
#include "qmsguploadprofileblock.h"

using namespace QRserver;
using namespace QProfile;
using namespace rserver;

QMessageSelfRegisteration<QMsgUploadProfileBlock>
registerQMsgUploadProfileBlock(
    rserver::eUploadProfileBlock);

const qint32 QMsgUploadProfileBlock::MaxUploadFileBlock = (30 * 1024);

QDataStream &QMsgUploadProfileBlock::read(QDataStream &in)
{
    st_ProfileFile stMessage;

    if ((qint32)sizeof(stMessage) <= in.readRawData((char *)&stMessage,
            sizeof(stMessage))) {
        m_profileName->setValue(QVariant(stMessage.szName));
        m_profileType = stMessage.nType;
        m_blockIndex = stMessage.nIndex;
        m_totalLength = stMessage.nLength;
        m_block.clear();
        m_block.resize(blockLength());

        if (blockLength() != (quint32)in.readRawData(m_block.data(), blockLength())) {
            qprofileDebug(QtWarningMsg) << "got invalid a message, length = " <<
                                        blockLength();
        }
    }

    return in;
}

const QByteArray QMsgUploadProfileBlock::toByteArray() const
{
    QByteArray messageArray(sizeof(st_ProfileFile) + blockLength(), 0);
    QDataStream out(&messageArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_6);
    out << *this;
    return messageArray;
}

QDataStream &QMsgUploadProfileBlock::write(QDataStream &out) const
{
    st_ProfileFile stMessage;
    bzero(&stMessage, sizeof(stMessage));
    m_profileName->toChar(stMessage.szName, sizeof(stMessage.szName));
    stMessage.nType = m_profileType;
    stMessage.nIndex = m_blockIndex;
    stMessage.nLength = m_totalLength;
    out.writeRawData((const char *)&stMessage, sizeof(stMessage));
    out.writeRawData((const char *)m_block.data(), blockLength());
    return out;
}

QMsgUploadProfileBlock::QMsgUploadProfileBlock(QObject *parent)
    : QAbstractBinMsg(rserver::eUploadProfileBlock, parent),
      m_totalLength(0), m_profileName(nullptr)
{
    initAttributes();
}

QMsgUploadProfileBlock::QMsgUploadProfileBlock(const QMsgUploadProfileBlock
        &message)
    : QAbstractBinMsg(message), m_totalLength(0), m_profileName(nullptr)
{
    initAttributes();
}

const QString QMsgUploadProfileBlock::profileName()
{
    return m_profileName->value().toString();
}
void QMsgUploadProfileBlock::setProfileName(QString profileName)
{
    m_profileName->setValue(QVariant(profileName));
}

quint32 QMsgUploadProfileBlock::profileType() const
{
    return m_profileType;
}
void QMsgUploadProfileBlock::setProfileType(quint32 profileType)
{
    m_profileType = profileType;
}

quint32 QMsgUploadProfileBlock::blockIndex() const
{
    return m_blockIndex;
}
void QMsgUploadProfileBlock::setBlockIndex(quint32 blockIndex)
{
    m_blockIndex = blockIndex;
}

quint32 QMsgUploadProfileBlock::blockLength() const
{
    return MaxUploadFileBlock;
}

quint32 QMsgUploadProfileBlock::totalLength() const
{
    return m_totalLength;
}
void QMsgUploadProfileBlock::setTotalLength(quint32 totalLength)
{
    m_totalLength = totalLength;
}

const QByteArray QMsgUploadProfileBlock::block() const
{
    return m_block.toBase64();
}
void QMsgUploadProfileBlock::setBlock(QByteArray block)
{
    m_block = QByteArray::fromBase64(block);
}

void QMsgUploadProfileBlock::initAttributes()
{
    m_profileName = new QStringAttribute(this);
}

QMsgUploadProfileBlock::~QMsgUploadProfileBlock()
{
    delete m_profileName;
}
