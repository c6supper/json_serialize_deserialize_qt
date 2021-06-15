#include "qdownloadprocess.h"
#include <QApplication>
#include <QFile>
#include <QFileInfo>
#include <QCryptographicHash>
#include <QTime>

#if defined(Q_OS_LINUX)
#include <sys/stat.h>
#include <sys/statvfs.h>
#include<unistd.h>
#include<sys/types.h>
#endif

#if defined(BUILD_X86)
#include "qjson/serializer.h"
#include "qjson/parser.h"
#include "qjson/qobjecthelper.h"
#else
#include <QJson/Serializer>
#include <QJson/Parser>
#include <QJson/QObjectHelper>
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/wait.h>
#endif

using namespace QRserver;

/*the defaule timeout of QProcess is 30000 */
const quint32 QDownloadProcess::DownloadTimeout = 60000;
const QString QDownloadProcess::UsbDir = "/home/root/usbd";

#if defined(BUILD_X86)
const QString QDownloadProcess::WgetCommand = "wget";
#else
const QString QDownloadProcess::WgetCommand = "/usr/local/bin/wget-1.20.3";
#endif

QDownloadProcess::QDownloadProcess(QObject *parent)
    : QProfile::QAbstractJsonElement(parent)
{
    m_wgetProcess.setProcessChannelMode(QProcess::MergedChannels);
    connect(&m_wgetProcess, SIGNAL(readyRead()), this, SLOT(checkOutput()));
    connect(&m_wgetProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this,
            SLOT(checkFinished(int, QProcess::ExitStatus)));
    //
    m_timer.setSingleShot(true);
    m_timer.setInterval(DownloadTimeout);
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    initial();
}

QDownloadProcess::~QDownloadProcess()
{
}

void QDownloadProcess::initial()
{
    m_totalSize = -1;
    m_downloadedSize = 0;
    m_downloadedPercentage = QString();
    m_downloadSpeed = QString();
    m_downloadEta = QString();
    m_downloadStatus = Idle;
    m_ElapsedTimer.invalidate();
}

const qint64 QDownloadProcess::totalSize() const
{
    return m_totalSize;
}
void QDownloadProcess::setTotalSize(const qint64 totalSize)
{
    m_totalSize = totalSize;
}

const qint64 QDownloadProcess::downloadedSize() const
{
    return m_downloadedSize;
}
void QDownloadProcess::setDownloadedSize(const qint64 downloadedSize)
{
    m_downloadedSize = downloadedSize;
}

const QString QDownloadProcess::downloadedPercentage() const
{
    return m_downloadedPercentage;
}
void QDownloadProcess::setDownloadedPercentage(const QString
        downloadedPercentage)
{
    m_downloadedPercentage = downloadedPercentage;
}

const QString QDownloadProcess::downloadSpeed() const
{
    return m_downloadSpeed;
}
void QDownloadProcess::setDownloadSpeed(const QString downloadSpeed)
{
    m_downloadSpeed = downloadSpeed;
}

const QString QDownloadProcess::downloadElapsed()
{
    if (m_ElapsedTimer.isValid()) {
        QString strFormat;
        QTime elapsed;
        elapsed = elapsed.addMSecs(m_ElapsedTimer.elapsed());

        if (elapsed.hour() > 0) {
            strFormat.append("H'h'");
        }

        if (elapsed.minute() > 0) {
            strFormat.append("m'm'");
        }

        if (elapsed.second() > 0) {
            strFormat.append("s's'");
        }

        m_downloadElapsed = elapsed.toString(strFormat);
    }

    return m_downloadElapsed;
}
void QDownloadProcess::setDownloadElapsed(const QString downloadElapsed)
{
    m_downloadElapsed = downloadElapsed;
}

const QString QDownloadProcess::downloadEta() const
{
    return m_downloadEta;
}
void QDownloadProcess::setDownloadEta(const QString downloadEta)
{
    m_downloadEta = downloadEta;
}

const QDownloadProcess::DownloadStatus QDownloadProcess::downloadStatus() const
{
    return m_downloadStatus;
}
void QDownloadProcess::setDownloadStatus(const QDownloadProcess::DownloadStatus
        downloadStatus)
{
    m_downloadStatus = downloadStatus;
}

