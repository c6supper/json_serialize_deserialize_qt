/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qprofile_debug.h"
#include "qdevicerelatedfiletype.h"

using namespace QRserver;

QDeviceRelatedFileType::QDeviceRelatedFileType(QObject *parent)
    : QRserverFileType(parent)
{
}

QDeviceRelatedFileType::~QDeviceRelatedFileType()
{
}

QString QDeviceRelatedFileType::localPath() const
{
    return m_localPath;
}
void QDeviceRelatedFileType::setLocalPath(QString localPath)
{
    m_localPath = localPath;
}

