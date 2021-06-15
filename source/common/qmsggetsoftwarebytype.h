/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QMSGGETSOFTWAREBYTYPE_H
#define QMSGGETSOFTWAREBYTYPE_H

#include <QMetaType>
#include <QSharedPointer>
#include <QDateTime>
#include "qabstractbinmsg.h"
#include "qprofile_debug.h"
#include "qprofile_global.h"
#include "qstringattribute.h"
#include "qnumericattribute.h"

namespace QRserver
{
    class QMsgGetSoftwareByType : public QAbstractBinMsg
    {
            Q_OBJECT

            Q_PROPERTY(qint32 deviceType READ deviceType WRITE setDeviceType)
            Q_PROPERTY(QString softwareVersion READ softwareVersion WRITE
                       setSoftwareVersion)
        public:
            QMsgGetSoftwareByType(QObject *parent = nullptr);
            QMsgGetSoftwareByType(const QMsgGetSoftwareByType &message);
            virtual ~QMsgGetSoftwareByType();

            QString softwareVersion() const;
            void setSoftwareVersion(QString softwareVersion);
            qint32 deviceType() const;
            void setDeviceType(qint32 deviceType);

        protected:
            virtual QDataStream &read(QDataStream &in) ;
            virtual QDataStream &write(QDataStream &out) const;
            virtual const QByteArray toByteArray() const;
            virtual void initAttributes();

        private:
            QSharedPointer<QProfile::QStringAttribute> 		m_softwareVersion;
            QSharedPointer<QProfile::QNumericAttribute> 	m_deviceType;
    };
}

Q_DECLARE_METATYPE(QRserver::QMsgGetSoftwareByType)

#endif
