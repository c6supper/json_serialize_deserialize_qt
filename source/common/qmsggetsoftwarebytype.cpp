/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qmessagefactory.h"
#include "qmsggetsoftwarebytype.h"

using namespace QRserver;
using namespace QProfile;
using namespace rserver;

QMessageSelfRegisteration<QMsgGetSoftwareByType> registerQMsgGetSoftwareByType(
    rserver::eGetSoftwareByType);

QDataStream &QMsgGetSoftwareByType::read(QDataStream &in)
{
    st_TestsetSWVersionInfo stMessage;

    if ((qint32)sizeof(stMessage) <= in.readRawData((char *)&stMessage,
            sizeof(stMessage))) {
        m_softwareVersion->setValue(QVariant(stMessage.CurrentVersion));
        m_deviceType->setValue(QVariant(stMessage.Type));
    }

    qprofileDebug(QtDebugMsg) << toJson();
    return in;
}

const QByteArray QMsgGetSoftwareByType::toByteArray() const
{
    qint32 deviceType = 0;
    QByteArray messageArray((const char *)&deviceType, sizeof(deviceType));
    QDataStream out(&messageArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_6);
    out << *this;
    return messageArray;
}

QDataStream &QMsgGetSoftwareByType::write(QDataStream &out) const
{
    qint32 deviceType = m_deviceType->value().toInt();
    out.writeRawData((const char *)&deviceType, sizeof(deviceType));
    qprofileDebug(QtDebugMsg) << toJson();
    return out;
}

QMsgGetSoftwareByType::QMsgGetSoftwareByType(QObject *parent)
    : QAbstractBinMsg(rserver::eGetSoftwareByType, parent)
{
    initAttributes();
}

QMsgGetSoftwareByType::QMsgGetSoftwareByType(const QMsgGetSoftwareByType
        &message)
    : QAbstractBinMsg(message)
{
    initAttributes();
}

void QMsgGetSoftwareByType::initAttributes()
{
    m_softwareVersion = QSharedPointer<QStringAttribute>(new QStringAttribute(
                            this));
    m_deviceType = QSharedPointer<QNumericAttribute>(new QNumericAttribute(0,
                   QVariant::Int, this));
}

QMsgGetSoftwareByType::~QMsgGetSoftwareByType() {}

QString QMsgGetSoftwareByType::softwareVersion() const
{
    return m_softwareVersion->value().toString();
}
void QMsgGetSoftwareByType::setSoftwareVersion(QString softwareVersion)
{
    m_softwareVersion->setValue(softwareVersion);
}

qint32 QMsgGetSoftwareByType::deviceType() const
{
    return m_deviceType->value().toInt();
}
void QMsgGetSoftwareByType::setDeviceType(qint32 deviceType)
{
    m_deviceType->setValue(deviceType);
}

