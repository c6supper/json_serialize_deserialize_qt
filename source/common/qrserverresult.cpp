/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include <QtGlobal>
#include <QMetaEnum>
#include <QCoreApplication>
#include <QFile>
#include "qprofile_debug.h"
#include "qrserverresult.h"
#include "qrserverenvironment.h"
#include "qetherresultfilter.h"
#include "qosaresultfilter.h"
#include "qotdrresultfilter.h"
#include "qfiberresultfilter.h"
#include "qsdhresultfilter.h"
#include "qcpriresultfilter.h"
#include "qecpriresultfilter.h"
#include "qotuxresultfilter.h"
#include "qodu0resultfilter.h"
#include "qvipagresultfilter.h"
#include "qwifiresultfilter.h"
#include "qgponresultfilter.h"
#include "qmtt52xresultfilter.h"
#include "qg703resultfilter.h"
#include "qc3794resultfilter.h"
#include "qscreenshotresultfilter.h"
#include "qfilemodel.h"
#include <QFileInfo>
#include <QDir>

#if defined(BUILD_X86)
#include "qjson/parser.h"
#include "qjson/qobjecthelper.h"
#else
#include <QJson/Parser>
#include <QJson/QObjectHelper>
#endif

using namespace QRserver;

QRserverResult *QRserverResult::m_instance = nullptr;
const QString QRserverResult::DefaultRserverResultConfig = "rserverresult.json";

QRserverResult::QRserverResult(QObject *parent)
    : QRserverFile(parent)
{
    QMutexLocker locker(&m_mutex);
    m_resultFilterList.append(new QEtherResultFilter(this));
    m_resultFilterList.append(new QOSAResultFilter(this));
    m_resultFilterList.append(new QOtdrResultFilter(this));
    m_resultFilterList.append(new QFiberResultFilter(this));
    m_resultFilterList.append(new QSdhResultFilter(this));
    m_resultFilterList.append(new QCpriResultFilter(this));
    m_resultFilterList.append(new QEcpriResultFilter(this));
    m_resultFilterList.append(new QOtuxResultFilter(this));
    m_resultFilterList.append(new QOdu0ResultFilter(this));
    m_resultFilterList.append(new QVipagResultFilter(this));
    m_resultFilterList.append(new QWifiResultFilter(this));
    m_resultFilterList.append(new QGponResultFilter(this));
    m_resultFilterList.append(new QMtt52xResultFilter(this));
    m_resultFilterList.append(new QG703ResultFilter(this));
    m_resultFilterList.append(new QC3794ResultFilter(this));
    m_resultFilterList.append(new QScreenshotResultFilter(this));
}

QRserverResult::~QRserverResult()
{
}

const QString QRserverResult::type2String(const qint32 type)
{
    qint32 idx = metaObject()->indexOfEnumerator("ResultType");
    QMetaEnum metaEnum = metaObject()->enumerator(idx);
    return metaEnum.valueToKey((ResultType)type);
}

const qint32 QRserverResult::stringToType(const QString typeString)
{
    qint32 idx = metaObject()->indexOfEnumerator("ResultType");
    QMetaEnum metaEnum = metaObject()->enumerator(idx);
    return metaEnum.keyToValue(typeString.toLatin1().data());
}

const QString QRserverResult::GetFilterPattern()
{
    return Instance()->getFilterPattern();
}

const QString QRserverResult::getFilterPattern()
{
    QMutexLocker locker(&m_mutex);
    QList<QAbstractResultFilter *>::const_iterator i;
    QStringList patternList;
    QString pattern("^[a-zA-z0-9\\_' '\\-]+(");

    for (i = m_resultFilterList.constBegin(); i != m_resultFilterList.constEnd();
         ++i) {
        if (!patternList.contains((*i)->filterPattern())
            && !(*i)->filterPattern().isEmpty()) {
            pattern += (*i)->filterPattern() + "|";
            patternList << (*i)->filterPattern();
        }
    }

    if (pattern.endsWith("|")) {
        pattern.remove(pattern.lastIndexOf("|"), 1);
    }

    return (pattern + ")$");
}

/*void QRserverResult::registerFilter(QAbstractResultFilter *filter)
{
    QMutexLocker locker(&m_mutex);

    if (!m_resultFilterList.contains(filter)) {
        m_resultFilterList.append(filter);
    } else {
        qprofileDebug(QtFatalMsg) << "filter had been appended before !";
    }
}*/

QAbstractResultFilter *QRserverResult::filter(const QString measureMode,
        const QString measureType)
{
    QMutexLocker locker(&m_mutex);
    QList<QAbstractResultFilter *>::const_iterator i;

    for (i = m_resultFilterList.constBegin(); i != m_resultFilterList.constEnd();
         ++i) {
        if ((*i)->matched(measureMode, measureType))
            return *i;
    }

    return nullptr;
}

