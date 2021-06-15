/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QMSGREGISTERTARGETEXTLONGID_H
#define QMSGREGISTERTARGETEXTLONGID_H

#include "qmsgregistertargetext.h"

namespace QRserver
{
    class QMsgRegisterTargetExtLongId : public QMsgRegisterTargetExt
    {
            Q_OBJECT

        public:
            QMsgRegisterTargetExtLongId(QObject *parent = nullptr);
            QMsgRegisterTargetExtLongId(const QMsgRegisterTargetExtLongId &message);
            virtual ~QMsgRegisterTargetExtLongId();

        protected:
            virtual QDataStream &read(QDataStream &in) ;
            virtual QDataStream &write(QDataStream &out) const;
            virtual const QByteArray toByteArray() const;
    };
}

Q_DECLARE_METATYPE(QRserver::QMsgRegisterTargetExtLongId)

#endif
