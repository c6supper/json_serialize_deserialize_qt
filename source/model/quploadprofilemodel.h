/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QUPLOADPROFILEMODEL_H
#define QUPLOADPROFILEMODEL_H

#include "quploadfilemodel.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QRserver
{
    class QUploadProfileModel: public QUploadFileModel
    {
            Q_OBJECT

        public:
            static const QString CommandUploadProfileInfoList;
            static const quint32 MaxCapPerProfInfoMsg;

        public:
            QUploadProfileModel(QObject *parent = nullptr);
            ~QUploadProfileModel();
    };
}

Q_DECLARE_METATYPE(QRserver::QUploadProfileModel)

#endif
