#ifndef QMSGGETPROFILEINFO_H
#define QMSGGETPROFILEINFO_H

#include <QMetaType>
#include "qabstractbinmsg.h"

namespace QRserver
{
    class QMsgGetProfileInfo : public QAbstractBinMsg
    {
            Q_OBJECT

            Q_PROPERTY(qint32 count READ count WRITE setCount)

        public:
            QMsgGetProfileInfo(QObject *parent = nullptr);
            QMsgGetProfileInfo(const QMsgGetProfileInfo &message);
            virtual ~QMsgGetProfileInfo();

            const qint32 count() const;
            void setCount(const qint32 count);

        protected:
            virtual QDataStream &read(QDataStream &in) ;
            virtual QDataStream &write(QDataStream &out) const;
            virtual const QByteArray toByteArray() const;

        protected:
            qint32 m_count;
    };
}

Q_DECLARE_METATYPE(QRserver::QMsgGetProfileInfo)

#endif /* QMSGGETPROFILEINFO_H */

