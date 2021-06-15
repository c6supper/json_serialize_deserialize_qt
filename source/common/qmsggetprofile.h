/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QMSGGETPROFILE_H
#define QMSGGETPROFILE_H

#include <QSharedPointer>
#include "qabstractbinmsg.h"
#include "qprofile_debug.h"
#include "qprofile_global.h"
#include "qstringattribute.h"
#include "qnumericattribute.h"

namespace QRserver
{
    class QMsgGetProfile : public QAbstractBinMsg
    {
            Q_OBJECT

            Q_PROPERTY(QString profileName READ profileName WRITE setProfileName)
            Q_PROPERTY(quint32 fileType READ fileType WRITE setFileType)

        public:
            QMsgGetProfile(QObject *parent = nullptr);
            QMsgGetProfile(const QMsgGetProfile &message);
            virtual ~QMsgGetProfile();

            QString profileName() const;
            void setProfileName(const QString profileName);

            quint32 fileType() const;
            void setFileType(quint32 fileType);

        protected:
            virtual QDataStream &read(QDataStream &in) ;
            virtual QDataStream &write(QDataStream &out) const;
            virtual const QByteArray toByteArray() const;
            virtual void initAttributes();

        private:
            QSharedPointer<QProfile::QStringAttribute> 		m_profileName;
            QSharedPointer<QProfile::QNumericAttribute> 	m_fileType;
    };
}

Q_DECLARE_METATYPE(QRserver::QMsgGetProfile)

#endif
