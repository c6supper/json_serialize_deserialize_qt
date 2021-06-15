/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QGenericBinMessage_H
#define QGenericBinMessage_H

#include <QMetaType>
#include "qabstractbinmsg.h"

namespace QRserver
{
    class QGenericBinMessage : public QAbstractBinMsg
    {
            Q_OBJECT

        public:
            QGenericBinMessage(const quint32 type = 0, QObject *parent = nullptr);
            QGenericBinMessage(const QGenericBinMessage &message);
            virtual ~QGenericBinMessage();

        protected:
            virtual QDataStream &read(QDataStream &in) ;
            virtual QDataStream &write(QDataStream &out) const;
            virtual const QByteArray toByteArray() const;
    };
}

Q_DECLARE_METATYPE(QRserver::QGenericBinMessage)

#endif
