/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QMSGTESTSETSWVERSIONINFO_H
#define QMSGTESTSETSWVERSIONINFO_H

#include "qabstractbinmsg.h"
#include "qstringattribute.h"

namespace QRserver
{
    class QMsgTestsetSwVersionInfo : public QAbstractBinMsg
    {
            Q_OBJECT

            Q_PROPERTY(qint32 imageType READ imageType WRITE setImageType)
            Q_PROPERTY(QString version READ version WRITE setVersion)

        public:
            QMsgTestsetSwVersionInfo(QObject *parent = nullptr);
            QMsgTestsetSwVersionInfo(const QMsgTestsetSwVersionInfo &message);
            virtual ~QMsgTestsetSwVersionInfo();

            const qint32 imageType() const
            {
                return m_imageType;
            }
            void setImageType(const qint32 imageType)
            {
                m_imageType = imageType;
            }

            const QString version() const
            {
                return m_version->value().toString();
            }
            void setVersion(const QString version)
            {
                m_version->setValue(version);
            }


        protected:
            virtual QDataStream &read(QDataStream &in) ;
            virtual QDataStream &write(QDataStream &out) const;
            virtual const QByteArray toByteArray() const;
            virtual void initAttributes();

        protected:
            qint32							m_imageType;
            QProfile::QStringAttribute 		*m_version;
    };
}

Q_DECLARE_METATYPE(QRserver::QMsgTestsetSwVersionInfo)

#endif
