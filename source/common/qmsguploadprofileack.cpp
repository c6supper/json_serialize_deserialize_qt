#include "qmessagefactory.h"
#include "qmsguploadprofileack.h"

using namespace QRserver;
using namespace rserver;
using namespace QProfile;

QMessageSelfRegisteration<QMsgUploadProfileAck> registerUploadProfileAck(
    rserver::eUploadProfileAck);

QMsgUploadProfileAck::QMsgUploadProfileAck(QObject *parent, bool ignoreStatus)
    : QMsgUploadResultAck(parent, ignoreStatus)
{
    setType(eUploadProfileAck);
}

QMsgUploadProfileAck::QMsgUploadProfileAck(const QMsgUploadProfileAck
        &message)
    : QMsgUploadResultAck(message)
{
    setType(eUploadProfileAck);
}

QMsgUploadProfileAck::~QMsgUploadProfileAck()
{
}

QDataStream &QMsgUploadProfileAck::read(QDataStream &in)
{
    st_TestsetProfileAck stMessage;

    if ((qint32)sizeof(stMessage) <= in.readRawData((char *)&stMessage,
            sizeof(stMessage))) {
        m_resultName->setValue(QVariant(stMessage.szProfileName));
        setResultType(stMessage.nProfileType);
        setResultStatus(stMessage.nProfileStatus);
        //Actually,the members, functions and properties in this class should be named like m_profileName, m_profileType and m_profileStatus.
        //But to make use of inheritance.
    }

    return in;
}

QDataStream &QMsgUploadProfileAck::write(QDataStream &out) const
{
    /*    st_TestsetProfileAck stMessage;
        bzero(&stMessage, sizeof(stMessage));
        m_resultName->toChar(stMessage.szProfileName, sizeof(stMessage.szProfileName));
        stMessage.nProfileType = resultType();
        stMessage.nProfileStatus = resultStatus();
        out.writeRawData((const char *)&stMessage, sizeof(stMessage));*/
    return out;
}

const QByteArray QMsgUploadProfileAck::toByteArray() const
{
    st_TestsetProfileAck stMessage;
    bzero(&stMessage, sizeof(st_TestsetProfileAck));
    QByteArray messageArray((const char *)&stMessage,
                            sizeof(stMessage));
    QDataStream out(&messageArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_6);
    out << *this;
    return messageArray;
}





