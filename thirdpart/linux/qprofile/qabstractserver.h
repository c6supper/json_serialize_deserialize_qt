/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QABSTRACTSERVER_H
#define QABSTRACTSERVER_H

#include <QObject>
#include "qprofile_global.h"

namespace QProfile
{
    class QAbstractServer : public QObject
    {
            Q_OBJECT
        public:
            explicit QAbstractServer(QObject *parent = nullptr);
            virtual ~QAbstractServer() {}


    };
}
#endif // QABSTRACTSERVER_H
