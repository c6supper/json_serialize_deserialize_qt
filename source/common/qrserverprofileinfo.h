#ifndef QSERVERPROFILEINFO_H
#define QSERVERPROFILEINFO_H

#include "qabstractjsonelement.h"

#include <QCryptographicHash>
#include <QFileInfo>
#include <QDateTime>

namespace QRserver
{
    class QRserverProfileInfo : public QProfile::QAbstractJsonElement
    {
            Q_OBJECT

            Q_PROPERTY(QString pathName READ pathName WRITE setPathName)
            Q_PROPERTY(QDateTime downloadTime READ downloadTime WRITE setDownloadTime)
            Q_PROPERTY(qint64 fileSize READ fileSize WRITE setFileSize)
            Q_PROPERTY(QString md5sum READ md5sum WRITE setMd5sum)

        public:
            QRserverProfileInfo(QObject *parent = nullptr);
            ~QRserverProfileInfo();
            virtual void pureVirtualMock() {};
            void setProfileInfo(const QString &pathName);

            QString pathName() const;
            void setPathName(QString pathName);
            QDateTime downloadTime() const;
            void setDownloadTime(QDateTime dateTime);
            quint32 fileSize() const;
            void setFileSize(quint32 fileSize);
            QString md5sum() const;
            void setMd5sum(QString md5sum);

        private:
            static QString fileChecksum(const QString &fileName,
                                        QCryptographicHash::Algorithm hashAlgorithm);

            QString m_pathName;
            QDateTime m_downloadTime;
            qint64 m_fileSize;
            QString m_md5sum;
    };
}

#endif // QSERVERPROFILEINFO_H
