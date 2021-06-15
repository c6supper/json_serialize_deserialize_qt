/*
 * qmsgblockprofileandchangetechname.h
 *
 *  Created on: Jul 28, 2020
 *      Author: duyi
 */

#ifndef QMSGBLOCKPROFILEANDCHANGETECHNAME_H
#define QMSGBLOCKPROFILEANDCHANGETECHNAME_H

#include <QMetaType>
#include "qabstractbinmsg.h"

namespace QRserver
{
    class QMsgBlockProfileAndChangeTechName : public QAbstractBinMsg
    {
            Q_OBJECT

            Q_PROPERTY(bool blockProfileUpload READ blockProfileUpload WRITE
                       setBlockProfileUpload)
            Q_PROPERTY(bool changeTechName READ changeTechName WRITE setChangeTechName)

        public:
            QMsgBlockProfileAndChangeTechName(QObject *parent = nullptr);
            QMsgBlockProfileAndChangeTechName(const QMsgBlockProfileAndChangeTechName
                                              &message);
            virtual ~QMsgBlockProfileAndChangeTechName();
            bool blockProfileUpload() const;
            void setBlockProfileUpload(bool blockProfileUpload);
            bool changeTechName() const;
            void setChangeTechName(bool changeTechName);

        protected:
            virtual QDataStream &read(QDataStream &in) ;
            virtual QDataStream &write(QDataStream &out) const;
            virtual const QByteArray toByteArray() const;

        private:
            bool m_blockProfileUpload;
            bool m_changeTechName;
    };
}

Q_DECLARE_METATYPE(QRserver::QMsgBlockProfileAndChangeTechName)



#endif /* QMSGBLOCKPROFILEANDCHANGETECHNAME_H */
