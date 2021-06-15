#ifndef QMSGFILEFINISHLONGFILENAME_H
#define QMSGFILEFINISHLONGFILENAME_H

#include "qmsgfileblocklongfilename.h"

namespace QRserver
{
    class QMsgFileFinishLongFileName : public QMsgFileBlockLongFileName
    {
            Q_OBJECT

        public:
            QMsgFileFinishLongFileName(QObject *parent = nullptr);
            QMsgFileFinishLongFileName(const QMsgFileFinishLongFileName &message);
            virtual ~QMsgFileFinishLongFileName();
    };
}

Q_DECLARE_METATYPE(QRserver::QMsgFileFinishLongFileName)

#endif
