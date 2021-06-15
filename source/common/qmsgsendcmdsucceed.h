/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QMSGSENDCMDSUCCEED_H
#define QMSGSENDCMDSUCCEED_H

#include <QMetaType>
#include "qabstractbinmsg.h"

namespace QRserver
{
    class QMsgSendCmdSucceed : public QAbstractBinMsg
    {
            Q_OBJECT

            Q_PROPERTY(quint32 sendMessageType READ sendMessageType WRITE
                       setSendMessageType)

        public:
            QMsgSendCmdSucceed(quint32 sendMessageType = 0, QObject *parent = nullptr);
            QMsgSendCmdSucceed(const QMsgSendCmdSucceed &message);
            virtual ~QMsgSendCmdSucceed();

            const quint32 sendMessageType() const
            {
                return m_sendMessageType;
            }
            void setSendMessageType(const quint32 sendMessageType)
            {
                m_sendMessageType = sendMessageType;
            }

        protected:
            virtual QDataStream &read(QDataStream &in) ;
            virtual QDataStream &write(QDataStream &out) const;
            virtual const QByteArray toByteArray() const;

        private:
            quint32 	m_sendMessageType;
    };
}

Q_DECLARE_METATYPE(QRserver::QMsgSendCmdSucceed)

#endif
