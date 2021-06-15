/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QOTDRRESULTFILTER_H
#define QOTDRRESULTFILTER_H

#include "qabstractresultfilter.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QRserver
{
    class QOtdrResultFilter: public QAbstractResultFilter
    {
            Q_OBJECT

        public:
            QOtdrResultFilter(QObject *parent = nullptr);
            ~QOtdrResultFilter();
            virtual bool isKey(const QString absolutePath);
            virtual const QString filterPattern();
            virtual const QStringList getRelatedFileFolderList(const QString
                    absoluteKeyPath);
            virtual const QStringList getFileListNeedToBeDeleted(const QString
                    absoluteKeyPath);
            virtual const QString getDisplayName(const QString &absoluteFilePath);
            virtual const QString keyFilePattern();

        private:
            static const QString otdrKeyFile;
    };
}


#endif /* QOTDRRESULTFILTER_H */
