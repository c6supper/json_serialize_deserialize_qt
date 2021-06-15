#ifndef QMSGCHECKALLLONGID_H
#define QMSGCHECKALLLONGID_H

#include "qmsgcheckall.h"

namespace QRserver
{
    class QMsgCheckAllLongId : public QMsgCheckAll
    {
            Q_OBJECT

        public:
            QMsgCheckAllLongId(QObject *parent = nullptr);
            QMsgCheckAllLongId(const QMsgCheckAllLongId &message);
            virtual ~QMsgCheckAllLongId();

        protected:
            virtual QDataStream &read(QDataStream &in) ;
            virtual QDataStream &write(QDataStream &out) const;
            virtual const QByteArray toByteArray() const;
    };
}

Q_DECLARE_METATYPE(QRserver::QMsgCheckAllLongId)

#endif /* QMSGCHECKALLLONGID_H */
