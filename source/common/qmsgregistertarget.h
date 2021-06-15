/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QMSGREGISTERTARGET_H
#define QMSGREGISTERTARGET_H

#include <QMetaType>
#include <QSharedPointer>
#include "qabstractbinmsg.h"
#include "qprofile_debug.h"
#include "qprofile_global.h"
#include "qstringattribute.h"
#include "qmacattribute.h"
#include "qregistermodel.h"
#include "qnumericattribute.h"

namespace QRserver
{
    class QMsgRegisterTarget : public QAbstractBinMsg
    {
            Q_OBJECT

            Q_PROPERTY(quint32 deviceType READ deviceType WRITE setDeviceType)
            Q_PROPERTY(QString serialNumber READ serialNumber WRITE setSerialNumber)
            Q_PROPERTY(QString mac READ mac WRITE setMac)
            Q_PROPERTY(QString cableMac READ cableMac WRITE setCableMac)
            Q_PROPERTY(QString version READ version WRITE setVersion)
            Q_PROPERTY(QString techId READ techId WRITE setTechId)
            Q_PROPERTY(QString techName READ techName WRITE setTechName)
            Q_PROPERTY(QString company READ company WRITE setCompany)

        public:
            QMsgRegisterTarget(QObject *parent = nullptr);
            QMsgRegisterTarget(const QMsgRegisterTarget &message);
            virtual ~QMsgRegisterTarget();

            QString techId() const;
            void setTechId(QString techId);

            QString techName() const;
            void setTechName(QString techName);

            QString company() const;
            void setCompany(QString company);

            QString version() const;
            void setVersion(QString version);

            QString serialNumber() const;
            void setSerialNumber(QString serialNumber);

            QString mac() const;
            void setMac(QString mac);

            QString cableMac() const;
            void setCableMac(QString cableMac);

            quint32 deviceType() const;
            void setDeviceType(quint32 deviceType);

            void getMac(char *mac) const;
            void setMacFromChar(char *mac, int length);
            void getCableMac(char *mac) const;
            void setCableMacFromChar(char *mac, int length);

        protected:
            virtual QDataStream &read(QDataStream &in) ;
            virtual QDataStream &write(QDataStream &out) const;
            virtual const QByteArray toByteArray() const;
            virtual void initAttributes();

        private:
            QSharedPointer<QProfile::QStringAttribute> 		m_version;
            QSharedPointer<QProfile::QStringAttribute> 		m_serialNumber;
            QSharedPointer<QProfile::QMacAttribute> 		m_mac;
            QSharedPointer<QProfile::QMacAttribute> 		m_cableMac;
            QSharedPointer<QProfile::QNumericAttribute> 	m_deviceType;
            QSharedPointer<QProfile::QStringAttribute> 		m_techId;
            QSharedPointer<QProfile::QStringAttribute> 		m_techName;
            QSharedPointer<QProfile::QStringAttribute> 		m_company;
    };
}

Q_DECLARE_METATYPE(QRserver::QMsgRegisterTarget)

#endif
