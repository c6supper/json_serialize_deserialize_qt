
#ifndef QADMINSETTINGPROFILE_H
#define QADMINSETTINGPROFILE_H

#include <QFileSystemWatcher>
#include <QDataStream>
#include <QCryptographicHash>
#include <QMetaProperty>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include "qabstractadminsettingprofile.h"

#define reserverLog(message)       \
    do                          \
    {                           \
        QRserver::QAdminSettingProfile::logRserverMessage(__FILE__, __LINE__ ,__PRETTY_FUNCTION__,message);           \
    }while(0)

namespace QRserver
{
    class QAdminSettingProfile : public QAbstractAdminSettingProfile
    {
            Q_OBJECT

            Q_PROPERTY(QDateTime registrationExpiredDateTime READ
                       registrationExpiredDateTime WRITE setRegistrationExpiredDateTime)

        public:
            QAdminSettingProfile(QObject *parent)
                : QAbstractAdminSettingProfile(parent)
            {
                m_registrationExpiredDateTime = QDateTime();
                connect(this, SIGNAL(propertyChanged()), this, SLOT(onPropertyChanged()));

                //file watcher
                if (QFile::exists(adminSettingProfilePath())) {
                    m_fileSystemWatcher.addPath(adminSettingProfilePath());
                }

                connect(&m_fileSystemWatcher, SIGNAL(fileChanged(const QString)), this,
                        SLOT(onFileChanged(const QString)));
                //check Profile Exists
                m_checkProfileExistsTimer.setSingleShot(false);
                m_checkProfileExistsTimer.setInterval(CheckProfileExistsInterval);
                connect(&m_checkProfileExistsTimer, SIGNAL(timeout()), this,
                        SLOT(onCheckProfileExists()));
                m_checkProfileExistsTimer.start();
                //check timebomb
                m_timeBombTimer.setSingleShot(false);
                m_timeBombTimer.setInterval(CheckTimeBombInterval);
                connect(&m_timeBombTimer, SIGNAL(timeout()), this, SLOT(onCheckTimeBomb()));
                m_timeBombTimer.start();
            }

            QAdminSettingProfile(const QVariant &otherVar)
                : QAbstractAdminSettingProfile(NULL)
            {
                QAbstractAdminSettingProfile::fromVariant(otherVar);
            }

            ~QAdminSettingProfile()
            {
            }

            static const qint32 CheckTimeBombInterval = 3600 * 1000;
            static const qint32 CheckProfileExistsInterval = 5 * 1000;

            // could only be called when system init or profile is missing
            void loadProfile()
            {
                if (QFile::exists(adminSettingProfilePath())) {
                    blockNotify();
                    fromFile();
                    //set server datetime to invalid.
                    setServerDateTime(QDateTime());
                    unblockNotify();
                }
            }

            bool isTimeBombExpired()
            {
                bool bExpired = false;
                QDateTime expiredDT = expiredDateTime();

                if (expiredDT.isValid()) {
                    if (QDateTime::currentDateTime() > expiredDT) {
                        bExpired = true;
                    }
                }

                return bExpired;
            }

            bool isRegistered() const
            {
                //QDeviceInfo RegisteredAuthorized
                return (status() == 1);
            }

            virtual quint32 timeLock() const
            {
                return (adminStatus() && QAbstractAdminSettingProfile::timeLock());
            }

            virtual QString timeBombMessage() const
            {
                return (timeBomb() ? QAbstractAdminSettingProfile::timeBombMessage() :
                        QString());
            }

            virtual QDateTime expiredDateTime() const
            {
                return (timeBomb() ? QAbstractAdminSettingProfile::expiredDateTime() :
                        QDateTime());
            }

            virtual quint32 timeBomb() const
            {
                return (timeLock() && QAbstractAdminSettingProfile::timeBomb());
            }

            virtual QDateTime serverDateTime() const
            {
                return (isRegistered() ? QAbstractAdminSettingProfile::serverDateTime() :
                        QDateTime());
            }

