#include "qrserverdownloadprocess.h"
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

QRserverDownloadProcess::QRserverDownloadProcess(QObject *parent)
    : QDownloadProcess(parent)
{
}

QRserverDownloadProcess::~QRserverDownloadProcess()
{
}

bool QRserverDownloadProcess::wget(const QString url,
                                   const QString saveDirectory,
                                   const QString fileName, qint64 fileSize, const QString md5)
{
    qprofileDebug(QtWarningMsg) << url << saveDirectory << fileName << fileSize <<
                                md5;
    bool ret = false;

    do {
        initial();
        m_filePath = QString("%1/%2").arg(saveDirectory).arg(fileName);
        m_md5 = md5;

        if (!isDirExists(saveDirectory)) {
            qprofileDebug(QtWarningMsg) << "SaveDirectoryNotExists:" << saveDirectory;
            setDownloadStatus(SaveDirectoryNotExists);
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

void QRserverDownloadProcess::checkFinished(int exitCode,
        QProcess::ExitStatus exitStatus)
{
    qprofileDebug(QtWarningMsg) << "CheckFinished" << exitCode << exitStatus;
    m_timer.stop();
    QProcess::execute("sync");

    if (exitCode == 3) {
        setDownloadStatus(NotEnoughSpace);
        return;
    } else {
        QDownloadProcess::checkFinished(exitCode, exitStatus);
    }
}
