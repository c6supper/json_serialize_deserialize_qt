/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QMSGUNREGISTTARGETLONGID_H
#define QMSGUNREGISTTARGETLONGID_H

#include "qmsgregistertargetextlongid.h"

namespace QRserver
{
    class QMsgUnregistTargetLongId : public QMsgRegisterTargetExtLongId
    {
            Q_OBJECT

        public:
            QMsgUnregistTargetLongId(QObject *parent = nullptr);
            QMsgUnregistTargetLongId(const QMsgUnregistTargetLongId &message);
            virtual ~QMsgUnregistTargetLongId() {}
    };
}

Q_DECLARE_METATYPE(QRserver::QMsgUnregistTargetLongId)

#endif