            virtual quint32 lockRegistration() const
            {
                return (adminStatus() && QAbstractAdminSettingProfile::lockRegistration());
            }

            virtual bool lockVeexpressUpgrade() const
            {
                return (adminStatus() && QAbstractAdminSettingProfile::lockVeexpressUpgrade());
            }

            virtual bool blockProfileUpload() const
            {
                return (adminStatus() && QAbstractAdminSettingProfile::blockProfileUpload());
            }

            virtual bool changeTechName() const
            {
                return (adminStatus() && QAbstractAdminSettingProfile::changeTechName());
            }

            bool isLockTechId() const
            {
                return lockTechId();
            }

            bool isRegistrationBomb() const
            {
                return registrationBomb();
            }

            bool isRegistrationBombExpired()
            {
                bool bExpired = false;
                QDateTime expiredDT = registrationExpiredDateTime();

                if (expiredDT.isValid() && isRegistrationBomb()) {
                    if (QDateTime::currentDateTime() > expiredDT) {
                        bExpired = true;
                    }
                }

                return bExpired;
            }

            bool isLockDown() const
            {
                return (!isMaintenance() && (lockDown() != 0));
            }

            bool isLockVeexpressUpgrade()
            {
                return lockVeexpressUpgrade();
            }

            bool isBlockProfileUpload() const
            {
                return blockProfileUpload();
            }

            bool isTechNameCanBeChanged() const
            {
                return changeTechName();
            }

            bool isMaintenance() const
            {
                return maintenance();
            }

            static QAdminSettingProfile *Instance()
            {
                static QAdminSettingProfile instance(NULL);
                return &instance;
            }

            bool operator==(const QAdminSettingProfile &other) const
            {
                for (int propertyIdx = 0; propertyIdx < metaObject()->propertyCount();
                     propertyIdx++) {
                    QMetaProperty metaProperty = metaObject()->property(propertyIdx);
                    int dstIdx = other.metaObject()->indexOfProperty(metaProperty.name());

                    if (dstIdx >= 0) {
                        if (property(metaProperty.name()) != other.property(metaProperty.name())) {
                            return false;
                        }
                    } else {
                        return false;
                    }
                }

                return true;
            }

            virtual void fromVariant(const QVariant &variant)
            {
                QAdminSettingProfile other(variant);
                QDateTime registrationExpiredDateTime = m_registrationExpiredDateTime;

                if (!other.isRegistrationBomb()) {
                    registrationExpiredDateTime = QDateTime();
                } else if (!other.serverDateTime().isValid()) {
                    registrationExpiredDateTime = QDateTime();
                    reserverLog(QString("Invalid Server Date time. %1").arg(
                                    QString(other.toJson())));
                } else if (!isRegistrationBomb() || !m_registrationExpiredDateTime.isValid()) {
                    registrationExpiredDateTime = other.serverDateTime().addDays(1);
                }

                qDebug() << "fromVariant:" << variant << other.toVariant();
                QAbstractAdminSettingProfile::fromVariant(other.toVariant());
                m_registrationExpiredDateTime = registrationExpiredDateTime;
            }

            void setRegistrationExpiredDateTime(const QDateTime dateTime)
            {
                if (m_registrationExpiredDateTime == dateTime)
                    return;

                m_registrationExpiredDateTime = dateTime;
                propertyChangeNotify();
            }

            static void logRserverMessage(const QString &file, const qint32 line,
                                          const QString &func, const QString &message)
            {
                QFile profile("/tmp/rserver.log");
                bool ret = false;

                if (profile.size() < 1024 * 16)
                    ret = profile.open(QIODevice::ReadWrite | QIODevice::Append);
                else
                    ret = profile.open(QIODevice::ReadWrite | QIODevice::Truncate);

                if (ret) {
                    QTextStream out(&profile);
                    out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ") << file <<
                        ":" << QString("%1").arg(line) << ":" << func << ":" << message << endl;
                    profile.close();
                }
            }

        protected:
            virtual bool lockTechId() const
            {
                return (registrationBomb() && QAbstractAdminSettingProfile::lockTechId());
            }

