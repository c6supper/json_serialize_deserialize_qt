/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QDEVICERELATEDFILETYPE_H
#define QDEVICERELATEDFILETYPE_H

#include "qrserverfiletype.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QRserver
{
    class QDeviceRelatedFileType: public QRserverFileType
    {
            Q_OBJECT

            Q_PROPERTY(QString pathName READ localPath WRITE setLocalPath)

        public:
            QDeviceRelatedFileType(QObject *parent = nullptr);
            ~QDeviceRelatedFileType();

            QString localPath() const;
            void setLocalPath(QString localPath);

        private:
            QString					m_localPath;
    };
}

#endif
