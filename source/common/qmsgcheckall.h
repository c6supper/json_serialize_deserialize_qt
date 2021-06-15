#ifndef QMSGCHECKALL_H
#define QMSGCHECKALL_H

#include <QMetaType>
#include <QSharedPointer>
#include <QDateTime>
#include "qabstractbinmsg.h"
#include "qprofile_debug.h"
#include "qprofile_global.h"
#include "qstringattribute.h"
#include "qmacattribute.h"
#include "qregistermodel.h"
#include "qnumericattribute.h"
#include "qdatetimeattribute.h"

namespace QRserver
{
    class QMsgCheckAll : public QAbstractBinMsg
    {
            Q_OBJECT

            //RegisterStatus
            Q_PROPERTY(qint8 status READ status WRITE setStatus)

            //st_SyncServerInfo
            Q_PROPERTY(QString serverName READ serverName WRITE setServerName)
            Q_PROPERTY(QString serverLocation READ serverLocation WRITE setServerLocation)
            Q_PROPERTY(QDateTime serverDateTime READ serverDateTime WRITE setServerDateTime)

            //st_RserverDeviceInfoExt
            Q_PROPERTY(quint32 deviceType READ deviceType WRITE setDeviceType)
            Q_PROPERTY(QString mac READ mac WRITE setMac)
            Q_PROPERTY(QString cableMac READ cableMac WRITE setCableMac)
            Q_PROPERTY(QString version READ version WRITE setVersion)
            Q_PROPERTY(QString techId READ techId WRITE setTechId)
            Q_PROPERTY(QString techName READ techName WRITE setTechName)
            Q_PROPERTY(QString company READ company WRITE setCompany)
            Q_PROPERTY(QString supervisorId READ supervisorId WRITE setSupervisorId)

            //st_AdminSetting
            Q_PROPERTY(quint32 adminStatus READ adminStatus WRITE setAdminStatus)
            Q_PROPERTY(quint32 lockDown READ lockDown WRITE setLockDown)
            Q_PROPERTY(quint32 timeLock READ timeLock WRITE setTimeLock)
            Q_PROPERTY(quint32 timeBomb READ timeBomb WRITE setTimeBomb)
            Q_PROPERTY(QString timeBombMessage READ timeBombMessage WRITE
                       setTimeBombMessage)
            Q_PROPERTY(quint32 timeBombDays READ timeBombDays WRITE setTimeBombDays)

            Q_PROPERTY(QDateTime expiredDateTime READ expiredDateTime)

            //LockRegistration
            Q_PROPERTY(quint32 lockRegistration READ lockRegistration WRITE
                       setLockRegistration)

            //st_CurrentVerison
            Q_PROPERTY(QString remoteVersion READ remoteVersion WRITE setRemoteVersion)

            //st_OrgChartStatus
            Q_PROPERTY(quint32 orgChartStatus READ orgChartStatus WRITE setOrgChartStatus)

            //st_OrgChartInfo
            Q_PROPERTY(QString region READ region WRITE setRegion)
            Q_PROPERTY(QString district READ district WRITE setDistrict)
            Q_PROPERTY(QString system READ system WRITE setSystem)

        public:
            QMsgCheckAll(QObject *parent = nullptr);
            QMsgCheckAll(const QMsgCheckAll &message);
            virtual ~QMsgCheckAll();

            //
            qint8 status() const;
            void setStatus(qint8 status);

            //
            QString serverName() const;
            void setServerName(QString serverName);
            QString serverLocation() const;
            void setServerLocation(QString serverLocation);
            QDateTime serverDateTime() const;
            void setServerDateTime(QDateTime dateTime);

            //
            quint32 deviceType() const;
            void setDeviceType(quint32 deviceType);

            QString mac() const;
            void setMac(QString mac);

            QString cableMac() const;
            void setCableMac(QString cableMac);

            QString version() const;
            void setVersion(QString version);

            QString techId() const;
            void setTechId(QString techId);

            QString techName() const;
            void setTechName(QString techName);

            QString company() const;
            void setCompany(QString company);

            QString supervisorId() const;
            void setSupervisorId(QString supervisorId);

            //
            quint32 adminStatus() const;
            void setAdminStatus(quint32 adminStatus);

            quint32 lockDown() const;
            void setLockDown(quint32 lockDown);

            quint32 timeLock() const;
            void setTimeLock(quint32 timeLock);

            quint32 timeBomb() const;
            void setTimeBomb(quint32 timeBomb);

            QString timeBombMessage() const;
            void setTimeBombMessage(QString timeBombMessage);

            quint32 timeBombDays() const;
            void setTimeBombDays(quint32 timeBombDays);

            QDateTime expiredDateTime() const;

            //
            quint32 lockRegistration() const;
            void setLockRegistration(quint32 lockRegistration);

            //
            QString remoteVersion() const;
            void setRemoteVersion(QString remoteVersion);

            //
            quint32 orgChartStatus() const;
            void setOrgChartStatus(quint32 status);

            //
            QString region() const;
            void setRegion(QString region);
            QString district() const;
            void setDistrict(QString district);
            QString system() const;
            void setSystem(QString system);

        protected:
            virtual QDataStream &read(QDataStream &in) ;
            virtual QDataStream &write(QDataStream &out) const;
            virtual const QByteArray toByteArray() const;
            virtual void initAttributes();

            void getMac(char *mac) const;
            void setMacFromChar(char *mac, int length);
            void getCableMac(char *mac) const;
            void setCableMacFromChar(char *mac, int length);

        private:
            //
            qint8 m_registerStatus;

            //
            QSharedPointer<QProfile::QStringAttribute> m_serverName;
            QSharedPointer<QProfile::QStringAttribute> m_serverLocation;
            QSharedPointer<QProfile::QDateTimeAttribute> m_serverDateTime;

            //
            QSharedPointer<QProfile::QNumericAttribute> m_deviceType;
            QSharedPointer<QProfile::QMacAttribute> m_mac;
            QSharedPointer<QProfile::QMacAttribute> m_cableMac;
            QSharedPointer<QProfile::QStringAttribute> m_version;
            QSharedPointer<QProfile::QStringAttribute> m_techId;
            QSharedPointer<QProfile::QStringAttribute> m_techName;
            QSharedPointer<QProfile::QStringAttribute> m_company;
            QSharedPointer<QProfile::QStringAttribute> m_supervisorId;

            //
            QSharedPointer<QProfile::QNumericAttribute> m_adminStatus;
            QSharedPointer<QProfile::QNumericAttribute> m_lockDown;
            QSharedPointer<QProfile::QNumericAttribute> m_timeLock;
            QSharedPointer<QProfile::QNumericAttribute> m_timeBomb;
            QSharedPointer<QProfile::QStringAttribute> m_timeBombMessage;
            QSharedPointer<QProfile::QNumericAttribute> m_timeBombDays;

            //
            QSharedPointer<QProfile::QNumericAttribute> m_lockRegistration;

            //
            QSharedPointer<QProfile::QStringAttribute> m_remoteVersion;

            //
            QSharedPointer<QProfile::QNumericAttribute> m_orgChartStatus;

            //
            QSharedPointer<QProfile::QStringAttribute> m_region;
            QSharedPointer<QProfile::QStringAttribute> m_district;
            QSharedPointer<QProfile::QStringAttribute> m_system;
    };
}

Q_DECLARE_METATYPE(QRserver::QMsgCheckAll)

#endif /* QMSGCHECKALL_H */
