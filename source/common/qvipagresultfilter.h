/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QVIPAGRESULTFILTER_H
#define QVIPAGRESULTFILTER_H

#include "qetherresultfilter.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QRserver
{
    class QVipagResultFilter: public QEtherResultFilter
    {
            Q_OBJECT

        public:
            QVipagResultFilter(QObject *parent = nullptr);
            ~QVipagResultFilter();
    };
}


#endif /* QVIPAGRESULTFILTER_H */
