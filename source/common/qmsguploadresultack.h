#ifndef QMSGUPLOADRESULTACK_H
#define QMSGUPLOADRESULTACK_H

#include <QMetaType>
#include "qabstractbinmsg.h"
#include "qprofile_debug.h"
#include "qprofile_global.h"
#include "qstringattribute.h"

namespace QRserver
{
    class QMsgUploadResultAck : public QAbstractBinMsg
    {
            Q_OBJECT

            Q_PROPERTY(QString resultName READ resultName WRITE setResultName)
            Q_PROPERTY(qint32 resultType READ resultType WRITE setResultType)
            Q_PROPERTY(qint32 resultStatus READ resultStatus WRITE setResultStatus)

        public:
            enum ResultAckStatus {Succeed = 0, XmlError = 1, InvalidStatus};
            QMsgUploadResultAck(QObject *parent = nullptr, bool ignoreStatus = false);
            QMsgUploadResultAck(const QMsgUploadResultAck &message);
            virtual ~QMsgUploadResultAck();

            const QString resultName();
            void setResultName(QString resultName);

            qint32 resultType() const;
            void setResultType(qint32 resultType);

            qint32 resultStatus() const;
            void setResultStatus(qint32 resultStatus);

        protected:
            virtual QDataStream &read(QDataStream &in) ;
            virtual QDataStream &write(QDataStream &out) const;
            virtual const QByteArray toByteArray() const;
            virtual void initAttributes();
            virtual const QStringList ignoredProperties() const;

        private:
            qint32 							m_resultType;
            qint32 							m_resultStatus;
            bool m_ignoreStatus;

        protected:
            QProfile::QStringAttribute 		*m_resultName;
            //set it protected to use functions in QStringAttribute in derived classes.
            //resultName() only return a QString object.
    };
}

Q_DECLARE_METATYPE(QRserver::QMsgUploadResultAck)

#endif
