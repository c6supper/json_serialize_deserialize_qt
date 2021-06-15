/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QMSGDEVICEINFOLONGID_H
#define QMSGDEVICEINFOLONGID_H

#include <QMetaType>
#include "qmsgregistertargetextlongid.h"

namespace QRserver
{
    class QMsgDeviceInfoLongId : public QMsgRegisterTargetExtLongId
    {
            Q_OBJECT

        public:
            QMsgDeviceInfoLongId(QObject *parent = nullptr);
            QMsgDeviceInfoLongId(const QMsgDeviceInfoLongId &message);
            virtual ~QMsgDeviceInfoLongId();
    };
}

Q_DECLARE_METATYPE(QRserver::QMsgDeviceInfoLongId)

#endif
