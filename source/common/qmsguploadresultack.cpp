#include "qmessagefactory.h"
#include "qmsguploadresultack.h"

using namespace QRserver;
using namespace rserver;
using namespace QProfile;

QMessageSelfRegisteration<QMsgUploadResultAck> registerUploadResultAck(
    rserver::eUploadResultAck);

QMsgUploadResultAck::QMsgUploadResultAck(QObject *parent, bool ignoreStatus)
    : QAbstractBinMsg(rserver::eUploadResultAck, parent),
      m_ignoreStatus(ignoreStatus), m_resultName(nullptr)
{
    initAttributes();
}

QMsgUploadResultAck::QMsgUploadResultAck(const QMsgUploadResultAck &message)
    : QAbstractBinMsg(message), m_resultName(nullptr)
{
    initAttributes();
}

QMsgUploadResultAck::~QMsgUploadResultAck()
{
    if (m_resultName != nullptr) {
        delete m_resultName;
    }
}

const QString QMsgUploadResultAck::resultName()
{
    return m_resultName->value().toString();
}
void QMsgUploadResultAck::setResultName(QString resultName)
{
    m_resultName->setValue(QVariant(resultName));
}

qint32 QMsgUploadResultAck::resultType() const
{
    return m_resultType;
}
void QMsgUploadResultAck::setResultType(qint32 resultType)
{
    m_resultType = resultType;
}

qint32 QMsgUploadResultAck::resultStatus() const
{
    return m_resultStatus;
}
void QMsgUploadResultAck::setResultStatus(qint32 resultStatus)
{
    m_resultStatus = resultStatus;
}

QDataStream &QMsgUploadResultAck::read(QDataStream &in)
{
    st_ResultAck stMessage;

    if ((qint32)sizeof(stMessage) <= in.readRawData((char *)&stMessage,
            sizeof(stMessage))) {
        m_resultName->setValue(QVariant(stMessage.szResultName));
        m_resultType = stMessage.nResultType;
        m_resultStatus = stMessage.nResultStatus;
    }

    return in;
}

QDataStream &QMsgUploadResultAck::write(QDataStream &out) const
{
    /*    st_ResultAck stMessage;
        bzero(&stMessage, sizeof(stMessage));
        m_resultName->toChar(stMessage.szResultName, sizeof(stMessage.szResultName));
        stMessage.nResultType = m_resultType;
        stMessage.nResultStatus = m_resultStatus;
        out.writeRawData((const char *)&stMessage, sizeof(stMessage));*/
    return out;
}

const QByteArray QMsgUploadResultAck::toByteArray() const
{
    st_ResultAck stMessage;
    bzero(&stMessage, sizeof(st_ResultAck));
    QByteArray messageArray((const char *)&stMessage,
                            sizeof(stMessage));
    QDataStream out(&messageArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_6);
    out << *this;
    return messageArray;
}

void QMsgUploadResultAck::initAttributes()
{
    m_resultName = new QStringAttribute(this);
}

const QStringList QMsgUploadResultAck::ignoredProperties() const
{
    QStringList list(QAbstractJsonElement::ignoredProperties());

    if (m_ignoreStatus)
        list << "resultStatus";

    return list;
}

