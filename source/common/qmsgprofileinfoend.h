/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QMSGPROFILEINFOEND_H
#define QMSGPROFILEINFOEND_H

#include <QMetaType>
#include <QSharedPointer>
#include <QDateTime>
#include "qmsgprofileinfo.h"

namespace QRserver
{
    class QMsgProfileInfoEnd : public QMsgProfileInfo
    {
            Q_OBJECT

        public:
            QMsgProfileInfoEnd(QObject *parent = nullptr);
            QMsgProfileInfoEnd(const QMsgProfileInfoEnd &message);
            virtual ~QMsgProfileInfoEnd();
    };
}

Q_DECLARE_METATYPE(QRserver::QMsgProfileInfoEnd)

#endif
