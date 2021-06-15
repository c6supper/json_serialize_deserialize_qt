/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QMSGPROFILEBLK_H
#define QMSGPROFILEBLK_H

#include "qmsguploadprofileblock.h"

namespace QRserver
{
    class QMsgProfileBlk : public QMsgUploadProfileBlock
    {
            Q_OBJECT

        public:
            QMsgProfileBlk(QObject *parent = nullptr);
            QMsgProfileBlk(const QMsgProfileBlk &message);
            virtual ~QMsgProfileBlk();
    };
}

Q_DECLARE_METATYPE(QRserver::QMsgProfileBlk)

#endif
