/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QMSGUNREGISTTARGET_H
#define QMSGUNREGISTTARGET_H

#include "qmsgregistertarget.h"

namespace QRserver
{
    class QMsgUnregistTarget : public QMsgRegisterTarget
    {
            Q_OBJECT

        public:
            QMsgUnregistTarget(QObject *parent = nullptr);
            QMsgUnregistTarget(const QMsgUnregistTarget &message);
            virtual ~QMsgUnregistTarget() {}
    };
}

Q_DECLARE_METATYPE(QRserver::QMsgUnregistTarget)

#endif
