
#include "qmessagefactory.h"
#include "qmsgblockprofileandchangetechname.h"

using namespace QRserver;
using namespace rserver;

QMessageSelfRegisteration<QMsgBlockProfileAndChangeTechName>
registerQMsgBlockProfileAndChangeTechName(
    rserver::eCheckAllExt3);

QMsgBlockProfileAndChangeTechName::QMsgBlockProfileAndChangeTechName(
    QObject *parent)
    : QAbstractBinMsg(rserver::eCheckAllExt3, parent), m_blockProfileUpload(false),
      m_changeTechName(false)
{
}

QMsgBlockProfileAndChangeTechName::QMsgBlockProfileAndChangeTechName(
    const QMsgBlockProfileAndChangeTechName &message)
    : QAbstractBinMsg(message)
{
}

QMsgBlockProfileAndChangeTechName::~QMsgBlockProfileAndChangeTechName() {}

bool QMsgBlockProfileAndChangeTechName::blockProfileUpload() const
{
    return m_blockProfileUpload;
}
void QMsgBlockProfileAndChangeTechName::setBlockProfileUpload(
    bool blockProfileUpload)
{
    m_blockProfileUpload = blockProfileUpload;
}

bool QMsgBlockProfileAndChangeTechName::changeTechName() const
{
    return m_changeTechName;
}
void QMsgBlockProfileAndChangeTechName::setChangeTechName(bool changeTechName)
{
    m_changeTechName = changeTechName;
}

QDataStream &QMsgBlockProfileAndChangeTechName::read(QDataStream &in)
{
    quint8 blockProfileAndChangeTechName[2];
    bzero(blockProfileAndChangeTechName, sizeof(blockProfileAndChangeTechName));

    if ((qint32)sizeof(blockProfileAndChangeTechName) <= in.readRawData(
            (char *)blockProfileAndChangeTechName, sizeof(blockProfileAndChangeTechName))) {
        m_blockProfileUpload = (blockProfileAndChangeTechName[0] == 1);
        m_changeTechName = (blockProfileAndChangeTechName[1] == 1);
    }

    return in;
}

const QByteArray QMsgBlockProfileAndChangeTechName::toByteArray() const
{
    char blockProfileAndChangeTechName[2];
    bzero(blockProfileAndChangeTechName, sizeof(blockProfileAndChangeTechName));
    QByteArray messageArray(blockProfileAndChangeTechName,
                            sizeof(blockProfileAndChangeTechName));
    QDataStream out(&messageArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_6);
    out << *this;
    return messageArray;
}

QDataStream &QMsgBlockProfileAndChangeTechName::write(QDataStream &out) const
{
    return out;
}
