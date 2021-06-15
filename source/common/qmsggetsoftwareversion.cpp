/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qmessagefactory.h"
#include "qmsggetsoftwareversion.h"

using namespace QRserver;
using namespace QProfile;
using namespace rserver;

QMessageSelfRegisteration<QMsgGetSoftwareVersion>
registerQMsgGetSoftwareVersion(
    rserver::eSoftwareVersion);

QDataStream &QMsgGetSoftwareVersion::read(QDataStream &in)
{
    st_CurrentVerison stMessage;

    if ((qint32)sizeof(stMessage) <= in.readRawData((char *)&stMessage,
            sizeof(stMessage))) {
        m_softwareVersion->setValue(QVariant(stMessage.CurrentVersion));
        /*        m_deltaVersion->setValue(QVariant(stMessage.DeltaVersion));
                m_basicVersion->setValue(QVariant(stMessage.BasicVersion));
                m_deltaPkgSize->setValue(QVariant(stMessage.deltaPkgSize));
                m_fullPkgSize->setValue(QVariant(stMessage.fullPkgSize));*/
    }

    return in;
}

const QByteArray QMsgGetSoftwareVersion::toByteArray() const
{
    st_RServerSWVersionExt versionInfo;
    bzero(&versionInfo, sizeof(versionInfo));
    QByteArray messageArray((const char *)&versionInfo, sizeof(versionInfo));
    QDataStream out(&messageArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_6);
    out << *this;
    return messageArray;
}

QDataStream &QMsgGetSoftwareVersion::write(QDataStream &out) const
{
    return out;
}

QMsgGetSoftwareVersion::QMsgGetSoftwareVersion(QObject *parent)
    : QAbstractBinMsg(rserver::eSoftwareVersion, parent)
{
    initAttributes();
}

QMsgGetSoftwareVersion::QMsgGetSoftwareVersion(const QMsgGetSoftwareVersion
        &message)
    : QAbstractBinMsg(message)
{
    initAttributes();
}

void QMsgGetSoftwareVersion::initAttributes()
{
    m_softwareVersion = QSharedPointer<QStringAttribute>(new QStringAttribute(
                            this));
    /*    m_deltaVersion = QSharedPointer<QStringAttribute>(new QStringAttribute(this));
        m_basicVersion = QSharedPointer<QStringAttribute>(new QStringAttribute(this));
        m_deltaPkgSize = QSharedPointer<QNumericAttribute>(new QNumericAttribute(0,
                         QVariant::Int, this));
        m_fullPkgSize = QSharedPointer<QNumericAttribute>(new QNumericAttribute(0,
                        QVariant::Int, this));*/
}

QMsgGetSoftwareVersion::~QMsgGetSoftwareVersion() {}

QString QMsgGetSoftwareVersion::softwareVersion() const
{
    return m_softwareVersion->value().toString();
}
void QMsgGetSoftwareVersion::setSoftwareVersion(QString softwareVersion)
{
    m_softwareVersion->setValue(softwareVersion);
}

/*QString QMsgGetSoftwareVersion::deltaVersion() const
{
    return m_deltaVersion->value().toString();
}
void QMsgGetSoftwareVersion::setDeltaVersion(QString deltaVersion)
{
    m_deltaVersion->setValue(deltaVersion);
}

QString QMsgGetSoftwareVersion::basicVersion() const
{
    return m_basicVersion->value().toString();
}
void QMsgGetSoftwareVersion::setBasicVersion(QString basicVersion)
{
    m_basicVersion->setValue(basicVersion);
}

qint32 QMsgGetSoftwareVersion::fullPkgSize() const
{
    return m_fullPkgSize->value().toInt();
}
void QMsgGetSoftwareVersion::setFullPkgSize(qint32 fullPkgSize)
{
    m_fullPkgSize->setValue(fullPkgSize);
}

qint32 QMsgGetSoftwareVersion::deltaPkgSize() const
{
    return m_deltaPkgSize->value().toInt();
}
void QMsgGetSoftwareVersion::setDeltaPkgSize(qint32 deltaPkgSize)
{
    m_deltaPkgSize->setValue(deltaPkgSize);
}*/
