/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QMSGGETSERVERINFO_H
#define QMSGGETSERVERINFO_H

#include <QMetaType>
#include <QSharedPointer>
#include <QDateTime>
#include "qabstractbinmsg.h"
#include "qprofile_debug.h"
#include "qprofile_global.h"
#include "qstringattribute.h"
#include "qdatetimeattribute.h"

namespace QRserver
{
    class QMsgGetServerInfo : public QAbstractBinMsg
    {
            Q_OBJECT

            Q_PROPERTY(QString serverName READ serverName WRITE setServerName)
            Q_PROPERTY(QString serverLocation READ serverLocation WRITE setServerLocation)
            Q_PROPERTY(QDateTime serverDateTime READ serverDateTime WRITE setServerDateTime)

        public:
            QMsgGetServerInfo(QObject *parent = nullptr);
            QMsgGetServerInfo(const QMsgGetServerInfo &message);
            virtual ~QMsgGetServerInfo();

            QString serverName() const;
            void setServerName(QString serverName);
            QString serverLocation() const;
            void setServerLocation(QString serverLocation);
            QDateTime serverDateTime() const;
            void setServerDateTime(QDateTime dateTime);

        protected:
            virtual QDataStream &read(QDataStream &in) ;
            virtual QDataStream &write(QDataStream &out) const;
            virtual const QByteArray toByteArray() const;
            virtual void initAttributes();

        private:
            QSharedPointer<QProfile::QStringAttribute> 		m_serverName;
            QSharedPointer<QProfile::QStringAttribute> 		m_serverLocation;
            QSharedPointer<QProfile::QDateTimeAttribute> 	m_serverDateTime;
    };
}

Q_DECLARE_METATYPE(QRserver::QMsgGetServerInfo)

#endif