const QString QRserverResult::getDisplayName(const QString &filePath,
        const QString measureMode, const QString measureType)
{
    QAbstractResultFilter *filter = this->filter(measureMode, measureType);

    if (filter == nullptr) {
        return QString();
    }

    return filter->getDisplayName(filePath);
}

const QVariant QRserverResult::GetResultKeyFileVar()
{
    return Instance()->getResultKeyFileVar();
}

const QVariant QRserverResult::getResultKeyFileVar()
{
    QMutexLocker locker(&m_mutex);
    QList<QAbstractResultFilter *>::const_iterator i;
    QVariantList keyFileVarList;

    for (i = m_resultFilterList.constBegin(); i != m_resultFilterList.constEnd();
         ++i) {
        QVariantList singleKeyFileVarList;
        singleKeyFileVarList.append((*i)->measureMode());
        singleKeyFileVarList.append((*i)->measureType());
        singleKeyFileVarList.append((*i)->keyFilePattern());
        keyFileVarList.append(QVariant(singleKeyFileVarList));
    }

    return QVariant(keyFileVarList);
}

bool QRserverResult::isKey(const QString &absolutePath,
                           const QString measureMode,
                           const QString measureType)
{
    if (measureMode.isEmpty()) {
        qprofileDebug(QtWarningMsg) << "measureMode should not be empty.";
        return false;
    }

    QAbstractResultFilter *filter = this->filter(measureMode, measureType);

    if (filter == nullptr) {
        qprofileDebug(QtDebugMsg) << "Trying to key a unkonw result " << absolutePath <<
                                  measureMode << measureType;
        return false;
    }

    return filter->isKey(absolutePath);
}

const QVariant QRserverResult::GetFileType(const QString &filePath)
{
    QVariant fileType = Instance()->getFileType(filePath);

    if (fileType.isValid()) {
        QFileModel fileModel;
        fileModel.fromVariant(fileType);
        QAbstractResultFilter *filter = Instance()->filter(fileModel.measureMode(),
                                        fileModel.measureType());

        if (filter != nullptr) {
            QFileInfo keyInfo(filePath);
            QFileInfo xmlFileInfo(keyInfo.absoluteDir().absolutePath() + "/" +
                                  keyInfo.completeBaseName());
            fileModel.setJobId(filter->getJobIdFromXml(xmlFileInfo.absoluteFilePath()));
            fileModel.setLocation(filter->getLocationFromXml(
                                      xmlFileInfo.absoluteFilePath()));
        }

        return fileModel.toVariant();
    }

    return  QVariant();
}

const QStringList QRserverResult::GetRelatedFileList(
    const QString absoluteKeyPath,
    const QString measureMode,
    const QString measureType)
{
    QAbstractResultFilter *filter = Instance()->filter(measureMode, measureType);

    if (filter != nullptr) {
        return filter->getRelatedFileFolderList(absoluteKeyPath);
    }

    return QStringList();
}

const QStringList QRserverResult::GetFileListNeedToBeDeleted(
    const QString absoluteKeyPath,
    const QString measureMode,
    const QString measureType)
{
    QAbstractResultFilter *filter = Instance()->filter(measureMode, measureType);

    if (filter != nullptr) {
        return filter->getFileListNeedToBeDeleted(absoluteKeyPath);
    }

    return QStringList();
}

const bool QRserverResult::postUploading(const QString absoluteKeyPath,
        const QString measureMode, const QString measureType,
        const quint32 commandStatus)
{
    QAbstractResultFilter *filter = Instance()->filter(measureMode, measureType);

    if (filter != nullptr) {
        return filter->postUploading(absoluteKeyPath, commandStatus);
    }

    return false;
}

QRserverResult *QRserverResult::Instance()
{
    if (m_instance == nullptr) {
        m_instance = new QRserverResult(QCoreApplication::instance());
        QFile file(QRserverEnvironment::ResultConfig());

        if (!file.exists()) {
            qprofileDebug(QtFatalMsg) << QRserverEnvironment::ResultConfig() <<
                                      " is missing! ";
        }

        if (file.open(QIODevice::ReadOnly)) {
            QByteArray data = file.readAll();
            QJson::Parser parser;
            bool ok = false;
            QVariant variant = parser.parse(data, &ok);

            if (!ok) {
                qprofileDebug(QtFatalMsg) << "Failed to load rserverresult.json!";
            }

            QJson::QObjectHelper::qvariant2qobject(variant.toMap(), m_instance);
        }
    }

    return m_instance;
}
