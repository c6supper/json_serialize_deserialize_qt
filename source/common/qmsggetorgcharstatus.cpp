/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qmessagefactory.h"
#include "qmsggetorgcharstatus.h"

using namespace QRserver;
using namespace QProfile;
using namespace rserver;

QMessageSelfRegisteration<QMsgGetOrgChartStatus> registerQMsgGetOrgChartStatus(
    rserver::eGetOrgChartStatus);

QDataStream &QMsgGetOrgChartStatus::read(QDataStream &in)
{
    st_OrgChartStatus stMessage;

    if ((qint32)sizeof(stMessage) <= in.readRawData((char *)&stMessage,
            sizeof(stMessage))) {
        m_status->setValue(QVariant(stMessage.status).toUInt());
    }

    return in;
}

const QByteArray QMsgGetOrgChartStatus::toByteArray() const
{
    st_OrgChartStatus orgCharStatus;
    bzero(&orgCharStatus, sizeof(orgCharStatus));
    QByteArray messageArray((const char *)&orgCharStatus, sizeof(orgCharStatus));
    QDataStream out(&messageArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_6);
    out << *this;
    return messageArray;
}

QDataStream &QMsgGetOrgChartStatus::write(QDataStream &out) const
{
    return out;
}

QMsgGetOrgChartStatus::QMsgGetOrgChartStatus(QObject *parent)
    : QAbstractBinMsg(rserver::eGetOrgChartStatus, parent)
{
    initAttributes();
}

QMsgGetOrgChartStatus::QMsgGetOrgChartStatus(const QMsgGetOrgChartStatus
        &message)
    : QAbstractBinMsg(message)
{
    initAttributes();
}

void QMsgGetOrgChartStatus::initAttributes()
{
    m_status = QSharedPointer<QNumericAttribute>(new QNumericAttribute(this));
}

QMsgGetOrgChartStatus::~QMsgGetOrgChartStatus() {}

quint32 QMsgGetOrgChartStatus::status() const
{
    return m_status->value().toUInt();
}
void QMsgGetOrgChartStatus::setStatus(quint32 status)
{
    m_status->setValue(status);
}

