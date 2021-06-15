/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QFIBERRESULTFILTER_H
#define QFIBERRESULTFILTER_H

#include "qetherresultfilter.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QRserver
{
    class QFiberResultFilter: public QEtherResultFilter
    {
            Q_OBJECT

        public:
            QFiberResultFilter(QObject *parent = nullptr);
            ~QFiberResultFilter();
    };
}


#endif /* QFIBERRESULTFILTER_H */
