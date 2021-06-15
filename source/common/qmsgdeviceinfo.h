/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QMSGDEVICEINFO_H
#define QMSGDEVICEINFO_H

#include <QMetaType>
#include "qmsgregistertarget.h"

namespace QRserver
{
    class QMsgDeviceInfo : public QMsgRegisterTarget
    {
            Q_OBJECT

        public:
            QMsgDeviceInfo(QObject *parent = nullptr);
            QMsgDeviceInfo(const QMsgDeviceInfo &message);
            virtual ~QMsgDeviceInfo();
    };
}

Q_DECLARE_METATYPE(QRserver::QMsgDeviceInfo)

#endif
