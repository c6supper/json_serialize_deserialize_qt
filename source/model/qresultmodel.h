
#ifndef QRESULTMODEL_H
#define QRESULTMODEL_H

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
    class QResultModel: public QProfile::QAbstractModel
    {
            Q_OBJECT

            Q_PROPERTY(QString displayName READ displayName WRITE setDisplayName)
            Q_PROPERTY(quint32 fileType READ fileType WRITE setFileType)
            Q_PROPERTY(QString typeName READ typeName WRITE setTypeName)
            Q_PROPERTY(QDateTime dateTime READ dateTime WRITE setDateTime)
            Q_PROPERTY(QStringList relatedFileList READ relatedFileList WRITE
                       setRelatedFileList)
#if 0
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
            static const qint32 FileDateTimeRange;
#endif
        public:
            QResultModel(QObject *parent = nullptr);
            ~QResultModel() {}

            QString displayName() const;
            void setDisplayName(QString displayName);

            quint32 fileType() const;
            void setFileType(quint32 fileType);

            QString typeName() const;
            void setTypeName(QString typeName);

            QDateTime dateTime() const;
            void setDateTime(QDateTime dateTime);

            QStringList relatedFileList() const;
            void setRelatedFileList(QStringList relatedFileList);

        public slots:
            virtual void initAttributes();

        private:
            QSharedPointer<QProfile::QStringAttribute> m_displayName;
            QSharedPointer<QProfile::QNumericAttribute> m_fileType;
            QSharedPointer<QProfile::QStringAttribute> m_typeName;
            QSharedPointer<QProfile::QDateTimeAttribute> 	m_dateTime;
            QStringList m_relatedFileList;
    };
}

Q_DECLARE_METATYPE(QRserver::QResultModel)

#endif
