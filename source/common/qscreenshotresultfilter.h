
#ifndef QSCREENSHOTRESULTFILTER_H
#define QSCREENSHOTRESULTFILTER_H

#include "qabstractresultfilter.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QRserver
{
    class QScreenshotResultFilter: public QAbstractResultFilter
    {
            Q_OBJECT

        public:
            QScreenshotResultFilter(QObject *parent = nullptr);
            ~QScreenshotResultFilter();

            static const QString HtmlImageFolderKey;

            virtual bool isKey(const QString absolutePath);
            virtual const QString filterPattern();
            virtual const QStringList getRelatedFileFolderList(const QString
                    absoluteKeyPath);
            virtual const bool postUploading(const QString &absoluteFilePath,
                                             const quint32 commandStatus);
            virtual const QString getDisplayName(const QString &absoluteFilePath);
            virtual const QString keyFilePattern();

        private:
            void patchHtmlFile(const QString &absoluteHtmlPath,
                               const QString &absoluteKeyPath);
    };
}

#endif /* QSCREENSHOTRESULTFILTER_H */
