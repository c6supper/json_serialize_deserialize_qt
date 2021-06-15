/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QCPRIRESULTFILTER_H
#define QCPRIRESULTFILTER_H

#include "qetherresultfilter.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QRserver
{
    class QCpriResultFilter: public QEtherResultFilter
    {
            Q_OBJECT

        public:
            QCpriResultFilter(QObject *parent = nullptr);
            ~QCpriResultFilter();
        protected:
            virtual const QString xmlFileName(const QFileInfo keyInfo);
    };
}


#endif /* QCPRIRESULTFILTER_H */
