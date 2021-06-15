/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QMSGGETORGCHARISTATUS_H
#define QMSGGETORGCHARISTATUS_H

#include <QMetaType>
#include <QSharedPointer>
#include <QDateTime>
#include "qabstractbinmsg.h"
#include "qprofile_debug.h"
#include "qprofile_global.h"
#include "qnumericattribute.h"

namespace QRserver
{
    class QMsgGetOrgChartStatus : public QAbstractBinMsg
    {
            Q_OBJECT

            Q_PROPERTY(quint32 status READ status WRITE setStatus)

        public:
            QMsgGetOrgChartStatus(QObject *parent = nullptr);
            QMsgGetOrgChartStatus(const QMsgGetOrgChartStatus &message);
            virtual ~QMsgGetOrgChartStatus();

            quint32 status() const;
            void setStatus(quint32 status);

        protected:
            virtual QDataStream &read(QDataStream &in) ;
            virtual QDataStream &write(QDataStream &out) const;
            virtual const QByteArray toByteArray() const;
            virtual void initAttributes();

        private:
            QSharedPointer<QProfile::QNumericAttribute> 		m_status;
    };
}

Q_DECLARE_METATYPE(QRserver::QMsgGetOrgChartStatus)

#endif
