/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QUPLOADFILEMODEL_H
#define QUPLOADFILEMODEL_H

#include <QObject>
#include <QVariant>
#include <QList>
#include <QMutexLocker>
#include "qprofile_debug.h"
#include "qprofile_global.h"
#include "qfilemodel.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QRserver
{
    class QUploadFileModel: public QProfile::QAbstractModel
    {
            Q_OBJECT

            Q_PROPERTY(QVariantList fileModelList READ fileModelList WRITE setFileModelList)

        public:
            static const QString CommandGetFileList;
            static const QString CommandUpload;
            static const QString CommandUploadDelete;
            static const quint32 UploadSingleFileTimeout;
            static const quint32 GetFileListTimeout;

        public:
            QUploadFileModel(QObject *parent = nullptr);
            ~QUploadFileModel();

            const QVariantList fileModelList() const;
            void setFileModelList(const QVariantList &fileModelList);
            //            void appendFile(const QVariant fileVariant);
            //            const QVariantList takeFileList();
            void clear();
            const QVariantList getFileModeList(quint32 fileType, const QString profileName);

        private:
            void initAttributes();

        private:
            QList<QFileModel *>					m_fileList;
            QMutex								*m_mutex;
    };
}

Q_DECLARE_METATYPE(QRserver::QUploadFileModel)

#endif
