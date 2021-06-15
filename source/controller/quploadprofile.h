/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QUPLOADPROFILE_H
#define QUPLOADPROFILE_H

#include "quploadfile.h"
#include "quploadprofilemodel.h"
#include "quploadstatus.h"

using namespace QProfile;

namespace QRserver
{
    class QUploadProfile: public QUploadFile
    {
            Q_OBJECT

        public:
            QUploadProfile(QObject *parent = nullptr);

        protected slots:
            virtual bool initPropertiesMap(const QVariantMap &properties = QVariantMap());
            virtual	void allocModel();

        protected:
            virtual const QVariant upload(const QVariant argsVar,
                                          const bool bDelete = false);
            virtual const QVariant uploadDelete(const QVariant argsVar);
            virtual const QVariant absolutePathToFileType(const QString absoluteFilePath);
            virtual const QString getFilterPattern();
            virtual void processMessage(QRserver::QAbstractBinMsg *message);
            virtual bool processCommands(const QString &commandId,
                                         const QString &commandType, QVariant commandVar,
                                         QVariant *status = nullptr);

        private:
            const QAbstractStatus::CommandStatus uploadSingleFile(const QFileModel
                    *fileModel);
            void modifyFileDateTime(const QString &filepath);
            QVariantList getProfileInfoList();
            const QVariant uploadProfileInfoList();
            const QAbstractStatus::CommandStatus sendProfileInfoMsg();
            void updateProfInfoAckStatus(QAbstractBinMsg *message);

        private:
            QUploadProfileModel		*m_uploadProfileModel;
            QUploadStatus 			m_uploadStatus;
    };
}

Q_DECLARE_METATYPE(QRserver::QUploadProfile)

#endif
