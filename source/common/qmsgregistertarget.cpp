/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qmessagefactory.h"
#include "qmsgregistertarget.h"

using namespace QRserver;
using namespace QProfile;
using namespace rserver;

QMessageSelfRegisteration<QMsgRegisterTarget> registerQMsgRegisterTarget(
    rserver::eRegisterTarget);

QDataStream &QMsgRegisterTarget::read(QDataStream &in)
{
    st_RserverDeviceInfo stMessage;

    if ((qint32)sizeof(stMessage) <= in.readRawData((char *)&stMessage,
            sizeof(stMessage))) {
        m_deviceType->setValue(QVariant::fromValue<quint32>(stMessage.cDeviceType));
        m_serialNumber->setValue(QVariant(stMessage.szSerialNum));
        m_mac->fromChar((char *)stMessage.Mac, sizeof(stMessage.Mac));
        m_cableMac->fromChar((char *)stMessage.CMMac, sizeof(stMessage.CMMac));
        m_version->setValue(QVariant(stMessage.szVersion));
        m_techId->setValue(QVariant(stMessage.szTechID));
        m_techName->setValue(QVariant(stMessage.TechnicianName));
        m_company->setValue(QVariant(stMessage.szCompany));
    }

    return in;
}

const QByteArray QMsgRegisterTarget::toByteArray() const
{
    st_RserverDeviceInfo device;
    bzero(&device, sizeof(st_RserverDeviceInfo));
    QByteArray messageArray((const char *)&device, sizeof(device));
    QDataStream out(&messageArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_6);
    out << *this;
    return messageArray;
}

QDataStream &QMsgRegisterTarget::write(QDataStream &out) const
{
    st_RserverDeviceInfo stMessage;
    bzero(&stMessage, sizeof(stMessage));
    stMessage.cDeviceType = (quint8)m_deviceType->value().toUInt();
    m_serialNumber->toChar(stMessage.szSerialNum, sizeof(stMessage.szSerialNum));
    m_mac->toChar((char *)stMessage.Mac);
    m_cableMac->toChar((char *)stMessage.CMMac);
    m_version->toChar(stMessage.szVersion, sizeof(stMessage.szVersion));
    m_techId->toChar(stMessage.szTechID, sizeof(stMessage.szTechID));
    m_techName->toChar(stMessage.TechnicianName, sizeof(stMessage.TechnicianName));
    m_company->toChar(stMessage.szCompany, sizeof(stMessage.szCompany));
    out.writeRawData((const char *)&stMessage,
                     sizeof(stMessage));
    return out;
}

QMsgRegisterTarget::QMsgRegisterTarget(QObject *parent)
    : QAbstractBinMsg(rserver::eRegisterTarget, parent)
{
    initAttributes();
}

QMsgRegisterTarget::QMsgRegisterTarget(const QMsgRegisterTarget
                                       &message)
    : QAbstractBinMsg(message)
{
    initAttributes();
}

void QMsgRegisterTarget::initAttributes()
{
    m_version = QSharedPointer<QStringAttribute>(new QStringAttribute(this));
    m_serialNumber = QSharedPointer<QStringAttribute>(new QStringAttribute(this));
    m_mac = QSharedPointer<QMacAttribute>(new QMacAttribute(this));
    m_cableMac = QSharedPointer<QMacAttribute>(new QMacAttribute(this));
    m_deviceType = QSharedPointer<QNumericAttribute>(new QNumericAttribute(0,
                   QVariant::Int, this));
    m_techId = QSharedPointer<QStringAttribute>(new QStringAttribute(this));
    m_techName = QSharedPointer<QStringAttribute>(new QStringAttribute(this));
    m_company = QSharedPointer<QStringAttribute>(new QStringAttribute(this));
}

QMsgRegisterTarget::~QMsgRegisterTarget() {}

QString QMsgRegisterTarget::techId() const
{
    return m_techId->value().toString();
}
void QMsgRegisterTarget::setTechId(QString techId)
{
    m_techId->setValue(techId);
}

QString QMsgRegisterTarget::techName() const
{
    return m_techName->value().toString();
}
void QMsgRegisterTarget::setTechName(QString techName)
{
    m_techName->setValue(techName);
}

QString QMsgRegisterTarget::company() const
{
    return m_company->value().toString();
}
void QMsgRegisterTarget::setCompany(QString company)
{
    m_company->setValue(company);
}
QString QMsgRegisterTarget::version() const
{
    return m_version->value().toString();
}
void QMsgRegisterTarget::setVersion(QString version)
{
    m_version->setValue(version);
}

QString QMsgRegisterTarget::serialNumber() const
{
    return m_serialNumber->value().toString();
}
void QMsgRegisterTarget::setSerialNumber(QString serialNumber)
{
    m_serialNumber->setValue(serialNumber);
}

QString QMsgRegisterTarget::mac() const
{
    return m_mac->value().toString();
}
void QMsgRegisterTarget::setMac(QString mac)
{
    m_mac->setValue(mac);
}

QString QMsgRegisterTarget::cableMac() const
{
    return m_cableMac->value().toString();
}
void QMsgRegisterTarget::setCableMac(QString cableMac)
{
    m_cableMac->setValue(cableMac);
}

quint32 QMsgRegisterTarget::deviceType() const
{
    return m_deviceType->value().toUInt();
}
void QMsgRegisterTarget::setDeviceType(quint32 deviceType)
{
    m_deviceType->setValue(deviceType);
}

void QMsgRegisterTarget::getMac(char *mac) const
{
    m_mac->toChar(mac);
}
void QMsgRegisterTarget::setMacFromChar(char *mac, int length)
{
    m_mac->fromChar(mac, length);
}

void QMsgRegisterTarget::getCableMac(char *mac) const
{
    m_cableMac->toChar(mac);
}
void QMsgRegisterTarget::setCableMacFromChar(char *mac, int length)
{
    m_cableMac->fromChar(mac, length);
}

