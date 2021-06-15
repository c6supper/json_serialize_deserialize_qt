#ifndef QMSGUPLOADPROFILEACK_H
#define QMSGUPLOADPROFILEACK_H

#include <QMetaType>
#include "qabstractbinmsg.h"
#include "qprofile_debug.h"
#include "qprofile_global.h"
#include "qstringattribute.h"
#include "qmsguploadresultack.h"

namespace QRserver
{
    class QMsgUploadProfileAck : public QMsgUploadResultAck
    {
            Q_OBJECT

        public:
            enum ProfileAckStatus {Succeed = 0, InvalidEncryptedFile = 1};
            QMsgUploadProfileAck(QObject *parent = nullptr, bool ignoreStatus = false);
            QMsgUploadProfileAck(const QMsgUploadProfileAck &message);
            virtual ~QMsgUploadProfileAck();

        protected:
            virtual QDataStream &read(QDataStream &in) ;
            virtual QDataStream &write(QDataStream &out) const;
            virtual const QByteArray toByteArray() const;
    };
}

Q_DECLARE_METATYPE(QRserver::QMsgUploadProfileAck)

#endif /* QMSGUPLOADPROFILEACK_H */
