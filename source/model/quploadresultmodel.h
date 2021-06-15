/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QUPLOADRESULTMODEL_H
#define QUPLOADRESULTMODEL_H

#include "quploadfilemodel.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QRserver
{
    class QUploadResultModel: public QUploadFileModel
    {
            Q_OBJECT

        public:
            QUploadResultModel(QObject *parent = nullptr);
            ~QUploadResultModel();
    };
}

Q_DECLARE_METATYPE(QRserver::QUploadResultModel)

#endif
