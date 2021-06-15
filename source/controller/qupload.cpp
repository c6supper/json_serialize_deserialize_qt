/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qupload.h"
#include "qobjectfactory.h"
#include "quploadprofile.h"
#include "quploadresult.h"

using namespace QProfile;
using namespace QRserver;

QObjectSelfRegisteration<QUpload> registerQUpload;

QUpload::QUpload(QObject *parent): QRserverContainer(parent, false)
{
    m_uploadList.append(new QUploadProfile(this));
    m_uploadList.append(new QUploadResult(this));
}


bool QUpload::initPropertiesMap(const QVariantMap &properties)
{
    if (properties.count() <= 0) {
        setProperty("tagName", "qupload");
    }

    return QRserverContainer::initPropertiesMap(properties);
}

void QUpload::allocModel()
{
    //todo
}

