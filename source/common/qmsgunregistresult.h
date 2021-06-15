/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QMSGUNREGISTRESULT_H
#define QMSGUNREGISTRESULT_H

#include <QMetaType>
#include "qmsgregistresult.h"

namespace QRserver
{
    class QMsgUnregistResult : public QMsgRegisterResult
    {
            Q_OBJECT

            Q_ENUMS(UnregisteredStatus)

        public:
            enum UnregisteredStatus { notRegistered = 0};
        public:
            QMsgUnregistResult(QObject *parent = nullptr);
            QMsgUnregistResult(const QMsgUnregistResult &message);
            virtual ~QMsgUnregistResult();
    };
}

Q_DECLARE_METATYPE(QRserver::QMsgUnregistResult)

#endif
