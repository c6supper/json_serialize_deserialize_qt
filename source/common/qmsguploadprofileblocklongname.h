#ifndef QMSGUPLOADPROFILEBLOCKLONGNAME_H
#define QMSGUPLOADPROFILEBLOCKLONGNAME_H

#include "qmsguploadprofileblock.h"

namespace QRserver
{
    class QMsgUploadProfileBlockLongName : public QMsgUploadProfileBlock
    {
            Q_OBJECT

        public:
            QMsgUploadProfileBlockLongName(QObject *parent = nullptr);
            QMsgUploadProfileBlockLongName(const QMsgUploadProfileBlockLongName &message);
            virtual ~QMsgUploadProfileBlockLongName();

        protected:
            virtual QDataStream &read(QDataStream &in) ;
            virtual QDataStream &write(QDataStream &out) const;
            virtual const QByteArray toByteArray() const;

    };
}

Q_DECLARE_METATYPE(QRserver::QMsgUploadProfileBlockLongName)

#endif /* QMSGUPLOADPROFILEBLOCKLONGNAME_H */
