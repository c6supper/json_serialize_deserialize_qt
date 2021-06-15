/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QRSERVERFILETYPE_H
#define QRSERVERFILETYPE_H

#include <QRegExp>
#include <QList>
#include <QVariant>
#include <QMutexLocker>
#include "qprofile_debug.h"
#include "qprofile_global.h"
#include "qabstractjsonelement.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QRserver
{
    class QRserverFileType: public QProfile::QAbstractJsonElement
    {
            Q_OBJECT

            Q_PROPERTY(QString fileType READ fileType WRITE setFileType)
            Q_PROPERTY(QVariantList regExpList READ regExpList WRITE setRegexpList)
            Q_PROPERTY(QString typeName READ typeName WRITE setType2Name)
            Q_PROPERTY(QString measureType READ measureType WRITE setMeasureType)
            Q_PROPERTY(QString measureMode READ measureMode WRITE setMeasureMode)
            Q_PROPERTY(QString displayName READ displayName WRITE setDisplayName)

        public:
            QRserverFileType(QObject *parent = nullptr);
            ~QRserverFileType();
            virtual void pureVirtualMock() {};

            QString fileType() const;
            void setFileType(QString fileType);

            QVariantList regExpList();
            void setRegexpList(QVariantList regExpList);

            QString typeName() const;
            void setType2Name(QString typeName);

            QString displayName() const;
            void setDisplayName(QString displayName);

            QString measureType() const;
            void setMeasureType(QString measureType);

            QString measureMode() const;
            void setMeasureMode(QString measureMode);

            bool matchedFileType(const QString &pathFile);

        private:
            QString					m_fileType;
            QString					m_typeName;
            QString					m_measureType;
            QString					m_measureMode;
            QString					m_displayName;
            QList<QRegExp>			m_regExpList;
            QMutex					m_mutex;
            static const QString	propertyRegExp;

        public:
            static const QString 	PropertyFileType;
            static const QString	PropertyDisplayName;
    };
}

#endif
