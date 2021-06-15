/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QMSGVERIFYRESULT_H
#define QMSGVERIFYRESULT_H

#include <QMetaType>
#include "qmsgregistresult.h"

namespace QRserver
{
    class QMsgVerifyResult : public QMsgRegisterResult
    {
            Q_OBJECT

        public:
            QMsgVerifyResult(QObject *parent = nullptr);
            QMsgVerifyResult(const QMsgVerifyResult &message);
            virtual ~QMsgVerifyResult();
    };
}

Q_DECLARE_METATYPE(QRserver::QMsgVerifyResult)

#endif
