
#include "qmessagefactory.h"
#include "qmsguploadprofileblocklongname.h"

using namespace QRserver;
using namespace QProfile;
using namespace rserver;

QMessageSelfRegisteration<QMsgUploadProfileBlockLongName>
registerQMsgUploadProfileBlockLongName(
    rserver::eUploadProfileBlockLongName);

QDataStream &QMsgUploadProfileBlockLongName::read(QDataStream &in)
{
    st_ProfileFileLongName stMessage;

    if ((qint32)sizeof(stMessage) <= in.readRawData((char *)&stMessage,
            sizeof(stMessage))) {
        //to read file information
        setProfileName(QString(stMessage.szName));
        setProfileType(stMessage.nType);
        setBlockIndex(stMessage.nIndex);
        setTotalLength(stMessage.nLength);
        m_block.clear();
        m_block.resize(blockLength());

        //to read file content, so if the length of filename changed, this MSG object must be changed accordingly.
        if (blockLength() != (quint32)in.readRawData(m_block.data(), blockLength())) {
            qprofileDebug(QtWarningMsg) << "got invalid a message, length = " <<
                                        blockLength();
        }
    }

    return in;
}

const QByteArray QMsgUploadProfileBlockLongName::toByteArray() const
{
    QByteArray messageArray(sizeof(st_ProfileFileLongName) + blockLength(), 0);
    QDataStream out(&messageArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_6);
    out << *this;
    return messageArray;
}

QDataStream &QMsgUploadProfileBlockLongName::write(QDataStream &out) const
{
    st_ProfileFileLongName stMessage;
    bzero(&stMessage, sizeof(stMessage));
    m_profileName->toChar(stMessage.szName, sizeof(stMessage.szName));
    stMessage.nType = profileType();
    stMessage.nIndex = blockIndex();
    stMessage.nLength = totalLength();
    out.writeRawData((const char *)&stMessage, sizeof(stMessage));
    out.writeRawData((const char *)m_block.data(), blockLength());
    return out;
}

QMsgUploadProfileBlockLongName::QMsgUploadProfileBlockLongName(QObject *parent)
    : QMsgUploadProfileBlock(parent)
{
    setType(eUploadProfileBlockLongName);
}

QMsgUploadProfileBlockLongName::QMsgUploadProfileBlockLongName(
    const QMsgUploadProfileBlockLongName &message) :
    QMsgUploadProfileBlock(message)
{
    setType(eUploadProfileBlockLongName);
}

QMsgUploadProfileBlockLongName::~QMsgUploadProfileBlockLongName()
{
}
