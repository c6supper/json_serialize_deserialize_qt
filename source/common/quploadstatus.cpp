/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "quploadstatus.h"
#include "qobjectfactory.h"

using namespace QRserver;
using namespace QProfile;

QObjectSelfRegisteration<QUploadStatus> registerQUploadStatus;

QUploadStatus::QUploadStatus(QObject *parent): QAbstractStatus(
        parent)
{
}

QUploadStatus::QUploadStatus(const QVariant &statusVar) :
    QAbstractStatus(statusVar)
{
    fromVariant(statusVar);
}

QUploadStatus::QUploadStatus(QObject *parent, const QString &id,
                             const quint32 commandStatus)
    : QAbstractStatus(parent, id, "", commandStatus)
{
}

const QVariant QUploadStatus::fileModel()
{
    return m_fileModelVar;
}

void QUploadStatus::setFileModel(const QVariant &fileModel)
{
    m_fileModelVar = fileModel;
}

bool QUploadStatus::canRecover()
{
    return (QAbstractStatus::canRecover()
            && (QAbstractStatus::CommandStatus) UploadXmlError
            != commandStatus()
            && (QAbstractStatus::CommandStatus) InvalidEncryptedFile
            != commandStatus());
}

QUploadStatus::~QUploadStatus()
{
}
