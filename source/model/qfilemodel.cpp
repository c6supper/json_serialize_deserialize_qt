/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */


#include "qfilemodel.h"
#include "qobjectfactory.h"

using namespace QProfile;
using namespace QRserver;

QObjectSelfRegisteration<QFileModel> registerQFileModel;

const QString QFileModel::PropertyFileName = "fileName";
const QString QFileModel::PropertyDisplayName = "displayName";
const QString QFileModel::PropertyPathName = "pathName";
const QString QFileModel::PropertyFileType = "fileType";
const QString QFileModel::PropertyFileSize = "fileSize";
const QString QFileModel::PropertyLocalSize = "localSize";
const QString QFileModel::PropertyTypeName = "typeName";
const QString QFileModel::PropertyDateTime = "dateTime";
const QString QFileModel::PropertyRemoteDateTime = "remoteDateTime";
const QString QFileModel::PropertyMeasureType = "measureType";
const QString QFileModel::PropertyMeasureMode = "measureMode";
const QString QFileModel::PropertyJobId = "jobId";
const QString QFileModel::PropertyLocation = "location";
const qint32 QFileModel::FileDateTimeRange = 15 * 1000;

QFileModel::QFileModel(QObject *parent) : QAbstractModel(parent)
{
    m_fileName = QSharedPointer<QStringAttribute>(new QStringAttribute(this));
    m_pathName = QSharedPointer<QStringAttribute>(new QStringAttribute(this));
    m_typeName = QSharedPointer<QStringAttribute>(new QStringAttribute(this));
    m_dateTime = QSharedPointer<QDateTimeAttribute>(new QDateTimeAttribute(this));
    m_remoteDateTime = QSharedPointer<QDateTimeAttribute>(new QDateTimeAttribute(
                           this));
    m_fileType = QSharedPointer<QNumericAttribute>(new QNumericAttribute(QVariant(
                     0), QVariant::UInt, this));
    m_fileSize = QSharedPointer<QNumericAttribute>(new QNumericAttribute(QVariant(
                     0), QVariant::UInt, this));
    m_localSize = QSharedPointer<QNumericAttribute>(new QNumericAttribute(QVariant(
                      0), QVariant::UInt, this));
    m_measureType = QSharedPointer<QStringAttribute>(new QStringAttribute(this));
    m_measureMode = QSharedPointer<QStringAttribute>(new QStringAttribute(this));
    m_displayName = QSharedPointer<QStringAttribute>(new QStringAttribute(this));
    m_jobId = QSharedPointer<QStringAttribute>(new QStringAttribute(this));
    m_location = QSharedPointer<QStringAttribute>(new QStringAttribute(this));
}

void QFileModel::initAttributes()
{
}

QString QFileModel::fileName() const
{
    return m_fileName->value().toString();
}

void QFileModel::setFileName(QString fileName)
{
    m_fileName->setValue(fileName);
}

QString QFileModel::displayName() const
{
    return m_displayName->value().toString();
}
void QFileModel::setDisplayName(QString displayName)
{
    m_displayName->setValue(displayName);
}

QString QFileModel::pathName() const
{
    return m_pathName->value().toString();
}
void QFileModel::setPathName(QString pathName)
{
    m_pathName->setValue(pathName);
}

QString QFileModel::typeName() const
{
    return m_typeName->value().toString();
}

void QFileModel::setTypeName(QString typeName)
{
    m_typeName->setValue(typeName);
}

QDateTime QFileModel::dateTime() const
{
    return m_dateTime->value().toDateTime();
}

void QFileModel::setDateTime(QDateTime dateTime)
{
    m_dateTime->setValue(dateTime);
}

QDateTime QFileModel::remoteDateTime() const
{
    return m_remoteDateTime->value().toDateTime();
}

void QFileModel::setRemoteDateTime(QDateTime dateTime)
{
    m_remoteDateTime->setValue(dateTime);
}

quint32 QFileModel::fileType() const
{
    return m_fileType->value().toUInt();
}

void QFileModel::setFileType(quint32 fileType)
{
    m_fileType->setValue(fileType);
}

quint32 QFileModel::fileSize() const
{
    return m_fileSize->value().toUInt();
}

void QFileModel::setFileSize(quint32 fileSize)
{
    m_fileSize->setValue(fileSize);
}

quint32 QFileModel::localSize() const
{
    return m_localSize->value().toUInt();
}

void QFileModel::setLocalSize(quint32 localSize)
{
    m_localSize->setValue(localSize);
}

QString QFileModel::measureType() const
{
    return m_measureType->value().toString();
}

void QFileModel::setMeasureType(QString measureType)
{
    m_measureType->setValue(measureType);
}

QString QFileModel::measureMode() const
{
    return m_measureMode->value().toString();
}

void QFileModel::setMeasureMode(QString measureMode)
{
    m_measureMode->setValue(measureMode);
}

QString QFileModel::jobId() const
{
    return m_jobId->value().toString();
}
void QFileModel::setJobId(QString jobId)
{
    m_jobId->setValue(jobId);
}

QString QFileModel::location() const
{
    return m_location->value().toString();
}
void QFileModel::setLocation(QString location)
{
    m_location->setValue(location);
}

bool QFileModel::isStatusNew()
{
    return (dateTime().addMSecs(FileDateTimeRange) < remoteDateTime()
            || localSize() != fileSize());
}

