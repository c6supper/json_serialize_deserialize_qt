/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QRSERVERFILE_H
#define QRSERVERFILE_H

#include "qprofile_global.h"
#include "qrserverfiletype.h"
#include <QObject>
#include <QVariant>

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QRserver
{
    class QRserverFile: public QObject
    {
            Q_OBJECT

            Q_PROPERTY(QVariantList fileTypeList READ fileTypeList WRITE setFileTypeList)

        public:
            QRserverFile(QObject *parent = nullptr);
            ~QRserverFile();
            QVariantList fileTypeList();
            void setFileTypeList(QVariantList &fileTypeList);

        protected:
            virtual const qint32 stringToType(const QString typeString) = 0;
            virtual const QString type2String(const qint32 type) = 0;
            virtual bool isKey(const QString &absolutePath,
                               const QString measureMode = "", const QString measureType = "") = 0;
            const QVariant getFileType(const QString &filePath,
                                       const bool enableDirPath = false);
            const QRserverFileType *getRserverFileType(const QString &filePath);
            const QVariant getFileType(const qint32 type);
            virtual const QString getDisplayName(const QString &filePath,
                                                 const QString measureMode = "", const QString measureType = "") = 0;

        private:
            QList<QRserverFileType *>							m_fileTypeList;
            QMutex												m_mutex;
    };
}


#endif /* QRSERVERFILE_H */
