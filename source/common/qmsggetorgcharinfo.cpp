/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qmessagefactory.h"
#include "qmsggetorgcharinfo.h"

using namespace QRserver;
using namespace QProfile;
using namespace rserver;

QMessageSelfRegisteration<QMsgGetOrgChartInfo> registerQMsgGetOrgChartInfo(
    rserver::eGetOrgChartInfo);

QDataStream &QMsgGetOrgChartInfo::read(QDataStream &in)
{
    st_OrgChartInfo stMessage;

    if ((qint32)sizeof(stMessage) <= in.readRawData((char *)&stMessage,
            sizeof(stMessage))) {
        m_region->setValue(QVariant(stMessage.szRegion));
        m_district->setValue(QVariant(stMessage.szDistrict));
        m_system->setValue(QVariant(stMessage.szSystem));
    }

    return in;
}

const QByteArray QMsgGetOrgChartInfo::toByteArray() const
{
    st_SupervisorID orgCharInfo;
    bzero(&orgCharInfo, sizeof(orgCharInfo));
    QByteArray messageArray((const char *)&orgCharInfo, sizeof(orgCharInfo));
    QDataStream out(&messageArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_6);
    out << *this;
    return messageArray;
}

QDataStream &QMsgGetOrgChartInfo::write(QDataStream &out) const
{
    st_SupervisorID orgCharInfo;
    bzero(&orgCharInfo, sizeof(orgCharInfo));
    m_supervisorId->toChar(orgCharInfo.supervisorId,
                           sizeof(orgCharInfo.supervisorId));
    out.writeRawData((const char *)&orgCharInfo.supervisorId,
                     sizeof(orgCharInfo.supervisorId));
    return out;
}

QMsgGetOrgChartInfo::QMsgGetOrgChartInfo(QObject *parent)
    : QAbstractBinMsg(rserver::eGetOrgChartInfo, parent)
{
    initAttributes();
}

QMsgGetOrgChartInfo::QMsgGetOrgChartInfo(const QMsgGetOrgChartInfo &message)
    : QAbstractBinMsg(message)
{
    initAttributes();
}

void QMsgGetOrgChartInfo::initAttributes()
{
    m_region = QSharedPointer<QStringAttribute>(new QStringAttribute(this));
    m_district = QSharedPointer<QStringAttribute>(new QStringAttribute(this));
    m_system = QSharedPointer<QStringAttribute>(new QStringAttribute(this));
    m_supervisorId = QSharedPointer<QStringAttribute>(new QStringAttribute(this));
}

QMsgGetOrgChartInfo::~QMsgGetOrgChartInfo() {}
QString QMsgGetOrgChartInfo::supervisorId() const
{
    return m_supervisorId->value().toString();
}
void QMsgGetOrgChartInfo::setSupervisorId(QString supervisorId)
{
    m_supervisorId->setValue(supervisorId);
}
QString QMsgGetOrgChartInfo::district() const
{
    return m_district->value().toString();
}
void QMsgGetOrgChartInfo::setDistrict(QString district)
{
    m_district->setValue(district);
}
QString QMsgGetOrgChartInfo::system() const
{
    return m_system->value().toString();
}
void QMsgGetOrgChartInfo::setSystem(QString system)
{
    m_system->setValue(system);
}
QString QMsgGetOrgChartInfo::region() const
{
    return m_region->value().toString();
}
void QMsgGetOrgChartInfo::setRegion(QString region)
{
    m_region->setValue(region);
}

