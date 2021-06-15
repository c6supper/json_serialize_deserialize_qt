#ifndef QMSGFOLDERINFOLONGFILENAME_H
#define QMSGFOLDERINFOLONGFILENAME_H

#include "qmsgfolderinfo.h"

namespace QRserver
{
    class QMsgFolderInfoLongFileName : public QMsgFolderInfo
    {
            Q_OBJECT

        public:
            QMsgFolderInfoLongFileName(QObject *parent = nullptr);
            QMsgFolderInfoLongFileName(const QMsgFolderInfoLongFileName &message);
            virtual ~QMsgFolderInfoLongFileName();

        protected:
            virtual QDataStream &read(QDataStream &in) ;
            virtual QDataStream &write(QDataStream &out) const;
            virtual const QByteArray toByteArray() const;
    };
}

Q_DECLARE_METATYPE(QRserver::QMsgFolderInfoLongFileName)

#endif
