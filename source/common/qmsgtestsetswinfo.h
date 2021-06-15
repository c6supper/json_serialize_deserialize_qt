/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QMSGTESTSETSWINFO_H
#define QMSGTESTSETSWINFO_H

#include "qabstractbinmsg.h"
#include "qstringattribute.h"

namespace QRserver
{
    class QMsgTestsetSwInfo : public QAbstractBinMsg
    {
            Q_OBJECT

            Q_PROPERTY(qint32 softwareStatus READ softwareStatus WRITE setSoftwareStatus)
            Q_PROPERTY(qint32 softwareTime READ softwareTime WRITE setSoftwareTime)
            Q_PROPERTY(qint32 softwareLength READ softwareLength WRITE setSoftwareLength)
            Q_PROPERTY(QByteArray softwareMd5 READ softwareMd5 WRITE setSoftwareMd5)

        public:
            QMsgTestsetSwInfo(QObject *parent = nullptr);
            QMsgTestsetSwInfo(const QMsgTestsetSwInfo &message);
            virtual ~QMsgTestsetSwInfo();

            const qint32 softwareStatus() const
            {
                return m_softwareStatus;
            }
            void setSoftwareStatus(const qint32 softwareStatus)
            {
                m_softwareStatus = softwareStatus;
            }
            const qint32 softwareTime() const
            {
                return m_softwareTime;
            }
            void setSoftwareTime(const qint32 softwareTime)
            {
                m_softwareTime = softwareTime;
            }
            const qint32 softwareLength() const
            {
                return m_softwareLength;
            }
            void setSoftwareLength(const qint32 softwareLength)
            {
                m_softwareLength = softwareLength;
            }
            const QByteArray softwareMd5() const
            {
                return m_softwareMd5;
            }
            void setSoftwareMd5(const QByteArray softwareMd5)
            {
                m_softwareMd5 = softwareMd5;
            }

        protected:
            virtual QDataStream &read(QDataStream &in) ;
            virtual QDataStream &write(QDataStream &out) const;
            virtual const QByteArray toByteArray() const;
            virtual void initAttributes();

        protected:
            qint32			m_softwareStatus, m_softwareTime, m_softwareLength;
            QByteArray 		m_softwareMd5;
    };
}

Q_DECLARE_METATYPE(QRserver::QMsgTestsetSwInfo)

#endif