            virtual bool registrationBomb() const
            {
                return (timeLock() && isRegistered()
                        && QAbstractAdminSettingProfile::registrationBomb());
            }

            virtual quint32 lockDown() const
            {
                return (adminStatus() && QAbstractAdminSettingProfile::lockDown());
            }

            virtual bool isProfileChanged()
            {
                QAdminSettingProfile other(NULL);

                if (other.fromFile()) {
                    return !(*this == other);
                } else {
                    reserverLog("Profile load error.");
                }

                return true;
            }

            QDateTime registrationExpiredDateTime() const
            {
                return (registrationBomb() ? m_registrationExpiredDateTime : QDateTime());
            }

            bool fromFile()
            {
                QFile file(adminSettingProfilePath());

                if (file.exists()) {
                    if (file.open(QIODevice::ReadOnly)) {
                        QByteArray data = file.readAll();
                        QJson::Parser parser;
                        bool ok = false;
                        QVariant variant = parser.parse(data, &ok);

                        if (!ok) {
                            qDebug() << "Failed to load " << adminSettingProfilePath();
                            return false;
                        }

                        QAbstractAdminSettingProfile::fromVariant(variant);
                        file.close();
                        return true;
                    }
                }

                qDebug() << "File missing, file= " << adminSettingProfilePath();
                return false;
            }

            bool toFile(const QString &pathName)
            {
                if (!isProfileChanged()) {
                    return true;
                }

                //profile dir
                QFileInfo profileInfo(pathName);
                QDir profileDir = profileInfo.absoluteDir();

                if (!profileDir.exists()) {
                    if (!profileDir.mkpath(profileDir.absolutePath())) {
                        qDebug() << "mkpath failed, path = " << profileDir.absolutePath();
                        return false;
                    }
                }

                QIODevice::OpenMode openMode = QIODevice::WriteOnly;
                QFile file(pathName);

                if (file.exists()) {
                    openMode |= QIODevice::Truncate;
                }

                if (!file.open(openMode)) {
                    qDebug() << "Failed to open file:" << pathName;
                    return false;
                }

                QByteArray array = toJson();

                if (array.size() != file.write(array)) {
                    qDebug() << "Failed to write file:"  << pathName;
                    file.remove();
                    return false;
                }

                file.close();
                return true;
            }

            static const QString adminSettingProfilePath()
            {
#if defined(BUILD_X86)
                return QString("%1/home/rserver/adminsetting.json").arg(
                           QCoreApplication::applicationDirPath());
#else
                return  QString("/home/rserver/adminsetting.json");
#endif
            }

            virtual void postProfileRemoved()
            {
                if (QFile::exists(adminSettingProfilePath())) {
                    fromFile();
                }
            }

        signals:
            void timeBombExpired(bool bExpired, QString message);
            void lockDownStatus(bool bLocked);

        protected slots:
            void onCheckProfileExists()
            {
                if (!m_fileSystemWatcher.files().contains(adminSettingProfilePath())) {
                    // gui: saveFile and addpath
                    // rserver: loadFile and addpath
                    postProfileRemoved();

                    if (QFile::exists(adminSettingProfilePath())) {
                        m_fileSystemWatcher.addPath(adminSettingProfilePath());
                    }
                }
            }

            virtual void onPropertyChanged()
            {
                toFile(adminSettingProfilePath());
            }

            void onCheckTimeBomb()
            {
                emit timeBombExpired(isTimeBombExpired(), timeBombMessage());
            }

            void onCheckLockDown()
            {
                emit lockDownStatus(isLockDown());
            }

        private slots:
            virtual void onFileChanged(const QString &path)
            {
                //only rserverui could change this profile
                //                reserverLog(path + " Fatal:profile must not changed by others.");
            }

        private:
            virtual void pureVirtualMock() {}

        private:
            QTimer m_timeBombTimer;
            QTimer m_checkProfileExistsTimer;
            QFileSystemWatcher m_fileSystemWatcher;
            QDateTime m_registrationExpiredDateTime;
    };
}

#endif /* QADMINSETTINGMSG_H */
