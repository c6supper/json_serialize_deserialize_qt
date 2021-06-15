/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qdownloadstatus.h"
#include "qobjectfactory.h"

using namespace QRserver;
using namespace QProfile;

QObjectSelfRegisteration<QDownloadStatus> registerQDownloadStatus;

QDownloadStatus::QDownloadStatus(QObject *parent): QAbstractStatus(
        parent)
{
}

QDownloadStatus::QDownloadStatus(const QVariant &statusVar) :
    QAbstractStatus(statusVar)
{
}

QDownloadStatus::QDownloadStatus(QObject *parent, const QString &id,
                                 const quint32 commandStatus)
    : QAbstractStatus(parent, id, "", commandStatus)
{
}

const QVariant QDownloadStatus::fileModel()
{
    return m_fileModelVar;
}
void QDownloadStatus::setFileModel(const QVariant &fileModel)
{
    m_fileModelVar = fileModel;
}

QDownloadStatus::~QDownloadStatus()
{
}
