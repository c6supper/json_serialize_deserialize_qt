/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QMSGUPLOADPROFILEFINISH_H
#define QMSGUPLOADPROFILEFINISH_H

#include "qmsguploadprofileblock.h"

namespace QRserver
{
    class QMsgUploadProfileFinish : public QMsgUploadProfileBlock
    {
            Q_OBJECT

        public:
            QMsgUploadProfileFinish(QObject *parent = nullptr);
            QMsgUploadProfileFinish(const QMsgUploadProfileFinish &message);
            virtual ~QMsgUploadProfileFinish();

            virtual quint32 blockLength() const;
    };
}

Q_DECLARE_METATYPE(QRserver::QMsgUploadProfileFinish)

#endif
