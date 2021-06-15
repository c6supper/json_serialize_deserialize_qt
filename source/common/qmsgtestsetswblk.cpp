/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qmessagefactory.h"
#include "qmsgtestsetswblk.h"

using namespace QRserver;
using namespace QProfile;
using namespace rserver;

QMessageSelfRegisteration<QMsgTestsetSwBlk>
registerQMsgTestsetSwBlk(
    rserver::eTestsetSWBLK);

const qint32 QMsgTestsetSwBlk::MaxSwFileBlock = (30720);

QDataStream &QMsgTestsetSwBlk::read(QDataStream &in)
{
    m_block.clear();
    m_block.resize(MaxSwFileBlock);
    int length = in.readRawData(m_block.data(), MaxSwFileBlock);

    if (length < MaxSwFileBlock)
        m_block.resize(length);

    if (((MaxSwFileBlock != length) && (type() == eTestsetSWBLK)) || (length < 0)) {
        qprofileDebug(QtWarningMsg) <<
                                    "<QMsgTestsetSwBlk>got invalid a message, length = "
                                    << length;
    }

    return in;
}

const QByteArray QMsgTestsetSwBlk::toByteArray() const
{
    QByteArray messageArray(MaxSwFileBlock, 0);
    QDataStream out(&messageArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_6);
    out << *this;
    return messageArray;
}

QDataStream &QMsgTestsetSwBlk::write(QDataStream &out) const
{
    //    out.writeRawData((const char *)m_block.data(), MaxSwFileBlock);
    return out;
}

QMsgTestsetSwBlk::QMsgTestsetSwBlk(QObject *parent)
    : QAbstractBinMsg(rserver::eTestsetSWBLK, parent)
{
    initAttributes();
}

QMsgTestsetSwBlk::QMsgTestsetSwBlk(const QMsgTestsetSwBlk
                                   &message)
    : QAbstractBinMsg(message)
{
    initAttributes();
}

const QByteArray QMsgTestsetSwBlk::block() const
{
    return m_block.toBase64();
}
void QMsgTestsetSwBlk::setBlock(QByteArray block)
{
    m_block = QByteArray::fromBase64(block);
}

const qint32 QMsgTestsetSwBlk::length() const
{
    return m_block.size();
}

void QMsgTestsetSwBlk::initAttributes()
{
}

QMsgTestsetSwBlk::~QMsgTestsetSwBlk()
{
}


