/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QMSGPROFILEINFO_H
#define QMSGPROFILEINFO_H

#include <QMetaType>
#include <QSharedPointer>
#include <QVariant>
#include <QMutexLocker>
#include "qabstractbinmsg.h"
#include "qprofile_debug.h"
#include "qprofile_global.h"

namespace QRserver
{
    class QMsgProfileInfo : public QAbstractBinMsg
    {
            Q_OBJECT

            Q_PROPERTY(QVariantList profileInfoList READ profileInfoList WRITE
                       setProfileInfoList)

        public:
            QMsgProfileInfo(QObject *parent = nullptr);
            QMsgProfileInfo(const QMsgProfileInfo &message);
            virtual ~QMsgProfileInfo();

            QVariantList profileInfoList() const;
            void setProfileInfoList(QVariantList profileInfoList);

        protected:
            virtual QDataStream &read(QDataStream &in) ;
            virtual QDataStream &write(QDataStream &out) const;
            virtual const QByteArray toByteArray() const;

        private:
            QVariantList	m_profileInfoList;
            QMutex			*m_mutex;
    };
}

Q_DECLARE_METATYPE(QRserver::QMsgProfileInfo)

#endif
