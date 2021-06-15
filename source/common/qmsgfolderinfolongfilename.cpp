#include "qmsgfolderinfolongfilename.h"
#include "qmessagefactory.h"

using namespace QRserver;
using namespace QProfile;
using namespace rserver;

QMessageSelfRegisteration<QMsgFolderInfoLongFileName>
registerQMsgFolderInfoLongFileName(
    eFolderInfoEx);

QMsgFolderInfoLongFileName::QMsgFolderInfoLongFileName(QObject *parent)
    : QMsgFolderInfo(parent)
{
    setType(eFolderInfoEx);
}

QMsgFolderInfoLongFileName::QMsgFolderInfoLongFileName(const
        QMsgFolderInfoLongFileName &message)
    : QMsgFolderInfo(message)
{
    setType(eFolderInfoEx);
}

QMsgFolderInfoLongFileName::~QMsgFolderInfoLongFileName()
{
}

QDataStream &QMsgFolderInfoLongFileName::read(QDataStream &in)
{
    st_ResultDirectoryEx stMessage;

    if ((qint32)sizeof(stMessage) <= in.readRawData((char *)&stMessage,
            sizeof(stMessage))) {
        setFolderName(QString(stMessage.szDirName));
        setFolderType(stMessage.nResultType);
        setFileCount(stMessage.nFileCount);
    }

    return in;
}

QDataStream &QMsgFolderInfoLongFileName::write(QDataStream &out) const
{
    st_ResultDirectoryEx stMessage;
    bzero(&stMessage, sizeof(stMessage));
    getFolderName(stMessage.szDirName, sizeof(stMessage.szDirName));
    stMessage.nResultType = folderType();
    stMessage.nFileCount = fileCount();
    out.writeRawData((const char *)&stMessage, sizeof(stMessage));
    return out;
}

const QByteArray QMsgFolderInfoLongFileName::toByteArray() const
{
    st_ResultDirectoryEx stMessage;
    bzero(&stMessage, sizeof(st_ResultDirectoryEx));
    QByteArray messageArray((const char *)&stMessage,
                            sizeof(stMessage));
    QDataStream out(&messageArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_6);
    out << *this;
    return messageArray;
}
