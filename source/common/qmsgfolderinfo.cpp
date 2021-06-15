/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qmessagefactory.h"
#include "qmsgfolderinfo.h"

using namespace QRserver;
using namespace QProfile;
using namespace rserver;

QMessageSelfRegisteration<QMsgFolderInfo> registerQMsgFolderInfo(eFolderInfo);

QDataStream &QMsgFolderInfo::read(QDataStream &in)
{
    st_ResultDirectory stMessage;

    if ((qint32)sizeof(stMessage) <= in.readRawData((char *)&stMessage,
            sizeof(stMessage))) {
        m_folderName->setValue(QVariant(stMessage.szDirName));
        m_folderType = stMessage.nResultType;
        m_fileCount = stMessage.nFileCount;
    }

    return in;
}

const QByteArray QMsgFolderInfo::toByteArray() const
{
    st_ResultDirectory stMessage;
    bzero(&stMessage, sizeof(st_ResultDirectory));
    QByteArray messageArray((const char *)&stMessage,
                            sizeof(stMessage));
    QDataStream out(&messageArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_6);
    out << *this;
    return messageArray;
}

QDataStream &QMsgFolderInfo::write(QDataStream &out) const
{
    st_ResultDirectory stMessage;
    bzero(&stMessage, sizeof(stMessage));
    m_folderName->toChar(stMessage.szDirName, sizeof(stMessage.szDirName));
    stMessage.nResultType = m_folderType;
    stMessage.nFileCount = m_fileCount;
    out.writeRawData((const char *)&stMessage, sizeof(stMessage));
    return out;
}

QMsgFolderInfo::QMsgFolderInfo(QObject *parent)
    : QAbstractBinMsg(eFolderInfo, parent), m_folderName(nullptr)
{
    initAttributes();
}

QMsgFolderInfo::QMsgFolderInfo(const QMsgFolderInfo
                               &message)
    : QAbstractBinMsg(message), m_folderName(nullptr)
{
    initAttributes();
}

const QString QMsgFolderInfo::folderName()
{
    return m_folderName->value().toString();
}
void QMsgFolderInfo::setFolderName(QString folderName)
{
    m_folderName->setValue(QVariant(folderName));
}

quint32 QMsgFolderInfo::folderType() const
{
    return m_folderType;
}
void QMsgFolderInfo::setFolderType(quint32 folderType)
{
    m_folderType = folderType;
}

quint32 QMsgFolderInfo::fileCount() const
{
    return m_fileCount;
}
void QMsgFolderInfo::setFileCount(quint32 fileCount)
{
    m_fileCount = fileCount;
}

void QMsgFolderInfo::initAttributes()
{
    m_folderName = new QStringAttribute(this);
}

QMsgFolderInfo::~QMsgFolderInfo()
{
    delete m_folderName;
}

void QMsgFolderInfo::getFolderName(char *string, int length) const
{
    m_folderName->toChar(string, length);
}
