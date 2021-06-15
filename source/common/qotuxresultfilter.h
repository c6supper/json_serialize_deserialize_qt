/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QOTUXRESULTFILTER_H
#define QOTUXRESULTFILTER_H

#include "qetherresultfilter.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QRserver
{
    class QOtuxResultFilter: public QEtherResultFilter
    {
            Q_OBJECT

        public:
            QOtuxResultFilter(QObject *parent = nullptr);
            ~QOtuxResultFilter();
    };
}


#endif /* QOTUXRESULTFILTER_H */
