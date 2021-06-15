

/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QDOWNLOADSTATUS_H
#define QDOWNLOADSTATUS_H

#include "qabstractstatus.h"
#include "qfilemodel.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QRserver
{
    class QDownloadStatus : public QProfile::QAbstractStatus
    {
            Q_OBJECT

            Q_ENUMS(DownloadStatus)
            Q_PROPERTY(QVariant fileModel READ fileModel WRITE setFileModel)

        public:
            enum DownloadStatus { DecryptFailed = QProfile::QAbstractStatus::StatusButtom, FileSizeMismatch, LockDownFailed, UnlockDownFailed, DeleteLastLockdownProfFailed};

        public:
            QDownloadStatus(QObject *parent = nullptr);
            QDownloadStatus(const QVariant &statusVar);
            QDownloadStatus(QObject *parent, const QString &id,
                            const quint32 commandStatus = QProfile::QAbstractStatus::Failed);
            ~QDownloadStatus();

            const QVariant fileModel();
            void setFileModel(const QVariant &fileModel);

        private:
            QVariant m_fileModelVar;
    };
}

Q_DECLARE_METATYPE(QRserver::QDownloadStatus)

#endif // QDOWNLOADSTATUS_H
