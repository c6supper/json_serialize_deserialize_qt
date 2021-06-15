/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QMSGFILEFINISH_H
#define QMSGFILEFINISH_H

#include "qmsgfileblock.h"

namespace QRserver
{
    class QMsgFileFinish : public QMsgFileBlock
    {
            Q_OBJECT

        public:
            QMsgFileFinish(QObject *parent = nullptr);
            QMsgFileFinish(const QMsgFileFinish &message);
            virtual ~QMsgFileFinish();
    };
}

Q_DECLARE_METATYPE(QRserver::QMsgFileFinish)

#endif
