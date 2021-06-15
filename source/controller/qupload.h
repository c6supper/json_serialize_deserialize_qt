/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QUPLOAD_H
#define QUPLOAD_H

#include "qrservercontainer.h"
#include <QList>
#include "quploadfile.h"

namespace QRserver
{
    class QUpload: public QRserverContainer
    {
            Q_OBJECT

        public:
            QUpload(QObject *parent = nullptr);

        protected slots:
            virtual bool initPropertiesMap(const QVariantMap &properties = QVariantMap());
            virtual	void allocModel();


        private:
            QList<QUploadFile *> 			m_uploadList;
    };
}

Q_DECLARE_METATYPE(QRserver::QUpload)

#endif
