/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include <QtGlobal>
#include <QMetaEnum>
#include <QCoreApplication>
#include <QFile>
#include "qprofile_debug.h"
#include "qrserverprofile.h"
#include "qrserverenvironment.h"

#if defined(BUILD_X86)
#include "qjson/parser.h"
#include "qjson/qobjecthelper.h"
#else
#include <QJson/Parser>
#include <QJson/QObjectHelper>
#endif

using namespace QRserver;

QRserverFile::QRserverFile(QObject *parent)
    : QObject(parent)
{
}

QRserverFile::~QRserverFile()
{
    QMutexLocker locker(&m_mutex);

    while (!m_fileTypeList.isEmpty())
        delete m_fileTypeList.takeFirst();
}

QVariantList QRserverFile::fileTypeList()
{
    QMutexLocker locker(&m_mutex);
    QVariantList fileTypeList;
    QList<QRserverFileType *>::const_iterator iter;

    for (iter = m_fileTypeList.constBegin(); iter != m_fileTypeList.constEnd();
         ++iter) {
        fileTypeList.append((*iter)->toVariant());
    }

    return fileTypeList;
}
void QRserverFile::setFileTypeList(QVariantList &fileTypeList)
{
    QMutexLocker locker(&m_mutex);

    while (!m_fileTypeList.isEmpty())
        delete m_fileTypeList.takeFirst();

    QVariantList::const_iterator iter;

    for (iter = fileTypeList.constBegin(); iter != fileTypeList.constEnd();
         ++iter) {
        QRserverFileType *fileType = new QRserverFileType(this);
        fileType->fromVariant(*iter);
        m_fileTypeList.append(fileType);
    }
}

const QVariant QRserverFile::getFileType(const qint32 type)
{
    QMutexLocker locker(&m_mutex);
    QList<QRserverFileType *>::const_iterator iter;

    for (iter = m_fileTypeList.constBegin(); iter != m_fileTypeList.constEnd();
         ++iter) {
        if ((*iter)->fileType() == type2String(type)) {
            QVariantMap fileType = (*iter)->toVariant().toMap();
            fileType[QRserverFileType::PropertyFileType] = type;
            return QVariant(fileType);
        }
    }

    return QVariant();
}

const QRserverFileType *QRserverFile::getRserverFileType(
    const QString &filePath)
{
    QMutexLocker locker(&m_mutex);
    QList<QRserverFileType *>::const_iterator iter;

    for (iter = m_fileTypeList.constBegin(); iter != m_fileTypeList.constEnd();
         ++iter) {
        if ((*iter)->matchedFileType(filePath)) {
            return *iter;
        }
    }

    return nullptr;
}

const QVariant QRserverFile::getFileType(const QString &filePath,
        const bool enableUseDirPath)
{
    QMutexLocker locker(&m_mutex);
    QList<QRserverFileType *>::const_iterator iter;

    for (iter = m_fileTypeList.constBegin(); iter != m_fileTypeList.constEnd();
         ++iter) {
        if ((*iter)->matchedFileType(filePath)) {
            QVariantMap fileType = (*iter)->toVariant().toMap();
            fileType[QRserverFileType::PropertyFileType] = stringToType(
                        fileType[QRserverFileType::PropertyFileType].toString());

            if (-1 == fileType[QRserverFileType::PropertyFileType].toInt()) {
                break;
            }

            if (isKey(filePath, (*iter)->measureMode(), (*iter)->measureType())
                || enableUseDirPath) {
                fileType[QRserverFileType::PropertyDisplayName] = getDisplayName(filePath,
                        (*iter)->measureMode(), (*iter)->measureType());
                return QVariant(fileType);
            } else {
                break;
            }
        }
    }

    return QVariant();
}
