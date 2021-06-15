/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QMSGREGISTRATIONLOCK_H
#define QMSGREGISTRATIONLOCK_H

#include <QMetaType>
#include "qabstractbinmsg.h"

namespace QRserver
{
    class QMsgRegistrationLock : public QAbstractBinMsg
    {
            Q_OBJECT

            Q_PROPERTY(bool registrationBomb READ registrationBomb WRITE
                       setRegistrationBomb)
            Q_PROPERTY(bool lockTechId READ lockTechId WRITE setLockTechId)
            Q_PROPERTY(bool maintenance READ maintenance WRITE setMaintenance)

        public:
            QMsgRegistrationLock(QObject *parent = nullptr);
            QMsgRegistrationLock(const QMsgRegistrationLock &message);
            virtual ~QMsgRegistrationLock();

            bool lockTechId() const;
            void setLockTechId(bool lockTechId);

            bool registrationBomb() const;
            void setRegistrationBomb(bool registrationBomb);

            bool maintenance() const;
            void setMaintenance(bool maintenance);

        protected:
            virtual QDataStream &read(QDataStream &in) ;
            virtual QDataStream &write(QDataStream &out) const;
            virtual const QByteArray toByteArray() const;

        private:
            bool m_registrationBomb;
            bool m_lockTechId;
            bool m_maintenance;
    };
}

Q_DECLARE_METATYPE(QRserver::QMsgRegistrationLock)

#endif
