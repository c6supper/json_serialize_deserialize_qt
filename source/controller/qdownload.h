/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QDOWNLOAD_H
#define QDOWNLOAD_H

#include "qrserverconfig.h"
#include "qrservercontainer.h"
#include "qdownloadfilemodel.h"
#include "qdownloadstatus.h"
#ifdef BUILD_X86
#include <QFile>
#endif

using namespace QProfile;

namespace QRserver
{
    class QDownload: public QRserverContainer
    {
            Q_OBJECT

        public:
            QDownload(QObject *parent = nullptr);

        protected slots:
            virtual bool initPropertiesMap(const QVariantMap &properties = QVariantMap());
            virtual	void allocModel();

        protected:
            virtual void processMessage(QRserver::QAbstractBinMsg *message);
            virtual bool processCommands(const QString &commandId,
                                         const QString &commandType, QVariant commandVar,
                                         QVariant *status = nullptr);

        private:
            bool getChangeList();
            const QVariant download(QVariant argsVar);
            const QAbstractStatus::CommandStatus downloadSingleFile(
                const QFileModel *fileModel);
            void appendProfileInfo(QAbstractBinMsg *message);
            void saveProfile(QAbstractBinMsg *message);
            void modifyFileDateTime(const QString &filepath, const QDateTime &dateTime);
            bool isRsaEncrypedFile(const QString filePathName);
            bool decryptFile(const QString filePathName);
            const QVariant lockDown(QVariant argsVar);
            bool lockDownProfiles();
            bool unlockDownProfiles();
            bool deleteLastLockdownProf();
#ifdef BUILD_X86
            void setPermissions(const QString &fileName, QFile::Permissions permissions);
#endif
        private:
            QDownloadFileModel *m_downloadFileModel;
    };
}

Q_DECLARE_METATYPE(QRserver::QDownload)

#endif
