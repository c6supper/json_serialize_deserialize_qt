
#ifndef QABSTRACTADMINSETTINGPROFILE_H
#define QABSTRACTADMINSETTINGPROFILE_H

#include <QDebug>
#include <QObject>
#include <QDateTime>
#include <QTimer>
#include <QStringList>
#if defined(BUILD_X86)
#include <QCoreApplication>
#include "qjson/serializer.h"
#include "qjson/parser.h"
#include "qjson/qobjecthelper.h"
#else
#include <QJson/Serializer>
#include <QJson/Parser>
#include <QJson/QObjectHelper>
#endif

namespace QRserver
{
    class QAbstractAdminSettingProfile : public QObject
    {
            Q_OBJECT

            Q_PROPERTY(qint32 status READ status WRITE setStatus)

            //server date time.
            Q_PROPERTY(QDateTime serverDateTime READ serverDateTime WRITE setServerDateTime)
            Q_PROPERTY(QDateTime expiredDateTime READ expiredDateTime WRITE
                       setExpiredDateTime)

            //st_AdminSetting
            Q_PROPERTY(quint32 adminStatus READ adminStatus WRITE setAdminStatus)
            Q_PROPERTY(quint32 lockDown READ lockDown WRITE setLockDown)
            Q_PROPERTY(quint32 timeLock READ timeLock WRITE setTimeLock)
            Q_PROPERTY(quint32 timeBomb READ timeBomb WRITE setTimeBomb)
            Q_PROPERTY(QString timeBombMessage READ timeBombMessage WRITE
                       setTimeBombMessage)
            Q_PROPERTY(quint32 lockRegistration READ lockRegistration WRITE
                       setLockRegistration)

            Q_PROPERTY(bool registrationBomb READ registrationBomb WRITE
                       setRegistrationBomb)
            Q_PROPERTY(bool lockTechId READ lockTechId WRITE setLockTechId)
            Q_PROPERTY(bool lockVeexpressUpgrade READ lockVeexpressUpgrade WRITE
                       setLockVeexpressUpgrade)
            Q_PROPERTY(bool blockProfileUpload READ blockProfileUpload WRITE
                       setBlockProfileUpload)
            Q_PROPERTY(bool changeTechName READ changeTechName WRITE setChangeTechName)
            Q_PROPERTY(bool maintenance READ maintenance WRITE setMaintenance)

        public:
            QAbstractAdminSettingProfile(QObject *parent): QObject(parent)
            {
                m_registerStatus = 0;
                m_serverDateTime = QDateTime();
                m_expiredDateTime = QDateTime();
                m_adminStatus = 0;
                m_lockDown = 0;
                m_timeLock = 0;
                m_timeBomb = 0;
                m_timeBombMessage =
                    QObject::tr("Your test set have been locked by Time Bomb,\n"
                                "Please contact the R-Server administrator!");
                m_lockRegistration = 0;
                m_registrationBomb = false;
                m_lockTechId = false;
                m_lockVeexpressUpgrade = false;
                m_blockProfileUpload = false;
                m_changeTechName = false;
                m_maintenance = false;
                m_isNotify = true;
                //property changed handler
                m_changeNotifyTimer.setSingleShot(true);
                m_changeNotifyTimer.setInterval(propertyChangedNotifyInterval);
                connect(&m_changeNotifyTimer, SIGNAL(timeout()), this,
                        SIGNAL(propertyChanged()));
            }

            const QByteArray toJson() const
            {
                bool ok = false;
                QJson::Serializer serializer;
                QVariantMap variantMap = QJson::QObjectHelper::qobject2qvariant(this,
                                         ingroedPropertiesList());
                QByteArray json = serializer.serialize(variantMap, &ok);
                return json;
            }

            const QVariant toVariant() const
            {
                QJson::Serializer serializer;
                return QVariant(QJson::QObjectHelper::qobject2qvariant(this,
                                ingroedPropertiesList()));
            }

            virtual void fromVariant(const QVariant &variant)
            {
                QJson::QObjectHelper::qvariant2qobject(variant.toMap(), this);
            }

            virtual quint32 timeLock() const
            {
                return m_timeLock;
            }

            virtual QString timeBombMessage() const
            {
                return m_timeBombMessage;
            }

            virtual QDateTime expiredDateTime() const
            {
                return m_expiredDateTime;
            }

            void setExpiredDateTime(QDateTime dateTime)
            {
                if (m_expiredDateTime == dateTime)
                    return;

                m_expiredDateTime = dateTime;
                propertyChangeNotify();
            }

            quint32 adminStatus() const
            {
                return m_adminStatus;
            }

            void setAdminStatus(quint32 adminStatus)
            {
                if (m_adminStatus == adminStatus)
                    return;

                m_adminStatus = adminStatus;
                propertyChangeNotify();
            }

            void setTimeLock(quint32 timeLock)
            {
                if (m_timeLock == timeLock)
                    return;

                m_timeLock = timeLock;
                propertyChangeNotify();
            }

            virtual quint32 timeBomb() const
            {
                return m_timeBomb;
            }

            void setTimeBomb(quint32 timeBomb)
            {
                if (m_timeBomb == timeBomb)
                    return;

                m_timeBomb = timeBomb;
                propertyChangeNotify();
            }

            virtual QDateTime serverDateTime() const
            {
                return m_serverDateTime;
            }

            virtual quint32 lockRegistration() const
            {
                return m_lockRegistration;
            }

            void setLockTechId(bool lockTechId)
            {
                if (m_lockTechId == lockTechId)
                    return;

                m_lockTechId = lockTechId;
                propertyChangeNotify();
            }

