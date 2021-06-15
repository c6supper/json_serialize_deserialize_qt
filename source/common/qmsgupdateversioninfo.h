/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QMSGUPDATEVERSIONINFO_H
#define QMSGUPDATEVERSIONINFO_H

#include <QSharedPointer>
#include "qabstractbinmsg.h"
#include "qprofile_debug.h"
#include "qprofile_global.h"
#include "qstringattribute.h"
#include "qdeviceinfo.h"

namespace QRserver
{
    class QMsgUpdateVersionInfo : public QAbstractBinMsg
    {
            Q_OBJECT

            Q_PROPERTY(QVariantList deviceList READ deviceList WRITE setDeviceList)

        public:
            QMsgUpdateVersionInfo(QObject *parent = nullptr);
            QMsgUpdateVersionInfo(const QMsgUpdateVersionInfo &message);
            virtual ~QMsgUpdateVersionInfo();

            const QVariantList deviceList() const;
            void setDeviceList(const QVariantList deviceList);

        protected:
            virtual QDataStream &read(QDataStream &in) ;
            virtual QDataStream &write(QDataStream &out) const;
            virtual const QByteArray toByteArray() const;
            virtual void initAttributes();

        private:
            QSharedPointer<QProfile::QStringAttribute> 		m_deviceVersion;
            QList<QDeviceInfo *>							m_deviceList;
            QMutex											*m_mutex;
    };
}

Q_DECLARE_METATYPE(QRserver::QMsgUpdateVersionInfo)

#endif
