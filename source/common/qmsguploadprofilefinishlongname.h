#ifndef QMSGUPLOADPROFILEFINISHLONGNAME_H
#define QMSGUPLOADPROFILEFINISHLONGNAME_H

#include "qmsguploadprofileblocklongname.h"

namespace QRserver
{
    class QMsgUploadProfileFinishLongName : public QMsgUploadProfileBlockLongName
    {
            Q_OBJECT

        public:
            QMsgUploadProfileFinishLongName(QObject *parent = nullptr);
            QMsgUploadProfileFinishLongName(const QMsgUploadProfileFinishLongName &message);
            virtual ~QMsgUploadProfileFinishLongName();

            virtual quint32 blockLength() const;
    };
}

Q_DECLARE_METATYPE(QRserver::QMsgUploadProfileFinishLongName)



#endif /* QMSGUPLOADPROFILEFINISHLONGNAME_H */
