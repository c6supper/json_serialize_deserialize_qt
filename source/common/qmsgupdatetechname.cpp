#include "qmessagefactory.h"
#include "qmsgupdatetechname.h"

using namespace QRserver;
using namespace QProfile;
using namespace rserver;

QMessageSelfRegisteration<QMsgUpdateTechName> registerQMsgUpdateTechName(
    eUpdateTechName);

QMsgUpdateTechName::QMsgUpdateTechName(QObject *parent)
    : QAbstractBinMsg(eUpdateTechName, parent), m_updataTechNameResult(false)
{
    initAttributes();
}

QMsgUpdateTechName::QMsgUpdateTechName(const QMsgUpdateTechName
                                       &message)
    : QAbstractBinMsg(message)
{
    initAttributes();
}

void QMsgUpdateTechName::initAttributes()
{
    m_techName = QSharedPointer<QStringAttribute>(new QStringAttribute(this));
}

QString QMsgUpdateTechName::techName() const
{
    return m_techName->value().toString();
}
void QMsgUpdateTechName::setTechName(QString techName)
{
    m_techName->setValue(techName);
}

bool QMsgUpdateTechName::updataTechNameResult() const
{
    return m_updataTechNameResult;
}

void QMsgUpdateTechName::setUpdataTechNameResult(bool
        updataTechNameResult)
{
    m_updataTechNameResult = updataTechNameResult;
}

QDataStream &QMsgUpdateTechName::read(QDataStream &in)
{
    //The message type of sending and receiving is the same.
    //When sending we use struct st_TechNameInfo , and one byte is used for receiving.
    //The derived class QMsgUpdateTechNameResult is only used for function distinction.
    quint8 updataTechNameResult;

    if ((qint32)sizeof(updataTechNameResult) <= in.readRawData((
                char *)&updataTechNameResult,
            sizeof(updataTechNameResult))) {
        m_updataTechNameResult = (updataTechNameResult == 1);
    }

    return in;
}

const QByteArray QMsgUpdateTechName::toByteArray() const
{
    st_TechNameInfo stMessage;
    bzero(&stMessage, sizeof(st_TechNameInfo));
    QByteArray messageArray((const char *)&stMessage, sizeof(stMessage));
    QDataStream out(&messageArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_6);
    out << *this;
    return messageArray;
}

QDataStream &QMsgUpdateTechName::write(QDataStream &out) const
{
    st_TechNameInfo stMessage;
    bzero(&stMessage, sizeof(stMessage));
    m_serialNumber->toChar(stMessage.SerialNumber, sizeof(stMessage.SerialNumber));
    m_techName->toChar(stMessage.TechName, sizeof(stMessage.TechName));
    out.writeRawData((const char *)&stMessage, sizeof(stMessage));
    return out;
}

QMsgUpdateTechName::~QMsgUpdateTechName()
{
}
