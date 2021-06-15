/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */


#include "quploadresultmodel.h"
#include "qprofile_debug.h"
#include "qobjectfactory.h"

using namespace QProfile;
using namespace QRserver;

QObjectSelfRegisteration<QUploadResultModel> registerQUploadResultModel;

QUploadResultModel::QUploadResultModel(QObject *parent) : QUploadFileModel(
        parent)
{
    Q_ASSERT(parent != nullptr);
}

QUploadResultModel::~QUploadResultModel()
{
}
