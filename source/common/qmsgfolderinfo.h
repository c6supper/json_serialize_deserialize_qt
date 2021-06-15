/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QMSGFOLDERINFO_H
#define QMSGFOLDERINFO_H

#include <QMetaType>
#include "qabstractbinmsg.h"
#include "qprofile_debug.h"
#include "qprofile_global.h"
#include "qstringattribute.h"

namespace QRserver
{
    class QMsgFolderInfo : public QAbstractBinMsg
    {
            Q_OBJECT

            Q_PROPERTY(QString folderName READ folderName WRITE setFolderName)
            Q_PROPERTY(qint32 folderType READ folderType WRITE setFolderType)
            Q_PROPERTY(qint32 fileCount READ fileCount WRITE setFileCount)

        public:
            QMsgFolderInfo(QObject *parent = nullptr);
            QMsgFolderInfo(const QMsgFolderInfo &message);
            virtual ~QMsgFolderInfo();

            const QString folderName();
            void setFolderName(QString folderName);

            quint32 folderType() const;
            void setFolderType(quint32 folderType);

            quint32 fileCount() const;
            void setFileCount(quint32 fileCount);

        protected:
            virtual QDataStream &read(QDataStream &in) ;
            virtual QDataStream &write(QDataStream &out) const;
            virtual const QByteArray toByteArray() const;
            virtual void initAttributes();

            void getFolderName(char *string, int length) const;

        private:
            QProfile::QStringAttribute 		*m_folderName;
            qint32 							m_folderType;
            qint32 							m_fileCount;
    };
}

Q_DECLARE_METATYPE(QRserver::QMsgFolderInfo)

#endif
