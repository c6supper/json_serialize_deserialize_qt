/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QMSGGETDEVICEINFOEXT_H
#define QMSGGETDEVICEINFOEXT_H

#include "qmsgregistertargetext.h"

namespace QRserver
{
    class QMsgGetDeviceInfoExt : public QMsgRegisterTargetExt
    {
            Q_OBJECT

        public:
            QMsgGetDeviceInfoExt(QObject *parent = nullptr);
            QMsgGetDeviceInfoExt(const QMsgGetDeviceInfoExt &message);
            virtual ~QMsgGetDeviceInfoExt();
    };
}

Q_DECLARE_METATYPE(QRserver::QMsgGetDeviceInfoExt)

#endif
