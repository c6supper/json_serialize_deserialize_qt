#ifndef QDOWNLOADPROCESS_H
#define QDOWNLOADPROCESS_H

#include <QProcess>
#include <QTimer>
#include <QElapsedTimer>
#include "qprofile_global.h"
#include "qprofile_debug.h"
#include "qabstractjsonelement.h"

namespace QRserver
{
    class QDownloadProcess: public QProfile::QAbstractJsonElement
    {
            Q_OBJECT

            Q_ENUMS(DownloadStatus)
            Q_PROPERTY(qint64 totalSize READ totalSize WRITE setTotalSize)
            Q_PROPERTY(qint64 downloadedSize READ downloadedSize WRITE setDownloadedSize)
            Q_PROPERTY(QString downloadedPercentage READ downloadedPercentage WRITE
                       setDownloadedPercentage)
            Q_PROPERTY(QString downloadSpeed READ downloadSpeed WRITE setDownloadSpeed)
            Q_PROPERTY(QString downloadElapsed READ downloadElapsed WRITE
                       setDownloadElapsed)
            Q_PROPERTY(QString downloadEta READ downloadEta WRITE setDownloadEta)
            Q_PROPERTY(DownloadStatus downloadStatus READ downloadStatus WRITE
                       setDownloadStatus)

        public:
            enum DownloadStatus {
                Idle = 0xFF,
                SaveDirectoryNotExists,
                NotEnoughSpace,
                StartFailed,
                RemoteFileNotExists,
                Downloading,
                Timeout,
                CheckingMD5,
                MD5Mismatch,
                Failed,
                Succeed,
            };
            static const quint32 DownloadTimeout;
            static const QString UsbDir;
            static const QString WgetCommand;

        public:
            QDownloadProcess(QObject *parent = nullptr);
            ~QDownloadProcess();
            virtual void pureVirtualMock() {};
            /* fileSize(M)=-1: do not check free space
            md5 is empty: do not check md5. */
            virtual bool wget(const QString url, const QString saveDirectory,
                              const QString fileName, qint64 fileSize = -1, const QString md5 = QString());
            bool stop();

            const qint64 totalSize() const;
            void setTotalSize(const qint64 totalSize);

            const qint64 downloadedSize() const;
            void setDownloadedSize(const qint64 downloadedSize);

            const QString downloadedPercentage() const;
            void setDownloadedPercentage(const QString downloadedPercentage);

            const QString downloadSpeed() const;
            void setDownloadSpeed(const QString downloadSpeed);

            const QString downloadElapsed();
            void setDownloadElapsed(const QString downloadElapsed);

            const QString downloadEta() const;
            void setDownloadEta(const QString downloadEta);

            const DownloadStatus downloadStatus() const;
            void setDownloadStatus(const DownloadStatus downloadStatus);

            static QByteArray GetMD5(const QString &filePath);

        signals:
            //void dataTransferProgress(const QString percentage);
            //void downloadStatusChanged(int status);

        protected:
            void initial();
            bool isRemoteFileExists(const QString message);
            qint64 getFreeSpace(const QString &dir);
            bool isDirExists(const QString &dir);
            bool matchMD5(const QString filePath, const QString md5);

        protected slots:
            virtual void checkFinished(int exitCode, QProcess::ExitStatus exitStatus);
        private slots:
            void checkOutput();
            void onTimeout();

        protected:
            QTimer m_timer;
            QProcess m_wgetProcess;
            QString m_filePath;
            QString m_md5;
            QElapsedTimer m_ElapsedTimer;
            //
            qint64 m_totalSize;
            qint64 m_downloadedSize;
            QString m_downloadedPercentage;
            QString m_downloadSpeed;
            QString m_downloadElapsed;
            QString m_downloadEta;
            DownloadStatus m_downloadStatus;
    };
}

#endif /* QDOWNLOADPROCESS_H */

