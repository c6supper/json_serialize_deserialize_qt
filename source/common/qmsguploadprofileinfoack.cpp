/*
 * qmsguploadprofileinfoack.cpp

 *
 *  Created on: Sep 15, 2020
 *      Author: duyi
 */

#include "qmessagefactory.h"
#include "qmsguploadprofileinfoack.h"

using namespace QRserver;
using namespace QProfile;
using namespace rserver;

QMessageSelfRegisteration<QMsgUploadProfileInfoAck>
registerQMsgUploadProfileInfoAck(
    eUploadProfileInfoAck);

QMsgUploadProfileInfoAck::QMsgUploadProfileInfoAck(QObject *parent,
        bool ignoreStatus)
    : QAbstractBinMsg(eUploadProfileInfoAck, parent), m_ignoreStatus(ignoreStatus),
      m_uploadProfileInfoStatus(QMsgUploadProfileInfoAck::Failed)
{
}

QMsgUploadProfileInfoAck::QMsgUploadProfileInfoAck(const
        QMsgUploadProfileInfoAck
        &message)
    : QAbstractBinMsg(message)
{
}

qint8 QMsgUploadProfileInfoAck::uploadProfileInfoStatus() const
{
    return m_uploadProfileInfoStatus;
}

void QMsgUploadProfileInfoAck::setUploadProfileInfoStatus(qint8
        uploadProfileInfoStatus)
{
    m_uploadProfileInfoStatus = uploadProfileInfoStatus;
}

QDataStream &QMsgUploadProfileInfoAck::read(QDataStream &in)
{
    qint8 uploadProfileInfoStatus;

    if ((qint32)sizeof(uploadProfileInfoStatus) <= in.readRawData((
                char *)&uploadProfileInfoStatus,
            sizeof(uploadProfileInfoStatus))) {
        m_uploadProfileInfoStatus = uploadProfileInfoStatus;
    }

    return in;
}

const QByteArray QMsgUploadProfileInfoAck::toByteArray() const
{
    return QByteArray();
}

QDataStream &QMsgUploadProfileInfoAck::write(QDataStream &out) const
{
    return out;
}

const QStringList QMsgUploadProfileInfoAck::ignoredProperties() const
{
    QStringList list(QAbstractJsonElement::ignoredProperties());

    //We do not know which status the server will send to us, so we ignore this property to match the transition
    if (m_ignoreStatus)
        list << "uploadProfileInfoStatus";

    return list;
}

QMsgUploadProfileInfoAck::~QMsgUploadProfileInfoAck()
{
}




