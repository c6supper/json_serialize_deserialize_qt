/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QABSTRACTRESULTFILTER_H
#define QABSTRACTRESULTFILTER_H

#include <QVariant>
#include <QStringList>
#include <QDateTime>
#include <QDomElement>
#include "qprofile_global.h"
#include "qrservermodel.h"
#include "qabstractjsonelement.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QRserver
{
    class QAbstractResultFilter: public QProfile::QAbstractJsonElement
    {
            Q_OBJECT

            Q_PROPERTY(QString measureType READ measureType WRITE setMeasureType)
            Q_PROPERTY(QString measureMode READ measureMode WRITE setMeasureMode)

        public:
            QAbstractResultFilter(const QString &measureMode, const QString &measureType,
                                  QObject *parent = nullptr);
            ~QAbstractResultFilter();
            virtual void pureVirtualMock() {};

            const QString measureType() const;
            void setMeasureType(QString measureType);

            const QString measureMode() const;
            void setMeasureMode(QString measureMode);

            bool matched(const QString &measureMode, const QString &measureType);

            virtual bool isKey(const QString absolutePath) = 0;
            virtual const QString filterPattern() = 0;
            virtual const QStringList getRelatedFileFolderList(const QString
                    absoluteKeyPath) = 0;
            virtual const QStringList getFileListNeedToBeDeleted(const QString
                    absoluteKeyPath);
            virtual const QString getDisplayName(const QString &absoluteFilePath);
            virtual const bool postUploading(const QString &absoluteFilePath,
                                             const quint32 commandStatus);
            virtual const QString keyFilePattern() = 0;
            virtual QString getLocationFromXml(const QString &absoluteXmlPath);
            virtual QString getJobIdFromXml(const QString &absoluteXmlPath);

        protected:
            void patchRelatedFiles(const QString &absoluteXmlPath,
                                   const QDateTime &saveDateTime);
            virtual bool isNeedPatchAdvInfoFromHtmlToXml();
            virtual QDomElement patchAdvInfoFromHtmlToXml(QDomDocument &doc,
                    const QString &absoluteXmlPath);

        private:
            void createXml(const QString &absoluteXmlPath, const QDateTime &saveDateTime);
            void patchXmlFile(const QString &absoluteXmlPath,
                              const QDateTime &saveDateTime);
            void patchSignature(const QString &absoluteXmlPath);
            bool containsSignature(const QString &absoluteHtmlPath);
            QString getJobIdFromHtml(const QString &absoluteHtmlPath);
            bool addSignatureToHtml(const QString &jobId, const QString &absoluteHtmlPath);

        private:
            QDomElement createOemInfo(QDomDocument &doc, const QDateTime &saveDateTime);
            bool isValidOem(QDomElement &oem);

        private:
            QString					m_measureMode, m_measureType;
            const QRserverModel		*m_rserverModel;

    };
}

#endif /* QABSTRACTRESULTFILTER_H */
