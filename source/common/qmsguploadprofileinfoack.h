/*
 * qmsguploadprofileinfoack.h
 *
 *  Created on: Sep 15, 2020
 *      Author: duyi
 */

#ifndef QMSGUPLOADPROFILEINFOACK_H
#define QMSGUPLOADPROFILEINFOACK_H

#include <QMetaType>
#include "qabstractbinmsg.h"
#include "qprofile_debug.h"

namespace QRserver
{
    class QMsgUploadProfileInfoAck : public QAbstractBinMsg
    {
            Q_OBJECT

            Q_PROPERTY(qint8 uploadProfileInfoStatus READ uploadProfileInfoStatus WRITE
                       setUploadProfileInfoStatus)

        public:
            enum UploadProfileInfoStatus {Succeed = 0, WrongNumber = 2, Failed};
            QMsgUploadProfileInfoAck(QObject *parent = nullptr, bool ignoreStatus = false);
            QMsgUploadProfileInfoAck(const QMsgUploadProfileInfoAck &message);
            virtual ~QMsgUploadProfileInfoAck();
            qint8 uploadProfileInfoStatus() const;
            void setUploadProfileInfoStatus(qint8 uploadProfileInfoStatus);

        protected:
            virtual QDataStream &read(QDataStream &in) ;
            virtual QDataStream &write(QDataStream &out) const;
            virtual const QByteArray toByteArray() const;
            virtual const QStringList ignoredProperties() const;

        private:
            bool m_ignoreStatus;
            qint8 m_uploadProfileInfoStatus;
    };
}

Q_DECLARE_METATYPE(QRserver::QMsgUploadProfileInfoAck)

#endif /* QMSGUPLOADPROFILEINFOACK_H */
