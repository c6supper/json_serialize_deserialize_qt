/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QOSARESULTFILTER_H
#define QOSARESULTFILTER_H

#include "qetherresultfilter.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QRserver
{
    class QOSAResultFilter: public QEtherResultFilter
    {
            Q_OBJECT

        public:
            QOSAResultFilter(QObject *parent = nullptr);
            ~QOSAResultFilter();
    };
}


#endif /* QOSARESULTFILTER_H */
