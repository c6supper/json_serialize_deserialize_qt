/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QMSGCHECKTARGET_H
#define QMSGCHECKTARGET_H

#include "qmsgregistertarget.h"

namespace QRserver
{
    class QMsgCheckTarget : public QMsgRegisterTarget
    {
            Q_OBJECT

        public:
            QMsgCheckTarget(QObject *parent = nullptr);
            QMsgCheckTarget(const QMsgCheckTarget &message);
            virtual ~QMsgCheckTarget();
    };
}

Q_DECLARE_METATYPE(QRserver::QMsgCheckTarget)

#endif
