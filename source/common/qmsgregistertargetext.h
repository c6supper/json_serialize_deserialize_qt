/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QMSGREGISTERTARGETEXT_H
#define QMSGREGISTERTARGETEXT_H

#include "qmsgregistertarget.h"

namespace QRserver
{
    class QMsgRegisterTargetExt : public QMsgRegisterTarget
    {
            Q_OBJECT

            Q_PROPERTY(QString supervisorId READ supervisorId WRITE setSupervisorId)

        public:
            QMsgRegisterTargetExt(QObject *parent = nullptr);
            QMsgRegisterTargetExt(const QMsgRegisterTargetExt &message);
            virtual ~QMsgRegisterTargetExt();

            QString supervisorId() const;
            void setSupervisorId(QString supervisorId);

        protected:
            virtual QDataStream &read(QDataStream &in) ;
            virtual QDataStream &write(QDataStream &out) const;
            virtual const QByteArray toByteArray() const;
            virtual void initAttributes();

        private:
            QSharedPointer<QProfile::QStringAttribute> 		m_supervisorId;
    };
}

Q_DECLARE_METATYPE(QRserver::QMsgRegisterTargetExt)

#endif
