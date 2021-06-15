/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */


#include "quploadprofilemodel.h"
#include "qprofile_debug.h"
#include "qobjectfactory.h"

using namespace QProfile;
using namespace QRserver;

QObjectSelfRegisteration<QUploadProfileModel> registerQUploadProfileModel;

const QString QUploadProfileModel::CommandUploadProfileInfoList =
    "UploadProfileInfoList";
const quint32 QUploadProfileModel::MaxCapPerProfInfoMsg = 100;

QUploadProfileModel::QUploadProfileModel(QObject *parent) : QUploadFileModel(
        parent)
{
    Q_ASSERT(parent != nullptr);
}

QUploadProfileModel::~QUploadProfileModel()
{
}
