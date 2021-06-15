/*
 * QMsgTestsetSWVersionAndURL.h
 *
 *  Created on: Nov 17, 2020
 *      Author: duyi
 */

#ifndef TESTSETSWVERSIONANDURL_H
#define TESTSETSWVERSIONANDURL_H

#include <QMetaType>
#include <QSharedPointer>
#include "qabstractbinmsg.h"
#include "qprofile_debug.h"
#include "qprofile_global.h"
#include "qnumericattribute.h"
#include "qstringattribute.h"

namespace QRserver
{
    class QMsgTestsetSWVersionAndURL : public QAbstractBinMsg
    {
            Q_OBJECT

            Q_PROPERTY(quint32 deviceType READ deviceType WRITE setDeviceType)
            Q_PROPERTY(QString version READ version WRITE setVersion)
            Q_PROPERTY(QString fullImageURL READ fullImageURL WRITE setFullImageURL)
            Q_PROPERTY(QString deltaImageURL READ deltaImageURL WRITE setDeltaImageURL)
            Q_PROPERTY(QString upgradeFromVersion READ upgradeFromVersion WRITE
                       setUpgradeFromVersion)
            Q_PROPERTY(QString fullImageChecksum READ fullImageChecksum WRITE
                       setFullImageChecksum)
            Q_PROPERTY(QString deltaImageChecksum READ deltaImageChecksum WRITE
                       setDeltaImageChecksum)

        public:
            QMsgTestsetSWVersionAndURL(QObject *parent = nullptr);
            QMsgTestsetSWVersionAndURL(const QMsgTestsetSWVersionAndURL &message);
            virtual ~QMsgTestsetSWVersionAndURL();

            qint32 deviceType() const;
            void setDeviceType(qint32 status);

            QString version() const;
            void setVersion(QString version);

            QString fullImageURL() const;
            void setFullImageURL(QString fullImageURL);

            QString deltaImageURL() const;
            void setDeltaImageURL(QString deltaImageURL);

            QString upgradeFromVersion() const;
            void setUpgradeFromVersion(QString upgradeFromVersion);

            QString fullImageChecksum() const;
            void setFullImageChecksum(QString fullImageChecksum);

            QString deltaImageChecksum() const;
            void setDeltaImageChecksum(QString deltaImageChecksum);

        protected:
            virtual QDataStream &read(QDataStream &in) ;
            virtual QDataStream &write(QDataStream &out) const;
            virtual const QByteArray toByteArray() const;
            virtual void initAttributes();

        private:
            QSharedPointer<QProfile::QNumericAttribute> m_deviceType;
            QSharedPointer<QProfile::QStringAttribute> m_version;
            QSharedPointer<QProfile::QStringAttribute> m_fullImageURL;
            QSharedPointer<QProfile::QStringAttribute> m_deltaImageURL;
            QSharedPointer<QProfile::QStringAttribute> m_upgradeFromVersion;
            QSharedPointer<QProfile::QStringAttribute> m_fullImageChecksum;
            QSharedPointer<QProfile::QStringAttribute> m_deltaImageChecksum;
    };
}

Q_DECLARE_METATYPE(QRserver::QMsgTestsetSWVersionAndURL)

#endif /* TESTSETSWVERSIONANDURL_H */
