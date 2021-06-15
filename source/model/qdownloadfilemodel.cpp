/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */


#include "qdownloadfilemodel.h"
#include "qprofile_debug.h"
#include "qobjectfactory.h"

using namespace QProfile;
using namespace QRserver;

QObjectSelfRegisteration<QDownloadFileModel> registerQDownloadFileModel;

const QString QDownloadFileModel::CommandDownloadAll = "downloadAll";
const QString QDownloadFileModel::CommandGetChangeList = "getChangeList";
const QString QDownloadFileModel::CommandLockDown = "lockDown";

const QString QDownloadFileModel::FileStatus = "status";
const QString QDownloadFileModel::FileStatusCurrent = "Current";
const QString QDownloadFileModel::FileStatusNew = "New";

const quint32 QDownloadFileModel::DownloadSingleFileTimeout = 2 * 60 * 1000;

#ifdef BROADCAST_PROFILE_DOWNLOAD
const QString QDownloadFileModel::DownloadedProfileMark =
    "downloadedprofilemark.json";
#endif

QDownloadFileModel::QDownloadFileModel(QObject *parent) : QUploadFileModel(
        parent)
{
}

QDownloadFileModel::~QDownloadFileModel()
{
}
