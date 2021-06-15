/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QSDHRESULTFILTER_H
#define QSDHRESULTFILTER_H

#include "qetherresultfilter.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QRserver
{
    class QSdhResultFilter: public QEtherResultFilter
    {
            Q_OBJECT

        public:
            QSdhResultFilter(QObject *parent = nullptr);
            ~QSdhResultFilter();

        protected:
            virtual bool isNeedPatchAdvInfoFromHtmlToXml();
            virtual QDomElement patchAdvInfoFromHtmlToXml(QDomDocument &doc,
                    const QString &absoluteXmlPath);
            virtual const QString xmlFileName(const QFileInfo keyInfo);
    };
}


#endif /* QSDHRESULTFILTER_H */
