/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QMSGGETSOFTWAREVERSION_H
#define QMSGGETSOFTWAREVERSION_H

#include <QMetaType>
#include <QSharedPointer>
#include <QDateTime>
#include "qabstractbinmsg.h"
#include "qprofile_debug.h"
#include "qprofile_global.h"
#include "qstringattribute.h"
#include "qnumericattribute.h"

namespace QRserver
{
    class QMsgGetSoftwareVersion : public QAbstractBinMsg
    {
            Q_OBJECT

            Q_PROPERTY(QString softwareVersion READ softwareVersion WRITE
                       setSoftwareVersion)
            /*            Q_PROPERTY(QString deltaVersion READ deltaVersion WRITE setDeltaVersion)
                        Q_PROPERTY(QString basicVersion READ basicVersion WRITE setBasicVersion)
                        Q_PROPERTY(qint32 fullPkgSize READ fullPkgSize WRITE setFullPkgSize)
                        Q_PROPERTY(qint32 deltaPkgSize READ deltaPkgSize WRITE setDeltaPkgSize)*/

        public:
            QMsgGetSoftwareVersion(QObject *parent = nullptr);
            QMsgGetSoftwareVersion(const QMsgGetSoftwareVersion &message);
            virtual ~QMsgGetSoftwareVersion();

            QString softwareVersion() const;
            void setSoftwareVersion(QString softwareVersion);
            /*            QString deltaVersion() const;
                        void setDeltaVersion(QString deltaVersion);
                        QString basicVersion() const;
                        void setBasicVersion(QString basicVersion);
                        qint32 fullPkgSize() const;
                        void setFullPkgSize(qint32 fullPkgSize);
                        qint32 deltaPkgSize() const;
                        void setDeltaPkgSize(qint32 deltaPkgSize);*/

        protected:
            virtual QDataStream &read(QDataStream &in) ;
            virtual QDataStream &write(QDataStream &out) const;
            virtual const QByteArray toByteArray() const;
            virtual void initAttributes();

        private:
            QSharedPointer<QProfile::QStringAttribute> 		m_softwareVersion;
            /*            QSharedPointer<QProfile::QStringAttribute> 		m_deltaVersion;
                        QSharedPointer<QProfile::QStringAttribute> 		m_basicVersion;
                        QSharedPointer<QProfile::QNumericAttribute> 	m_deltaPkgSize;
                        QSharedPointer<QProfile::QNumericAttribute> 	m_fullPkgSize;*/
    };
}

Q_DECLARE_METATYPE(QRserver::QMsgGetSoftwareVersion)

#endif