bool QDownloadProcess::wget(const QString url, const QString saveDirectory,
                            const QString fileName, qint64 fileSize, const QString md5)
{
    qprofileDebug(QtWarningMsg) << url << saveDirectory << fileName << fileSize <<
                                md5;
    bool ret = false;

    do {
        initial();
        setTotalSize(fileSize / 1024 / 1024);
        m_filePath = QString("%1/%2").arg(saveDirectory).arg(fileName);
        m_md5 = md5;

        if (!isDirExists(saveDirectory)) {
            qprofileDebug(QtWarningMsg) << "SaveDirectoryNotExists:" << saveDirectory;
            setDownloadStatus(SaveDirectoryNotExists);
            break;
        }

        qint64 free = getFreeSpace(saveDirectory);

        if (free <= fileSize && fileSize != -1) {
            qprofileDebug(QtWarningMsg) << "NotEnoughSpace(free/fileSize):" << free <<
                                        fileSize;
            setDownloadStatus(NotEnoughSpace);
            break;
        }

        QString cmdFileExists = QString("%1 -S --spider --no-check-certificate %2").arg(
                                    WgetCommand).arg(url);
        QProcess fileExistsProcess;
        fileExistsProcess.setProcessChannelMode(QProcess::MergedChannels);
        fileExistsProcess.start(cmdFileExists);
        qprofileDebug(QtWarningMsg) << cmdFileExists;

        if (!fileExistsProcess.waitForStarted(DownloadTimeout)) {
            qprofileDebug(QtWarningMsg) << "waitForStarted failed.";
            setDownloadStatus(StartFailed);
            break;
        }

        if (!fileExistsProcess.waitForFinished(DownloadTimeout)) {
            qprofileDebug(QtWarningMsg) << "waitForFinished failed.";
            setDownloadStatus(Failed);
            break;
        }

        if (!isRemoteFileExists(QString(fileExistsProcess.readAll()))) {
            setDownloadStatus(RemoteFileNotExists);
            break;
        }

        if (m_wgetProcess.state() != QProcess::NotRunning) {
            m_wgetProcess.close();
        }

        QString cmdGet = QString("%1 -c --no-check-certificate -O %2 %3").arg(
                             WgetCommand).arg(fileName).arg(url);
        m_wgetProcess.setWorkingDirectory(saveDirectory);
        m_wgetProcess.start(cmdGet);
        qprofileDebug(QtWarningMsg) << cmdGet;
#ifndef BUILD_X86
        int oldPri = getpriority(PRIO_PROCESS, m_wgetProcess.pid());
        setpriority(PRIO_PROCESS, m_wgetProcess.pid(), oldPri + 15);
        int newPri = getpriority(PRIO_PROCESS, m_wgetProcess.pid());
        qprofileDebug(QtWarningMsg) << "m_wgetProcess.pid/oldpri/newpri:" <<
                                    m_wgetProcess.pid()  << oldPri << newPri;
#endif

        if (!m_wgetProcess.waitForStarted(DownloadTimeout)) {
            qprofileDebug(QtWarningMsg) << "waitForStarted failed.";
            setDownloadStatus(StartFailed);
            break;
        }

        m_ElapsedTimer.start();
        setDownloadStatus(Downloading);
        m_timer.start();
        ret = true;
    } while (0);

    return ret;
}

bool QDownloadProcess::stop()
{
    m_timer.stop();
    int count = 0;

    while (m_wgetProcess.state() != QProcess::NotRunning && ++count <= 50) {
        m_wgetProcess.close();
        usleep(100000);
    }

    return (m_wgetProcess.state() == QProcess::NotRunning);
}

void QDownloadProcess::checkOutput()
{
    if (!m_timer.isActive()) {
        return ;
    }

    m_timer.start();

    if (m_wgetProcess.canReadLine()) {
        QString data = QString(m_wgetProcess.readLine());
        qprofileDebug(QtDebugMsg) << data;
        QRegExp infoExp("(\\d+)K.*(\\d+\\%)\\s+([^\\s]*)\\s+([^\\s]*)");
        infoExp.indexIn(data);
        QStringList infoCapList = infoExp.capturedTexts();

        if (infoCapList.count() > 4) {
            qprofileDebug(QtDebugMsg) << infoCapList[1] << infoCapList[2] <<
                                      infoCapList[3] << infoCapList[4];

            if (!infoCapList[2].isEmpty()) {
                setDownloadedSize(infoCapList[1].toLongLong() * 1024);
                setDownloadedPercentage(infoCapList[2]);
                setDownloadSpeed(QString("%1/s").arg(infoCapList[3]));
                setDownloadEta(infoCapList[4]);
            }
        }
    }
}

