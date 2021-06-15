#ifndef QMSGUPDATETECHNAME_H
#define QMSGUPDATETECHNAME_H

#include <QMetaType>
#include "qabstractbinmsg.h"
#include "qprofile_debug.h"
#include "qprofile_global.h"
#include "qstringattribute.h"

namespace QRserver
{
    class QMsgUpdateTechName : public QAbstractBinMsg
    {
            Q_OBJECT

            Q_PROPERTY(QString techName READ techName WRITE setTechName)
            Q_PROPERTY(bool updataTechNameResult READ updataTechNameResult WRITE
                       setUpdataTechNameResult)

        public:
            QMsgUpdateTechName(QObject *parent = nullptr);
            QMsgUpdateTechName(const QMsgUpdateTechName &message);
            virtual ~QMsgUpdateTechName();
            QString techName() const;
            void setTechName(QString techName);
            bool updataTechNameResult() const;
            void setUpdataTechNameResult(bool updataTechNameResult);

        protected:
            virtual QDataStream &read(QDataStream &in) ;
            virtual QDataStream &write(QDataStream &out) const;
            virtual const QByteArray toByteArray() const;
            virtual void initAttributes();

        private:
            QSharedPointer<QProfile::QStringAttribute> m_techName;
            bool m_updataTechNameResult;
    };
}

Q_DECLARE_METATYPE(QRserver::QMsgUpdateTechName)

#endif /* QMSGUPDATETECHNAME_H */
