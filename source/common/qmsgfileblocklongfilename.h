#ifndef QMSGFILEBLOCKLONGFILENAME_H
#define QMSGFILEBLOCKLONGFILENAME_H

#include "qmsgfileblock.h"

namespace QRserver
{
    class QMsgFileBlockLongFileName : public QMsgFileBlock
    {
            Q_OBJECT

        public:
            QMsgFileBlockLongFileName(QObject *parent = nullptr);
            QMsgFileBlockLongFileName(const QMsgFileBlockLongFileName &message);
            virtual ~QMsgFileBlockLongFileName();

        protected:
            virtual QDataStream &read(QDataStream &in) ;
            virtual QDataStream &write(QDataStream &out) const;
            virtual const QByteArray toByteArray() const;
    };
}

Q_DECLARE_METATYPE(QRserver::QMsgFileBlockLongFileName)

#endif
