/*
 * qmsguploadprofileinfoend.h
 *
 *  Created on: Sep 14, 2020
 *      Author: duyi
 */

#ifndef QMSGUPLOADPROFILEINFOEND_H
#define QMSGUPLOADPROFILEINFOEND_H
#include <QMetaType>
#include "qabstractbinmsg.h"

namespace QRserver
{
    class QMsgUploadProfileInfoEnd : public QAbstractBinMsg
    {
            Q_OBJECT

            Q_PROPERTY(qint32 profileInfoCount READ profileInfoCount WRITE
                       setProfileInfoCount)

        public:
            QMsgUploadProfileInfoEnd(QObject *parent = nullptr);
            QMsgUploadProfileInfoEnd(const QMsgUploadProfileInfoEnd &message);
            virtual ~QMsgUploadProfileInfoEnd();
            qint32 profileInfoCount() const;
            void setProfileInfoCount(qint32  profileInfoCount);

        protected:
            virtual QDataStream &read(QDataStream &in) ;
            virtual QDataStream &write(QDataStream &out) const;
            virtual const QByteArray toByteArray() const;

        private:
            qint32 m_profileInfoCount;
    };
}

Q_DECLARE_METATYPE(QRserver::QMsgUploadProfileInfoEnd)

#endif /* QMSGUPLOADPROFILEINFOEND_H */
