#include "qmsgfileblocklongfilename.h"
#include "qmessagefactory.h"

using namespace QRserver;
using namespace QProfile;
using namespace rserver;

QMessageSelfRegisteration<QMsgFileBlockLongFileName>
registerQMsgFileBlockLongFileName(eFileBlockEx);

QMsgFileBlockLongFileName::QMsgFileBlockLongFileName(QObject *parent)
    : QMsgFileBlock(parent)
{
    setType(eFileBlockEx);
}

QMsgFileBlockLongFileName::QMsgFileBlockLongFileName(const
        QMsgFileBlockLongFileName &message)
    : QMsgFileBlock(message)
{
    setType(eFileBlockEx);
}

QMsgFileBlockLongFileName::~QMsgFileBlockLongFileName()
{
}

QDataStream &QMsgFileBlockLongFileName::read(QDataStream &in)
{
    st_ResultFileEx stMessage;

    if ((qint32)sizeof(stMessage) <= in.readRawData((char *)&stMessage,
            sizeof(stMessage))) {
        setFolderName(QString(stMessage.szFolderName));
        setFileName(QString(stMessage.szFileName));
        setBlockIndex(stMessage.nBlockIndex);
        qint32 blockLen = stMessage.nBlockLength > MaxFileBlock ? MaxFileBlock :
                          stMessage.nBlockLength;
        setBlockLength(blockLen);

        if (stMessage.nBlockLength != blockLen && type() == eFileBlock) {
            qprofileDebug(QtWarningMsg) << "got invalid a message, length = " <<
                                        stMessage.nBlockLength;
        }

        clearBlock();
        resizeBlock(blockLen);

        if (blockLen != in.readRawData(blockData(), blockLen)) {
            qprofileDebug(QtWarningMsg) << "got invalid a message, length = " <<
                                        blockLen;
        }
    }

    return in;
}

QDataStream &QMsgFileBlockLongFileName::write(QDataStream &out) const
{
    st_ResultFileEx stMessage;
    bzero(&stMessage, sizeof(stMessage));
    getFolderName(stMessage.szFolderName, sizeof(stMessage.szFolderName));
    getFileName(stMessage.szFileName, sizeof(stMessage.szFileName));
    stMessage.nBlockIndex = blockIndex();
    stMessage.nBlockLength = blockLength();
    out.writeRawData((const char *)&stMessage, sizeof(stMessage));
    out.writeRawData((const char *)blockData(), blockLength());
    return out;
}

const QByteArray QMsgFileBlockLongFileName::toByteArray() const
{
    QByteArray messageArray(sizeof(st_ResultFileEx) + blockLength(), 0);
    QDataStream out(&messageArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_6);
    out << *this;
    return messageArray;
}


