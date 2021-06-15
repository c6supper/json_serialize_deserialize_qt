
#ifndef QBINARYADMINSETTINGPROFILE_H
#define QBINARYADMINSETTINGPROFILE_H

#include "qadminsettingprofile.h"

#define MD5_LEN (16)

#pragma pack(1)

/* total: 256bytes, reserved = 256 - bytes of others,
if add new byte, must insert before reserved, and reserved will be reduce */
typedef struct st_AdminSettingE2prom {
    char enable; /* 1byte */
    char lockDownEnable; /* 1byte */
    char timeLockEnable; /* 1byte */
    char timeBombEnable; /* 1byte */
    char lockRegistration; /* 1byte */
    /* year/month/day/hour/minute/second.--struct tm */
    unsigned char expiredDatetime[6];  /* 6byte */
    unsigned char registrationExpiredDatetime[6];  /* 6byte */
    int registerStatus; /* 4byte */
    char lockVeexpressUpgradeEnable; /* 1byte */
    char blockProfileUploadEnable; /* 1byte */
    char changeTechNameEnable; /* 1byte */
    char maintenance; /* 1byte */
    char reserved[215];
    unsigned char md5[MD5_LEN]; /* 16byte(fixed) */
} st_AdminSettingE2prom;

#define ADMINSETTING_LEN (sizeof(st_AdminSettingE2prom))

#pragma pack()

namespace QRserver
{
    class QBinaryAdminSettingProfile: public QAdminSettingProfile
    {
            Q_OBJECT

        public:
            QBinaryAdminSettingProfile(QObject *parent)
                : QAdminSettingProfile(parent)
            {
            }

            ~QBinaryAdminSettingProfile()
            {
            }

            void loadBinarySetting(char *setting)
            {
                //make sure the properties which is not in the eeprom could loaded from file
                loadProfile();

                if (setting != NULL) {
                    //binmsg to class
                    QByteArray messageArray(setting, sizeof(st_AdminSettingE2prom));
                    QDataStream in(messageArray);
                    blockNotify();
                    in >> *this;
                    toFile(QAdminSettingProfile::adminSettingProfilePath());
                    unblockNotify();
                }

                QTimer::singleShot(0, this, SLOT(onCheckProfileExists()));
                QTimer::singleShot(0, this, SLOT(onCheckTimeBomb()));
                QTimer::singleShot(0, this, SLOT(onCheckLockDown()));
            }

            const QByteArray toByteArray() const
            {
                st_AdminSettingE2prom stMessage;
                bzero(&stMessage, sizeof(st_AdminSettingE2prom));
                QByteArray messageArray((const char *)&stMessage, sizeof(stMessage));
                QDataStream out(&messageArray, QIODevice::WriteOnly);
                out.setVersion(QDataStream::Qt_4_6);
                out << *this;
                return messageArray;
            }

            static QBinaryAdminSettingProfile *Instance()
            {
                static QBinaryAdminSettingProfile instance(NULL);
                return &instance;
            }

        private:
            friend QDataStream &operator>>(QDataStream &in,
                                           QBinaryAdminSettingProfile &message)
            {
                return message.read(in);
            }
            friend QDataStream &operator<<(QDataStream &out,
                                           const QBinaryAdminSettingProfile &message)
            {
                return message.write(out);
            }
            QByteArray calcMD5(char *memory, int md5Len) const
            {
                QByteArray array((const char *)memory, md5Len);
                return QCryptographicHash::hash(array, QCryptographicHash::Md5);
            }
        protected:
            QDataStream &read(QDataStream &in)
            {
                st_AdminSettingE2prom stMessage;

                if ((qint32)sizeof(stMessage) <= in.readRawData((char *)&stMessage,
                        sizeof(stMessage))) {
                    QByteArray storeMD5((const char *)stMessage.md5, sizeof(stMessage.md5));
                    QByteArray localMD5 = calcMD5((char *)&stMessage,
                                                  sizeof(st_AdminSettingE2prom) - MD5_LEN);

                    if (storeMD5 == localMD5) {
                        setAdminStatus((quint32)stMessage.enable);
                        setLockDown((quint32)stMessage.lockDownEnable);
                        setTimeLock((quint32)stMessage.timeLockEnable);
                        setTimeBomb((quint32)stMessage.timeBombEnable);
                        setLockRegistration((quint32)stMessage.lockRegistration);
                        setExpiredDateTime(QAbstractAdminSettingProfile::qCharToDateTime(
                                               stMessage.expiredDatetime));
                        QDateTime l_dateTime = QAbstractAdminSettingProfile::qCharToDateTime(
                                                   stMessage.registrationExpiredDatetime);
                        setRegistrationBomb(l_dateTime.isValid());
                        setRegistrationExpiredDateTime(l_dateTime);
                        setStatus(stMessage.registerStatus);
                        setLockVeexpressUpgrade(stMessage.lockVeexpressUpgradeEnable != 0);
                        setBlockProfileUpload(stMessage.blockProfileUploadEnable != 0);
                        setChangeTechName(stMessage.changeTechNameEnable != 0);
                        setMaintenance(stMessage.maintenance != 0);
                    } else {
                        qDebug() << "check MD5 failed!<storeMD5/localMD5>: " <<
                                 storeMD5.toHex() << localMD5.toHex();
                    }

                    qDebug() << "QBinaryAdminSettingProfile<read>:" << toJson();
                }

                return in;
            }
            QDataStream &write(QDataStream &out) const
            {
                st_AdminSettingE2prom stMessage;
                bzero(&stMessage, sizeof(stMessage));
                QAbstractAdminSettingProfile::qDateTimeToChar(expiredDateTime(),
                        stMessage.expiredDatetime);
                QAbstractAdminSettingProfile::qDateTimeToChar(registrationExpiredDateTime(),
                        stMessage.registrationExpiredDatetime);
                stMessage.enable = (quint8)adminStatus();
                stMessage.lockDownEnable = (quint8)lockDown();
                stMessage.timeLockEnable = (quint8)timeLock();
                stMessage.timeBombEnable = (quint8)timeBomb();
                stMessage.lockRegistration = (quint8)lockRegistration();
                stMessage.registerStatus = status();
                stMessage.lockVeexpressUpgradeEnable = (lockVeexpressUpgrade() ? 1 : 0);
                stMessage.blockProfileUploadEnable = (blockProfileUpload() ? 1 : 0);
                stMessage.changeTechNameEnable = (changeTechName() ? 1 : 0);
                stMessage.maintenance = (maintenance() ? 1 : 0);
                //check md5.
                QByteArray localMD5 = calcMD5((char *)&stMessage,
                                              sizeof(st_AdminSettingE2prom) - MD5_LEN);
                memcpy(stMessage.md5, localMD5.data(), MD5_LEN);
                out.writeRawData((const char *)&stMessage, sizeof(stMessage));
                qDebug() << "QBinaryAdminSettingProfile<write>:" << toJson() <<
                         localMD5.toHex();
                return out;
            }
            virtual void postProfileRemoved()
            {
                if (!QFile::exists(QAdminSettingProfile::adminSettingProfilePath())) {
                    toFile(QAdminSettingProfile::adminSettingProfilePath());
                }
            }

        signals:
            void timeLockChanged(bool lock);

        private slots:
            virtual void onFileChanged(const QString &path)
            {
                if ((adminSettingProfilePath() == path) && isProfileChanged()) {
                    fromFile();
                }
            }

        protected slots:
            virtual void onPropertyChanged()
            {
                emit timeLockChanged(timeLock());
            }
    };
}

#endif /* QBINARYADMINSETTINGPROFILE_H */
