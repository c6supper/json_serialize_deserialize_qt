/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qmsghandle.h"
#include "qprofile_debug.h"
#include "qabstractenvironment.h"
#include <stdio.h>
#include <stdlib.h>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QFileInfo>
#include <QDir>

using namespace QProfile;

void qGdbPrint(const QVariant msg)
{
    qDebug() << msg;
}

void qMessageOutput(QtMsgType type, const char *msg)
{
    QString strMsg = QString(msg);
    QtMsgType logLevel = (QtMsgType)QAbstractEnvironment::LogLevel();
    QString logFilePathName = QAbstractEnvironment::LogFile();

    if (type < logLevel) {
        return;
    }

    if (strMsg.length() > MAX_QPROFILE_DEBUG_LENGTH)
        strMsg = strMsg.left(MAX_QPROFILE_DEBUG_LENGTH);

    QString log;

    switch (type) {
    case QtDebugMsg:
        log = QString("Debug: %1\n").arg(strMsg);
        break;

    case QtWarningMsg:
        log = QString("Warning: %1\n").arg(strMsg);
        break;

    case QtCriticalMsg:
        log = QString("Critical: %1\n").arg(strMsg);
        break;

    case QtFatalMsg:
        log = QString("Fatal: %1\n").arg(strMsg);
#ifdef BUILD_X86
        abort();
#endif
    }

    log.prepend(QDateTime::currentDateTime().toString("dd:hh::mm::ss.zzz "));

    if (!logFilePathName.isEmpty()) {
        QFileInfo fileInfo(logFilePathName);
        QFile logFile(logFilePathName);

        if (!fileInfo.exists()) {
            QDir logDir = fileInfo.absoluteDir();

            if (!logDir.exists()) {
                if (logDir.mkpath(logDir.absolutePath())) {
#ifdef BUILD_X86

                    if (!QFile::setPermissions(logDir.absolutePath(),
                                               QFile::ReadOwner | QFile::WriteOwner | QFile::ReadUser
                                               | QFile::WriteUser | QFile::ReadOther | QFile::WriteOther | QFile::WriteGroup |
                                               QFile::ReadGroup | QFile::ExeOwner | QFile::ExeUser | QFile::ExeGroup |
                                               QFile::ExeOther)) {
                        fprintf(stderr, "%s\n",
                                QString("%1:%2:%3: Failed to set permission for file %4").arg(__FILE__).arg(
                                    __LINE__).arg(__PRETTY_FUNCTION__).arg(
                                    logDir.absolutePath()).toLatin1().data());
                    }

#endif
                }
            }

#ifdef BUILD_X86

            if (!fileInfo.exists()) {
                if (logFile.open(QIODevice::WriteOnly | QIODevice::Append)) {
                    logFile.close();

                    if (!QFile::setPermissions(logFilePathName,
                                               QFile::ReadOwner | QFile::WriteOwner | QFile::ReadUser
                                               | QFile::WriteUser | QFile::ReadOther | QFile::WriteOther | QFile::WriteGroup |
                                               QFile::ReadGroup | QFile::ExeOwner | QFile::ExeUser | QFile::ExeGroup |
                                               QFile::ExeOther)) {
                        fprintf(stderr, "%s\n",
                                QString("%1:%2:%3: Failed to set permission for file %4").arg(__FILE__).arg(
                                    __LINE__).arg(__PRETTY_FUNCTION__).arg(logFilePathName).toLatin1().data());
                    }
                }
            }

#endif
        }

        if (logFile.open(QIODevice::WriteOnly | QIODevice::Append)) {
            QTextStream ts(&logFile);
            ts << log << endl;
            return;
        }
    }

    fprintf(stderr, "%s", log.toLatin1().data());
}

