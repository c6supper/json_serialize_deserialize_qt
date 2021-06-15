#ifndef QMSGTESTSETSTATUS_H
#define QMSGTESTSETSTATUS_H

#include <QMetaType>
#include <QSharedPointer>
#include "qabstractbinmsg.h"
#include "qprofile_debug.h"
#include "qprofile_global.h"
#include "qstringattribute.h"
#include "qnumericattribute.h"

namespace QRserver
{
    class QMsgTestsetStatus : public QAbstractBinMsg
    {
            Q_OBJECT

            Q_PROPERTY(QString serialNumber READ serialNumber WRITE setSerialNumber)
            Q_PROPERTY(qint32 testsetStatus READ testsetStatus WRITE setTestsetStatus)

        public:
            QMsgTestsetStatus(QObject *parent = nullptr);
            QMsgTestsetStatus(const QMsgTestsetStatus &message);
            virtual ~QMsgTestsetStatus();

            QString serialNumber() const;
            void setSerialNumber(QString serialNumber);
            qint32 testsetStatus() const;
            void setTestsetStatus(qint32 status);

        protected:
            virtual QDataStream &read(QDataStream &in) ;
            virtual QDataStream &write(QDataStream &out) const;
            virtual const QByteArray toByteArray() const;
            virtual void initAttributes();

        private:
            QSharedPointer<QProfile::QStringAttribute> m_serialNumber;
            QSharedPointer<QProfile::QNumericAttribute> m_testsetStatus;
    };
}

Q_DECLARE_METATYPE(QRserver::QMsgTestsetStatus)

#endif // QMSGTESTSETSTATUS_H