void QDownloadProcess::checkFinished(int exitCode,
                                     QProcess::ExitStatus exitStatus)
{
    qprofileDebug(QtWarningMsg) << "CheckFinished" << exitCode << exitStatus;
    m_timer.stop();
    QProcess::execute("sync");

    if (exitCode == 0 && QProcess::NormalExit == exitStatus) {
        if (m_md5.isEmpty() || matchMD5(m_filePath, m_md5)) {
            setDownloadStatus(Succeed);
        } else {
            setDownloadStatus(MD5Mismatch);
        }
    } else {
        setDownloadStatus(Failed);
    }
}

void QDownloadProcess::onTimeout()
{
    if (m_wgetProcess.state() != QProcess::NotRunning) {
        m_wgetProcess.close();
    }

    setDownloadStatus(Timeout);
}

bool QDownloadProcess::isRemoteFileExists(const QString message)
{
    QRegExp fileExistsExp(".*(Remote file .*).*");
    fileExistsExp.indexIn(message);
    QStringList fileExistsCapList = fileExistsExp.capturedTexts();

    if (fileExistsCapList.count() > 1) {
        QString existString = fileExistsCapList.last().trimmed();
        qprofileDebug(QtWarningMsg) << "checkFileExists" << existString;

        if (existString.contains("Remote file exists")) {
            return true;
        }
    }

    return false;
}

bool QDownloadProcess::isDirExists(const QString &dir)
{
    if (dir.contains(UsbDir)) {
        QString cmd = "mount";
        QProcess process;
        process.setProcessChannelMode(QProcess::MergedChannels);
        process.start(cmd);

        if (!process.waitForStarted()) {
            qprofileDebug(QtWarningMsg) << "waitForStarted failed.";
            return false;
        }

        if (!process.waitForFinished()) {
            qprofileDebug(QtWarningMsg) << "waitForFinished failed.";
            return false;
        }

        QString data = QString(process.readAll());
        QRegExp usbExp(QString(".*(%1).*").arg(UsbDir));
        usbExp.indexIn(data);
        QStringList usbCapList = usbExp.capturedTexts();
        qprofileDebug(QtDebugMsg) << usbCapList;

        if (usbCapList.count() > 1) {
            QString usbString = usbCapList.last().trimmed();

            if (!usbString.contains(UsbDir)) {
                return false;
            }
        } else {
            return false;
        }
    }

    QFileInfo fileInfo(dir);
    return fileInfo.isDir();
}

qint64 QDownloadProcess::getFreeSpace(const QString &dir)
{
    qint64 freeSpace = 0;
#if defined(Q_OS_LINUX)
    struct statvfs st;

    if (::statvfs(dir.toLocal8Bit(), &st) == -1)
        return freeSpace;

    freeSpace = (qint64)st.f_bfree * (qint64)st.f_frsize;
#endif
    return freeSpace;
}

bool QDownloadProcess::matchMD5(const QString filePath, const QString md5)
{
    setDownloadStatus(CheckingMD5);
    QByteArray md5ByteArray = GetMD5(filePath);
    QString localMd5 = md5ByteArray.toHex().constData();
    qprofileDebug(QtWarningMsg) << "MD5(Local/Server): " << localMd5 << md5;

    if (localMd5 == md5) {
        return true;
    } else {
        QFile file(filePath);
        file.remove();
    }

    return false;
}

QByteArray QDownloadProcess::GetMD5(const QString &filePath)
{
    QFile sourceFile(filePath);
    qint64 fileSize = sourceFile.size();
    const qint64 bufferSize = 1024 * 10;

    if (sourceFile.open(QIODevice::ReadOnly)) {
        char buffer[bufferSize];
        int bytesRead;
        int readSize = ((fileSize <= bufferSize) ? fileSize : bufferSize);
        QCryptographicHash hash(QCryptographicHash::Md5);

        while (readSize > 0 && (bytesRead = sourceFile.read(buffer, readSize)) > 0) {
            QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents, 100);
            fileSize -= bytesRead;
            hash.addData(buffer, bytesRead);
            readSize = ((fileSize <= bufferSize) ? fileSize : bufferSize);
        }

        sourceFile.close();
        return hash.result();
    }

    return QByteArray();
}
