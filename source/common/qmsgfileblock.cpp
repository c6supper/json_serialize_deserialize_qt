/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qmessagefactory.h"
#include "qmsgfileblock.h"

using namespace QRserver;
using namespace QProfile;
using namespace rserver;

QMessageSelfRegisteration<QMsgFileBlock>
registerQMsgFileBlock(eFileBlock);

const qint32 QMsgFileBlock::MaxFileBlock = (60 * 1024);

QDataStream &QMsgFileBlock::read(QDataStream &in)
{
    st_ResultFile stMessage;

    if ((qint32)sizeof(stMessage) <= in.readRawData((char *)&stMessage,
            sizeof(stMessage))) {
        m_folderName->setValue(QVariant(stMessage.szFolderName));
        m_fileName->setValue(QVariant(stMessage.szFileName));
        m_blockIndex = stMessage.nBlockIndex;
        m_blockLength = stMessage.nBlockLength > MaxFileBlock ? MaxFileBlock :
                        stMessage.nBlockLength;

        if (stMessage.nBlockLength != m_blockLength && type() == eFileBlock) {
            qprofileDebug(QtWarningMsg) << "got invalid a message, length = " <<
                                        stMessage.nBlockLength;
        }

        m_block.clear();
        m_block.resize(m_blockLength);

        if (m_blockLength != in.readRawData(m_block.data(), m_blockLength)) {
            qprofileDebug(QtWarningMsg) << "got invalid a message, length = " <<
                                        m_blockLength;
        }
    }

    return in;
}

const QByteArray QMsgFileBlock::toByteArray() const
{
    QByteArray messageArray(sizeof(st_ResultFile) + m_blockLength, 0);
    QDataStream out(&messageArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_6);
    out << *this;
    return messageArray;
}

QDataStream &QMsgFileBlock::write(QDataStream &out) const
{
    st_ResultFile stMessage;
    bzero(&stMessage, sizeof(stMessage));
    m_folderName->toChar(stMessage.szFolderName, sizeof(stMessage.szFolderName));
    m_fileName->toChar(stMessage.szFileName, sizeof(stMessage.szFileName));
    stMessage.nBlockIndex = m_blockIndex;
    stMessage.nBlockLength = m_blockLength;
    out.writeRawData((const char *)&stMessage, sizeof(stMessage));
    out.writeRawData((const char *)m_block.data(), m_blockLength);
    return out;
}

QMsgFileBlock::QMsgFileBlock(QObject *parent)
    : QAbstractBinMsg(eFileBlock, parent), m_folderName(nullptr),
      m_fileName(nullptr)
{
    initAttributes();
}

QMsgFileBlock::QMsgFileBlock(const QMsgFileBlock
                             &message)
    : QAbstractBinMsg(message), m_folderName(nullptr), m_fileName(nullptr)
{
    initAttributes();
}

const QString QMsgFileBlock::folderName()
{
    return m_folderName->value().toString();
}
void QMsgFileBlock::setFolderName(QString folderName)
{
    m_folderName->setValue(QVariant(folderName));
}

const QString QMsgFileBlock::fileName()
{
    return m_fileName->value().toString();
}
void QMsgFileBlock::setFileName(QString fileName)
{
    m_fileName->setValue(QVariant(fileName));
}
quint32 QMsgFileBlock::blockIndex() const
{
    return m_blockIndex;
}
void QMsgFileBlock::setBlockIndex(quint32 blockIndex)
{
    m_blockIndex = blockIndex;
}

quint32 QMsgFileBlock::blockLength() const
{
    return m_blockLength;
}
void QMsgFileBlock::setBlockLength(quint32 blockLength)
{
    m_blockLength = blockLength;
}

const QByteArray QMsgFileBlock::block() const
{
    return m_block.toBase64();
}
void QMsgFileBlock::setBlock(QByteArray block)
{
    m_block = QByteArray::fromBase64(block);
}

void QMsgFileBlock::initAttributes()
{
    m_folderName = new QStringAttribute(this);
    m_fileName = new QStringAttribute(this);
}

QMsgFileBlock::~QMsgFileBlock()
{
    delete m_folderName;
    delete m_fileName;
}


void QMsgFileBlock::getFolderName(char *string, int length) const
{
    m_folderName->toChar(string, length);
}

void QMsgFileBlock::getFileName(char *string, int length) const
{
    m_fileName->toChar(string, length);
}

void QMsgFileBlock::clearBlock()
{
    m_block.clear();
}

void QMsgFileBlock::resizeBlock(int size)
{
    m_block.resize(size);
}

const char *QMsgFileBlock::blockData() const
{
    return m_block.data();
}

char *QMsgFileBlock::blockData()
{
    return m_block.data();
}

