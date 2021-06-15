/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QFILEMODEL_H
#define QFILEMODEL_H

#include "qprofile_debug.h"
#include "qprofile_global.h"
#include "qstringattribute.h"
#include "qdatetimeattribute.h"
#include "qnumericattribute.h"
#include "qabstractmodel.h"
#include <QDateTime>

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QRserver
{
    class QFileModel: public QProfile::QAbstractModel
    {
            Q_OBJECT
            Q_ENUMS(UploadDownloadStatus)

            Q_PROPERTY(QString fileName READ fileName WRITE setFileName)
            Q_PROPERTY(QString pathName READ pathName WRITE setPathName)
            Q_PROPERTY(quint32 fileType READ fileType WRITE setFileType)
            Q_PROPERTY(quint32 fileSize READ fileSize WRITE setFileSize)
            Q_PROPERTY(quint32 localSize READ localSize WRITE setLocalSize)
            Q_PROPERTY(QString typeName READ typeName WRITE setTypeName)
            Q_PROPERTY(QDateTime dateTime READ dateTime WRITE setDateTime)
            Q_PROPERTY(QDateTime remoteDateTime READ remoteDateTime WRITE setRemoteDateTime)
            Q_PROPERTY(QString measureType READ measureType WRITE setMeasureType)
            Q_PROPERTY(QString measureMode READ measureMode WRITE setMeasureMode)
            Q_PROPERTY(QString displayName READ displayName WRITE setDisplayName)
            Q_PROPERTY(QString jobId READ jobId WRITE setJobId)
            Q_PROPERTY(QString location READ location WRITE setLocation)

        public:
            static const QString PropertyFileName;
            static const QString PropertyPathName;
            static const QString PropertyFileType;
            static const QString PropertyFileSize;
            static const QString PropertyLocalSize;
            static const QString PropertyTypeName;
            static const QString PropertyDateTime;
            static const QString PropertyRemoteDateTime;
            static const QString PropertyMeasureType;
            static const QString PropertyMeasureMode;
            static const QString PropertyDisplayName;
            static const QString PropertyJobId;
            static const QString PropertyLocation;
            static const qint32 FileDateTimeRange;

        public:
            QFileModel(QObject *parent = nullptr);
            ~QFileModel() {}

            QString pathName() const;
            void setPathName(QString pathName);

            QString fileName() const;
            void setFileName(QString fileName);

            QString typeName() const;
            void setTypeName(QString typeName);

            QDateTime dateTime() const;
            void setDateTime(QDateTime dateTime);

            QDateTime remoteDateTime() const;
            void setRemoteDateTime(QDateTime dateTime);

            quint32 fileType() const;
            void setFileType(quint32 fileType);

            quint32 fileSize() const;
            void setFileSize(quint32 fileSize);

            quint32 localSize() const;
            void setLocalSize(quint32 localSize);

            QString measureType() const;
            void setMeasureType(QString measureType);

            QString measureMode() const;
            void setMeasureMode(QString measureMode);

            QString displayName() const;
            void setDisplayName(QString displayName);

            QString jobId() const;
            void setJobId(QString jobId);

            QString location() const;
            void setLocation(QString location);

            bool isStatusNew();

        public slots:
            virtual void initAttributes();

        private:
            QSharedPointer<QProfile::QStringAttribute> 		m_fileName;
            QSharedPointer<QProfile::QStringAttribute> 		m_pathName;
            QSharedPointer<QProfile::QStringAttribute> 		m_typeName;
            QSharedPointer<QProfile::QDateTimeAttribute> 	m_dateTime;
            QSharedPointer<QProfile::QDateTimeAttribute> 	m_remoteDateTime;
            QSharedPointer<QProfile::QNumericAttribute> 	m_fileType;
            QSharedPointer<QProfile::QNumericAttribute> 	m_fileSize;
            QSharedPointer<QProfile::QNumericAttribute> 	m_localSize;
            QSharedPointer<QProfile::QStringAttribute>		m_measureType;
            QSharedPointer<QProfile::QStringAttribute>		m_measureMode;
            QSharedPointer<QProfile::QStringAttribute>		m_displayName;
            QSharedPointer<QProfile::QStringAttribute>		m_jobId;
            QSharedPointer<QProfile::QStringAttribute>		m_location;
    };
}

Q_DECLARE_METATYPE(QRserver::QFileModel)

#endif
