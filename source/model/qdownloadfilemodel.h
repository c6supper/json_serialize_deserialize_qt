/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QDOWNLOADFILEMODEL_H
#define QDOWNLOADFILEMODEL_H

#include "qrserverconfig.h"
#include "quploadfilemodel.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QRserver
{
    class QDownloadFileModel: public QUploadFileModel
    {
            Q_OBJECT

        public:
            QDownloadFileModel(QObject *parent = nullptr);
            ~QDownloadFileModel();

            static const QString CommandDownloadAll;
            static const QString CommandGetChangeList;
            static const QString CommandLockDown;

            static const QString FileStatus;
            static const QString FileStatusCurrent;
            static const QString FileStatusNew;
            static const quint32 DownloadSingleFileTimeout;

#ifdef BROADCAST_PROFILE_DOWNLOAD
            static const QString DownloadedProfileMark;
#endif
    };
}

Q_DECLARE_METATYPE(QRserver::QDownloadFileModel)

#endif
