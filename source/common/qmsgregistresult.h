/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QMSGREGISTERRESULT_H
#define QMSGREGISTERRESULT_H

#include <QMetaType>
#include "qabstractbinmsg.h"

namespace QRserver
{
    class QMsgRegisterResult : public QAbstractBinMsg
    {
            Q_OBJECT
            Q_ENUMS(RegisteredStatus)

            Q_PROPERTY(quint8 status READ status WRITE setStatus)

        public:
            enum RegisteredStatus { Registered = 0};
        public:
            QMsgRegisterResult(QObject *parent = nullptr);
            QMsgRegisterResult(const QMsgRegisterResult &message);
            virtual ~QMsgRegisterResult();

            const qint8 status() const
            {
                return m_status;
            }
            void setStatus(const qint8 status)
            {
                m_status = status;
            }


        protected:
            virtual QDataStream &read(QDataStream &in) ;
            virtual QDataStream &write(QDataStream &out) const;
            virtual const QByteArray toByteArray() const;

        protected:
            qint8		m_status;
    };
}

Q_DECLARE_METATYPE(QRserver::QMsgRegisterResult)

#endif
