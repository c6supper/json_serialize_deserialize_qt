/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QMSGGETORGCHARINFO_H
#define QMSGGETORGCHARINFO_H

#include <QMetaType>
#include <QSharedPointer>
#include <QDateTime>
#include "qabstractbinmsg.h"
#include "qprofile_debug.h"
#include "qprofile_global.h"
#include "qstringattribute.h"

namespace QRserver
{
    class QMsgGetOrgChartInfo : public QAbstractBinMsg
    {
            Q_OBJECT

            Q_PROPERTY(QString region READ region WRITE setRegion)
            Q_PROPERTY(QString district READ district WRITE setDistrict)
            Q_PROPERTY(QString system READ system WRITE setSystem)
            Q_PROPERTY(QString supervisorId READ supervisorId WRITE setSupervisorId)

        public:
            QMsgGetOrgChartInfo(QObject *parent = nullptr);
            QMsgGetOrgChartInfo(const QMsgGetOrgChartInfo &message);
            virtual ~QMsgGetOrgChartInfo();

            QString supervisorId() const;
            void setSupervisorId(QString supervisorId);
            QString district() const;
            void setDistrict(QString district);
            QString system() const;
            void setSystem(QString system);
            QString region() const;
            void setRegion(QString region);

        protected:
            virtual QDataStream &read(QDataStream &in) ;
            virtual QDataStream &write(QDataStream &out) const;
            virtual const QByteArray toByteArray() const;
            virtual void initAttributes();

        private:
            QSharedPointer<QProfile::QStringAttribute> 		m_region;
            QSharedPointer<QProfile::QStringAttribute> 		m_district;
            QSharedPointer<QProfile::QStringAttribute> 		m_system;
            QSharedPointer<QProfile::QStringAttribute> 		m_supervisorId;
    };
}

Q_DECLARE_METATYPE(QRserver::QMsgGetOrgChartInfo)

#endif