            void setRegistrationBomb(bool registrationBomb)
            {
                if (m_registrationBomb == registrationBomb)
                    return;

                m_registrationBomb = registrationBomb;
                propertyChangeNotify();
            }

            void setStatus(qint32 registerStatus)
            {
                if (m_registerStatus == registerStatus) {
                    return;
                }

                m_registerStatus = registerStatus;
                propertyChangeNotify();
            }

            void setLockDown(quint32 lockDown)
            {
                if (m_lockDown == lockDown)
                    return;

                m_lockDown = lockDown;
                propertyChangeNotify();
            }

            void setTimeBombMessage(QString timeBombMessage)
            {
                if (m_timeBombMessage == timeBombMessage)
                    return;

                m_timeBombMessage = timeBombMessage;
                propertyChangeNotify();
            }

            void setLockRegistration(quint32 lockRegistration)
            {
                if (m_lockRegistration == lockRegistration)
                    return;

                m_lockRegistration = lockRegistration;
                propertyChangeNotify();
            }

            void setLockVeexpressUpgrade(bool lockVeexpressUpgrade)
            {
                if (m_lockVeexpressUpgrade == lockVeexpressUpgrade)
                    return;

                m_lockVeexpressUpgrade = lockVeexpressUpgrade;
                propertyChangeNotify();
            }

            void setBlockProfileUpload(bool blockProfileUpload)
            {
                if (m_blockProfileUpload == blockProfileUpload)
                    return;

                m_blockProfileUpload = blockProfileUpload;
                propertyChangeNotify();
            }

            void setChangeTechName(bool changeTechName)
            {
                if (m_changeTechName == changeTechName)
                    return;

                m_changeTechName = changeTechName;
                propertyChangeNotify();
            }

            void setMaintenance(bool maintenance)
            {
                if (m_maintenance == maintenance)
                    return;

                m_maintenance = maintenance;
                propertyChangeNotify();
            }

            static QDateTime qCharToDateTime(const unsigned char *expiredDatetime)
            {
                struct tm  tmDateTime = {0};
                tmDateTime.tm_sec  = expiredDatetime[5];
                tmDateTime.tm_min  = expiredDatetime[4];
                tmDateTime.tm_hour = expiredDatetime[3];
                tmDateTime.tm_mday = expiredDatetime[2];
                tmDateTime.tm_mon  = expiredDatetime[1];
                tmDateTime.tm_year = expiredDatetime[0];
                tmDateTime.tm_wday  = 0;
                tmDateTime.tm_yday  = 0;
                tmDateTime.tm_isdst = 0;
                time_t time_new = timelocal((struct tm *)&tmDateTime);
                return QDateTime::fromTime_t(time_new);
            }

        protected:
            //
            qint32 status() const
            {
                //QDeviceInfo RegisteredAuthorized
                return m_registerStatus;
            }

            virtual bool lockTechId() const
            {
                //return (registrationBomb() && m_lockTechId);
                return m_lockTechId;
            }

            virtual bool registrationBomb() const
            {
                //return (timeLock() && isRegistered() && m_registrationBomb);
                return m_registrationBomb;
            }

            void setServerDateTime(QDateTime dateTime)
            {
                if (m_serverDateTime == dateTime)
                    return;

                m_serverDateTime = dateTime;
                propertyChangeNotify();
            }

            virtual quint32 lockDown() const
            {
                return m_lockDown;
            }

            virtual bool lockVeexpressUpgrade() const
            {
                return m_lockVeexpressUpgrade;
            }

            virtual bool blockProfileUpload() const
            {
                return m_blockProfileUpload;
            }

            virtual bool changeTechName() const
            {
                return m_changeTechName;
            }

            virtual bool maintenance() const
            {
                return m_maintenance;
            }

            virtual QStringList ingroedPropertiesList() const
            {
                return QStringList() << "objectName";
            }

            static void qDateTimeToChar(const QDateTime &dateTime,
                                        unsigned char *expiredDatetime)
            {
                time_t timeT = (time_t)dateTime.toTime_t();
                struct tm *timeTM = localtime(&timeT);
                expiredDatetime[5] = timeTM->tm_sec;
                expiredDatetime[4] = timeTM->tm_min;
                expiredDatetime[3] = timeTM->tm_hour;
                expiredDatetime[2] = timeTM->tm_mday;
                expiredDatetime[1] = timeTM->tm_mon;
                expiredDatetime[0] = timeTM->tm_year;
            }

            bool blockNotify()
            {
                m_changeNotifyTimer.stop();
                bool ret = m_isNotify;
                m_isNotify = false;
                return ret;
            }

            bool unblockNotify()
            {
                bool ret = m_isNotify;
                m_isNotify = true;
                return ret;
            }

        signals:
            void propertyChanged();

        protected slots:
            void propertyChangeNotify()
            {
                if (m_isNotify) {
                    m_changeNotifyTimer.start();
                }
            }

        private:
            virtual void pureVirtualMock() = 0;

        public:
            static const quint32 RegistrationLockDays = 1;
        private:
            qint32 m_registerStatus;
            QDateTime m_serverDateTime;
            QDateTime m_expiredDateTime;
            quint32 m_adminStatus;
            quint32 m_lockDown;
            quint32 m_timeLock;
            quint32 m_timeBomb;
            quint32 m_lockRegistration;
            QString m_timeBombMessage;
            bool m_registrationBomb;
            bool m_lockTechId;
            bool m_lockVeexpressUpgrade;
            bool m_blockProfileUpload;
            bool m_changeTechName;
            bool m_maintenance;
            QTimer m_changeNotifyTimer;
            bool m_isNotify;
            static const qint32 propertyChangedNotifyInterval = 100;
    };
}

#endif /* QABSTRACTADMINSETTINGPROFILE_H */
