/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QMSGPROFILEEND_H
#define QMSGPROFILEEND_H

#include <QMetaType>
#include <QSharedPointer>
#include <QDateTime>
#include "qmsgprofileblk.h"

namespace QRserver
{
    class QMsgProfileEnd : public QMsgProfileBlk
    {
            Q_OBJECT

        public:
            QMsgProfileEnd(QObject *parent = nullptr);
            QMsgProfileEnd(const QMsgProfileEnd &message);
            virtual ~QMsgProfileEnd();

            virtual quint32 blockLength() const;
    };
}

Q_DECLARE_METATYPE(QRserver::QMsgProfileEnd)

#endif
