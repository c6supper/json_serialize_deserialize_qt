/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QETHERRESULTFILTER_H
#define QETHERRESULTFILTER_H

#include "qabstractresultfilter.h"
#include <QFileInfo>

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QRserver
{
    class QEtherResultFilter: public QAbstractResultFilter
    {
            Q_OBJECT

        public:
            QEtherResultFilter(QObject *parent = nullptr);
            ~QEtherResultFilter();
            virtual bool isKey(const QString absolutePath);
            virtual const QString filterPattern();
            virtual const QStringList getRelatedFileFolderList(const QString
                    absoluteKeyPath);
            virtual const QString keyFilePattern();
        protected:
            virtual const QString xmlFileName(const QFileInfo keyInfo);
    };
}


#endif /* QETHERRESULTFILTER_H */
