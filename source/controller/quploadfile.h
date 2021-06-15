/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QUPLOADFILE_H
#define QUPLOADFILE_H

#include "qrservercontainer.h"
#include "qabstractbinmsg.h"
#include "quploadfilemodel.h"

using namespace QProfile;

namespace QRserver
{
    class QUploadFile: public QRserverContainer
    {
            Q_OBJECT

        public:
            QUploadFile(QObject *parent = nullptr);

        protected slots:
            virtual	void allocModel();

        protected:
            virtual void processMessage(QRserver::QAbstractBinMsg *message);
            virtual bool processCommands(const QString &commandId,
                                         const QString &commandType, QVariant commandVar,
                                         QVariant *status = nullptr);
            virtual const QVariant upload(const QVariant argsVar,
                                          const bool bDelete = false) = 0;
            virtual const QVariant uploadDelete(const QVariant argsVar) = 0;
            virtual const QVariant absolutePathToFileType(const QString absoluteFilePath) =
                0;
            virtual const QString getFilterPattern() = 0;

        protected:
            const QVariant getFileList();

        private:
            QUploadFileModel			*m_uploadFileModel;
            static const QString		m_mlocateDataFile;

    };
}

#endif
