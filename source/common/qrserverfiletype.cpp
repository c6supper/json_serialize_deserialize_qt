/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qrserverfiletype.h"

using namespace QRserver;

const QString QRserverFileType::propertyRegExp = "regExp";
const QString QRserverFileType::PropertyFileType = "fileType";
const QString QRserverFileType::PropertyDisplayName = "displayName";

QRserverFileType::QRserverFileType(QObject *parent)
    : QProfile::QAbstractJsonElement(parent)
{
}

QRserverFileType::~QRserverFileType()
{
}

QString QRserverFileType::fileType() const
{
    return m_fileType;
}
void QRserverFileType::setFileType(QString fileType)
{
    m_fileType = fileType;
}

QVariantList QRserverFileType::regExpList()
{
    QMutexLocker locker(&m_mutex);
    QVariantList regExpList;
    QList<QRegExp>::const_iterator iter;

    for (iter = m_regExpList.constBegin(); iter != m_regExpList.constEnd();
         ++iter) {
        QVariantMap regExpMap;
        regExpMap[propertyRegExp] = (*iter).pattern();
        regExpList.append(QVariant(regExpMap));
    }

    return regExpList;
}
void QRserverFileType::setRegexpList(QVariantList regExpList)
{
    QMutexLocker locker(&m_mutex);
    m_regExpList.clear();
    QVariantList::const_iterator iter;

    for (iter = regExpList.constBegin(); iter != regExpList.constEnd(); ++iter) {
        QVariantMap regExpMap = (*iter).toMap();
        QRegExp regexp(regExpMap[propertyRegExp].toString());
        m_regExpList.append(regexp);
    }
}

QString QRserverFileType::displayName() const
{
    return m_displayName;
}
void QRserverFileType::setDisplayName(QString displayName)
{
    m_displayName = displayName;
}

QString QRserverFileType::typeName() const
{
    return m_typeName;
}
void QRserverFileType::setType2Name(QString typeName)
{
    m_typeName = typeName;
}

QString QRserverFileType::measureType() const
{
    return m_measureType;
}
void QRserverFileType::setMeasureType(QString measureType)
{
    m_measureType = measureType;
}

QString QRserverFileType::measureMode() const
{
    return m_measureMode;
}
void QRserverFileType::setMeasureMode(QString measureMode)
{
    m_measureMode = measureMode;
}

bool QRserverFileType::matchedFileType(const QString &pathFile)
{
    QMutexLocker locker(&m_mutex);
    QList<QRegExp>::const_iterator iter;

    for (iter = m_regExpList.constBegin(); iter != m_regExpList.constEnd();
         ++iter) {
        if ((*iter).indexIn(pathFile) > -1) {
            return true;
        }
    }

    return false;
}
