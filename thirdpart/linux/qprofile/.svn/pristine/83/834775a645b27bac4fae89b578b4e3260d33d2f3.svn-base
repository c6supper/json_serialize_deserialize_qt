/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */


#include <QMetaEnum>
#include <QCoreApplication>
#include "qabstractenvironment.h"
#include "qprofile_debug.h"
#include <QDir>

using namespace QProfile;

QAbstractEnvironment *QAbstractEnvironment::m_instance = nullptr;
const QString QAbstractEnvironment::m_logLevelArg = "logLevel";
const QString QAbstractEnvironment::m_logFileArg = "logFile";

QAbstractEnvironment::QAbstractEnvironment(QObject *parent):
    QAbstractJsonElement(parent),
    m_profileHome("/usr/local/etc"), m_logLevel(Warning),
    m_verboseLevel(NormalRunning), m_debugMode(false)
{
    Q_ASSERT(parent != nullptr);
#ifdef BUILD_X86
    m_profileHome = QCoreApplication::applicationDirPath() + "/../etc";
#endif
}

void QAbstractEnvironment::initialize()
{
    bool ok;
    qint32 logLevel = Argument(m_logLevelArg).toInt(&ok);

    if (ok) {
        m_logLevel = (LogLevelEnum)logLevel;
    }

    QString logFile = Argument(m_logFileArg);

    if (!logFile.isEmpty()) {
        m_logFile = logFile;
    }
}

const qint32 QAbstractEnvironment::logLevelEnum2Index(const QString
        logLevelString)
{
    qint32 idx =
        QAbstractEnvironment::staticMetaObject.indexOfEnumerator("LogLevelEnum");
    QMetaEnum metaEnum = QAbstractEnvironment::staticMetaObject.enumerator(idx);
    return metaEnum.keyToValue(logLevelString.toLatin1().data());
}
const qint32 QAbstractEnvironment::LogLevel()
{
    if (m_instance == nullptr) {
        return Warning;
    }

    return m_instance->logLevel();
}
void QAbstractEnvironment::CheckEnvironment()
{
    m_instance->checkEnvironment();
}
void QAbstractEnvironment::PrintEnvironmentVarList()
{
    m_instance->printEnvironmentVarList();
}
const QString QAbstractEnvironment::LastProfile()
{
    return m_instance->lastProfile();
}
const QString QAbstractEnvironment::DefaultProfile()
{
    return m_instance->defaultProfile();
}
void QAbstractEnvironment::checkEnvironment()
{
    qprofileDebug(QtWarningMsg) << "base class";
}
void QAbstractEnvironment::printEnvironmentVarList()
{
    qprofileDebug(QtWarningMsg) << toJson();
}
const QString QAbstractEnvironment::profileHome() const
{
    return m_profileHome;
}
QAbstractEnvironment *QAbstractEnvironment::SetInstance(
    QAbstractEnvironment *instance)
{
    QAbstractEnvironment *lastInstance = m_instance;
    m_instance = instance;
    m_instance->initialize();
    return lastInstance;
}
const QString QAbstractEnvironment::LogFile()
{
    if (m_instance == nullptr) {
        return QString();
    }

    return m_instance->logFile();
}
const QString QAbstractEnvironment::Argument(const QString name)
{
    if (name.isEmpty()) {
        return "";
    }

    QStringList args = QCoreApplication::arguments();
    QRegExp rxArg(QString("--%1=(.*)").arg(name));

    for (int i = 1; i < args.size(); ++i) {
        if (rxArg.indexIn(args.at(i)) != -1) {
            fprintf(stderr, "%s\n", QString("%1:%2:%3:--%4=%5").arg(__FILE__).arg(
                        __LINE__).arg(__PRETTY_FUNCTION__).arg(name).arg(rxArg.cap(
                                    1)).toLatin1().data());
            return  rxArg.cap(1);
        }
    }

    return "";
}
void QAbstractEnvironment::setProfileHome(const QString profileHome)
{
    QDir home(profileHome);

    if (home.exists() && home.isAbsolute()) {
        m_profileHome = profileHome;

        if (!QDir::setCurrent(m_profileHome)) {
            qprofileDebug(QtWarningMsg) << "Home path could not set to " << m_profileHome;
        }
    } else {
        qprofileDebug(QtWarningMsg) <<
                                    "Home path is not a absolute path or not existed.";
    }
}
const qint32 QAbstractEnvironment::verboseLevel() const
{
    return (qint32)m_verboseLevel;
}
void QAbstractEnvironment::setVerboseLevel(const qint32 verboseLevel)
{
    qint32 idx = metaObject()->indexOfEnumerator("VerboseLevel");
    QMetaEnum metaEnum = metaObject()->enumerator(idx);
    const char *key = metaEnum.valueToKey(verboseLevel);

    if (key != nullptr) {
        m_verboseLevel = (VerboseLevel)verboseLevel;
    } else {
        qprofileDebug(QtWarningMsg) << "Unsupported verboseLevel " << verboseLevel;
    }
}
const QString QAbstractEnvironment::version() const
{
    return QString();
}
void QAbstractEnvironment::setVersion(const QString version)
{
    qprofileDebug(QtWarningMsg) << "Set version is not allowed.";
}
const qint32 QAbstractEnvironment::logLevel() const
{
    return (qint32)m_logLevel;
}
void QAbstractEnvironment::setLogLevel(const qint32 logLevel)
{
    qint32 idx = metaObject()->indexOfEnumerator("LogLevelEnum");
    QMetaEnum metaEnum = metaObject()->enumerator(idx);
    const char *key = metaEnum.valueToKey(logLevel);

    if (key != nullptr) {
        m_logLevel = (LogLevelEnum)logLevel;
    } else {
        qprofileDebug(QtWarningMsg) << "Unsupported logLevel " << logLevel;
    }
}
const QString QAbstractEnvironment::logFile() const
{
    return m_logFile;
}

void QAbstractEnvironment::setLogFile(const QString logFile)
{
    m_logFile = logFile;

    if (logFile.isEmpty()) {
        return;
    }

    quint32 idx = m_logFile.lastIndexOf("/");
    m_logFile = m_logFile.insert(idx + 1,
                                 QString("%1_").arg(QCoreApplication::applicationPid()));
}

const bool QAbstractEnvironment::debugMode() const
{
    return m_debugMode;
}
void QAbstractEnvironment::setDebugMode(const bool debugMode)
{
    m_debugMode = debugMode;
}

const bool QAbstractEnvironment::DebugMode()
{
    if (m_instance == nullptr) {
        return false;
    }

    return m_instance->debugMode();
}

QAbstractEnvironment::~QAbstractEnvironment()
{
}

const QStringList QAbstractEnvironment::ignoredProperties() const
{
    QStringList ignoredPropertiesList(QAbstractJsonElement::ignoredProperties());
    ignoredPropertiesList << "profileHome";
    return ignoredPropertiesList;
}
