/*
 * qmsglockveexpressupgrade.h
 *
 *  Created on: Jul 23, 2020
 *      Author: duyi
 */

#ifndef QMSGLOCKVEEXPRESSUPGRADE_H
#define QMSGLOCKVEEXPRESSUPGRADE_H

#include <QMetaType>
#include "qabstractbinmsg.h"

namespace QRserver
{
    class QMsgLockVeexpressUpgrade : public QAbstractBinMsg
    {
            Q_OBJECT

            Q_PROPERTY(bool lockVeexpressUpgrade READ lockVeexpressUpgrade WRITE
                       setLockVeexpressUpgrade)

        public:
            QMsgLockVeexpressUpgrade(QObject *parent = nullptr);
            QMsgLockVeexpressUpgrade(const QMsgLockVeexpressUpgrade &message);
            virtual ~QMsgLockVeexpressUpgrade();

            const bool lockVeexpressUpgrade() const
            {
                return m_lockVeexpressUpgrade;
            }
            void setLockVeexpressUpgrade(const bool lockVeexpressUpgrade)
            {
                m_lockVeexpressUpgrade = lockVeexpressUpgrade;
            }


        protected:
            virtual QDataStream &read(QDataStream &in) ;
            virtual QDataStream &write(QDataStream &out) const;
            virtual const QByteArray toByteArray() const;

        protected:
            bool       m_lockVeexpressUpgrade;
    };
}

Q_DECLARE_METATYPE(QRserver::QMsgLockVeexpressUpgrade)


#endif /* QMSGLOCKVEEXPRESSUPGRADE_H */
