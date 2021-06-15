/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QODU0RESULTFILTER_H
#define QODU0RESULTFILTER_H

#include "qetherresultfilter.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QRserver
{
    class QOdu0ResultFilter: public QEtherResultFilter
    {
            Q_OBJECT

        public:
            QOdu0ResultFilter(QObject *parent = nullptr);
            ~QOdu0ResultFilter();
    };
}


#endif /* QODU0RESULTFILTER_H */
