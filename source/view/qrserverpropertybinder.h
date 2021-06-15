/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QRSERVERPROPERTYBINDER_H
#define QRSERVERPROPERTYBINDER_H

#include "qprofile_global.h"
#include "qabstractpropertybinder.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QRserver
{
    class QRserverPropertyBinder: public QProfile::QAbstractPropertyBinder
    {
            Q_OBJECT

        public:
            QRserverPropertyBinder(QObject *view, QObject *model);
            ~QRserverPropertyBinder();

    };
}


#endif /* QRSERVERPROPERTYBINDER_H */
