

/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QUPLOADSTATUS_H
#define QUPLOADSTATUS_H

#include "qabstractstatus.h"
#include "qfilemodel.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QRserver
{
    class QUploadStatus : public QProfile::QAbstractStatus
    {
            Q_OBJECT

            Q_ENUMS(UploadStatus)
            Q_PROPERTY(QVariant fileModel READ fileModel WRITE setFileModel)

        public:
            enum UploadStatus {
                FileNotExists = QProfile::QAbstractStatus::StatusButtom,
                CannotGetRelatedFiles,
                TouchMlocateDataFileFailed,
                UpatedbFailed,
                UploadResultFailed,
                UploadXmlError,
                DeleteResultFailed,
                AllocMemoryFailed,
                InvalidEncryptedFile,
                UploadProfileFailedOtherACK,
                UploadProfInfoWrongNumber,
                UploadProfInfoOtherFailedACK,
            };

        public:
            QUploadStatus(QObject *parent = nullptr);
            QUploadStatus(const QVariant &statusVar);
            QUploadStatus(QObject *parent, const QString &id,
                          const quint32 commandStatus = QProfile::QAbstractStatus::Failed);
            ~QUploadStatus();

            const QVariant fileModel();
            void setFileModel(const QVariant &fileModel);
            virtual bool canRecover();

        private:
            QVariant		m_fileModelVar;
    };
}

Q_DECLARE_METATYPE(QRserver::QUploadStatus)

#endif // QUPLOADSTATUS_H
