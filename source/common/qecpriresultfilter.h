/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QECPRIRESULTFILTER_H
#define QECPRIRESULTFILTER_H

#include "qetherresultfilter.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QRserver
{
    class QEcpriResultFilter: public QEtherResultFilter
    {
            Q_OBJECT

        public:
            QEcpriResultFilter(QObject *parent = nullptr);
            ~QEcpriResultFilter();
    };
}


#endif /* QECPRIRESULTFILTER_H */
