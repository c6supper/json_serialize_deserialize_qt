#include "qmessagefactory.h"
#include "qmsgtestsetstatus.h"

using namespace QRserver;
using namespace QProfile;
using namespace rserver;

QMessageSelfRegisteration<QMsgTestsetStatus>
registerQMsgTestsetStatus(rserver::eTestsetStatus);

QMsgTestsetStatus::QMsgTestsetStatus(QObject *parent)
    : QAbstractBinMsg(rserver::eTestsetStatus, parent)
{
    initAttributes();
}

QMsgTestsetStatus::QMsgTestsetStatus(const QMsgTestsetStatus
                                     &message)
    : QAbstractBinMsg(message)
{
    initAttributes();
}

QMsgTestsetStatus::~QMsgTestsetStatus()
{
}

void QMsgTestsetStatus::initAttributes()
{
    m_serialNumber = QSharedPointer<QStringAttribute>(new QStringAttribute(this));
    m_testsetStatus = QSharedPointer<QNumericAttribute>(new QNumericAttribute(0,
                      QVariant::Int, this));
}

QDataStream &QMsgTestsetStatus::read(QDataStream &in)
{
    st_TestsetStatus stMessage;

    if ((qint32)sizeof(stMessage) <= in.readRawData((char *)&stMessage,
            sizeof(stMessage))) {
        m_serialNumber->setValue(QVariant(stMessage.serialNum));
        m_testsetStatus->setValue(QVariant(stMessage.testsetStatus));
    }

    return in;
}

QDataStream &QMsgTestsetStatus::write(QDataStream &out) const
{
    st_TestsetStatus stMessage;
    bzero(&stMessage, sizeof(stMessage));
    m_serialNumber->toChar(stMessage.serialNum, sizeof(stMessage.serialNum));
    stMessage.testsetStatus = m_testsetStatus->value().toInt();
    out.writeRawData((const char *)&stMessage, sizeof(stMessage));
    qprofileDebug(QtDebugMsg) << toJson();
    return out;
}

const QByteArray QMsgTestsetStatus::toByteArray() const
{
    st_TestsetStatus stMessage;
    bzero(&stMessage, sizeof(st_TestsetStatus));
    QByteArray messageArray((const char *)&stMessage, sizeof(stMessage));
    QDataStream out(&messageArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_6);
    out << *this;
    return messageArray;
}

QString QMsgTestsetStatus::serialNumber() const
{
    return m_serialNumber->value().toString();
}
void QMsgTestsetStatus::setSerialNumber(QString serialNumber)
{
    m_serialNumber->setValue(serialNumber);
}

qint32 QMsgTestsetStatus::testsetStatus() const
{
    return m_testsetStatus->value().toInt();
}
void QMsgTestsetStatus::setTestsetStatus(qint32 status)
{
    m_testsetStatus->setValue(status);
}


