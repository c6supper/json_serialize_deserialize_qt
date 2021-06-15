/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QUPLOADRESULT_H
#define QUPLOADRESULT_H

#include "quploadfile.h"
#include "quploadresultmodel.h"
#include "quploadstatus.h"
#include "qvconnectresultmodel.h"

using namespace QProfile;

namespace QRserver
{
    class QUploadResult: public QUploadFile
    {
            Q_OBJECT

        public:
            QUploadResult(QObject *parent = nullptr);

        protected slots:
            virtual bool initPropertiesMap(const QVariantMap &properties = QVariantMap());
            virtual	void allocModel();

        protected:
            virtual const QVariant upload(const QVariant argsVar,
                                          const bool bDelete = false);
            virtual const QVariant uploadDelete(const QVariant argsVar);
            virtual const QVariant absolutePathToFileType(const QString absoluteFilePath);
            virtual const QString getFilterPattern();
            virtual bool processCommands(const QString &commandId,
                                         const QString &commandType, QVariant commandVar,
                                         QVariant *status = nullptr);
            virtual void processMessage(QRserver::QAbstractBinMsg *message);

        private:
            const QAbstractStatus::CommandStatus uploadSingleFile(const QString resultName,
                    const QString absoluteFilePath, const bool isLastFile, const quint32 fileType);
            const QAbstractStatus::CommandStatus uploadResultInfo(const QString resultName,
                    const quint32 fileType, const quint32 count);
            bool isXmlFile(const QString &resultName, const QString &absoluteFilePath);
            const QVariant getResultList();
            void cpUploadFileModel2VConnectResultModel();
            bool deleteResultFiles(const QStringList &fileList);

        private:
            QUploadResultModel *m_uploadResultModel;
            QVConnectResultModel *m_vconnectResultModel;
            QUploadStatus m_uploadStatus;
    };
}

Q_DECLARE_METATYPE(QRserver::QUploadResult)

#endif
