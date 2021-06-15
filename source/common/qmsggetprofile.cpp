/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qmessagefactory.h"
#include "qmsggetprofile.h"

using namespace QRserver;
using namespace QProfile;
using namespace rserver;

QMessageSelfRegisteration<QMsgGetProfile> registerQMsgGetProfile(
    rserver::eGetProfileFile);

QDataStream &QMsgGetProfile::read(QDataStream &in)
{
    return in;
}

const QByteArray QMsgGetProfile::toByteArray() const
{
    st_GetProfileFile profile;
    bzero(&profile, sizeof(st_GetProfileFile));
    QByteArray messageArray((const char *) &profile, sizeof(profile));
    QDataStream out(&messageArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_6);
    out << *this;
    return messageArray;
}

QDataStream &QMsgGetProfile::write(QDataStream &out) const
{
    st_GetProfileFile stMessage;
    bzero(&stMessage, sizeof(stMessage));
    stMessage.Type = (qint32)m_fileType->value().toUInt();
    m_profileName->toChar(stMessage.Name, sizeof(stMessage.Name));
    out.writeRawData((const char *) &stMessage, sizeof(stMessage));
    return out;
}

QMsgGetProfile::QMsgGetProfile(QObject *parent) :
    QAbstractBinMsg(rserver::eGetProfileFile, parent)
{
    initAttributes();
}

QMsgGetProfile::QMsgGetProfile(const QMsgGetProfile &message) :
    QAbstractBinMsg(message)
{
    initAttributes();
}

void QMsgGetProfile::initAttributes()
{
    m_profileName = QSharedPointer < QStringAttribute
                    > (new QStringAttribute(this));
    m_fileType = QSharedPointer < QNumericAttribute
                 > (new QNumericAttribute(0, QVariant::Int, this));
}

QMsgGetProfile::~QMsgGetProfile()
{
}

QString QMsgGetProfile::profileName() const
{
    return m_profileName->value().toString();
}
void QMsgGetProfile::setProfileName(QString profileName)
{
    m_profileName->setValue(profileName);
}
quint32 QMsgGetProfile::fileType() const
{
    return m_fileType->value().toUInt();
}
void QMsgGetProfile::setFileType(quint32 fileType)
{
    m_fileType->setValue(fileType);
}

