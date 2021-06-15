/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QABSTRACTENVIRONMENT_H
#define QABSTRACTENVIRONMENT_H

#include "qprofile_global.h"
#include "qabstractjsonelement.h"

namespace QProfile
{
    class QAbstractEnvironment : public QAbstractJsonElement
    {
            Q_OBJECT
            Q_ENUMS(LogLevelEnum)
            Q_ENUMS(VerboseLevel)

            Q_PROPERTY(QString profileHome READ profileHome WRITE setProfileHome)
            Q_PROPERTY(qint32 logLevel READ logLevel WRITE setLogLevel)
            Q_PROPERTY(QString logFile READ logFile WRITE setLogFile)
            Q_PROPERTY(qint32 verboseLevel READ verboseLevel WRITE setVerboseLevel)
            Q_PROPERTY(QString version READ version WRITE setVersion)
            Q_PROPERTY(bool debugMode READ debugMode WRITE setDebugMode)

        public:
            enum LogLevelEnum { Debug = QtDebugMsg, Warning = QtWarningMsg,
                                Critical = QtCriticalMsg, Fatal = QtFatalMsg
                              };
            enum VerboseLevel { NormalRunning, InterGrationTesting, UintTesting};
        public:
            explicit QAbstractEnvironment(QObject *parent = nullptr);
            virtual ~QAbstractEnvironment();
            virtual void pureVirtualMock() {};

            const QString profileHome() const;
            void setProfileHome(const QString profileHome);

            const qint32 logLevel() const;
            void setLogLevel(const qint32 logLevel);

            const QString logFile() const;
            void setLogFile(const QString logFile);

            const qint32 verboseLevel() const;
            void setVerboseLevel(const qint32 verboseLevel);

            virtual const QString version() const;
            virtual void setVersion(const QString version);

            const bool debugMode() const;
            void setDebugMode(const bool debugMode);

            virtual void initialize();

            static const qint32 LogLevel();
            static void CheckEnvironment();
            static void PrintEnvironmentVarList();
            static const QString LastProfile();
            static const QString DefaultProfile();

            static QAbstractEnvironment *SetInstance(QAbstractEnvironment *instance);

            static const QString Argument(const QString name);
            static const QString LogFile();
            static const bool DebugMode();

        private:
            static const qint32 logLevelEnum2Index(const QString logLevelString);

        protected:
            virtual void printEnvironmentVarList();
            virtual void checkEnvironment();
            virtual const QString lastProfile() = 0;
            virtual const QString defaultProfile() = 0;
            virtual const QStringList ignoredProperties() const;

        protected:
            static QAbstractEnvironment 		*m_instance;
            static const QString				m_logLevelArg;
            static const QString				m_logFileArg;

        private:
            QString 							m_profileHome;
            LogLevelEnum						m_logLevel;
            QString								m_logFile;
            VerboseLevel						m_verboseLevel;
            bool								m_debugMode;
    };
}
#endif // QABSTRACTENVIRONMENT_H
