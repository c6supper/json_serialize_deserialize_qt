
#include "qresultmodel.h"
#include "qobjectfactory.h"

using namespace QProfile;
using namespace QRserver;

QObjectSelfRegisteration<QResultModel> registerQResultModel;
#if 0
const QString QResultModel::PropertyFileName = "fileName";
const QString QResultModel::PropertyDisplayName = "displayName";
const QString QResultModel::PropertyPathName = "pathName";
const QString QResultModel::PropertyFileType = "fileType";
const QString QResultModel::PropertyFileSize = "fileSize";
const QString QResultModel::PropertyLocalSize = "localSize";
const QString QResultModel::PropertyTypeName = "typeName";
const QString QResultModel::PropertyDateTime = "dateTime";
const QString QResultModel::PropertyRemoteDateTime = "remoteDateTime";
const QString QResultModel::PropertyMeasureType = "measureType";
const QString QResultModel::PropertyMeasureMode = "measureMode";
const qint32 QResultModel::FileDateTimeRange = 15 * 1000;
#endif
QResultModel::QResultModel(QObject *parent) : QAbstractModel(parent)
{
    m_displayName = QSharedPointer<QStringAttribute>(new QStringAttribute(this));
    m_fileType = QSharedPointer<QNumericAttribute>(new QNumericAttribute(QVariant(
                     0), QVariant::UInt, this));
    m_typeName = QSharedPointer<QStringAttribute>(new QStringAttribute(this));
    m_dateTime = QSharedPointer<QDateTimeAttribute>(new QDateTimeAttribute(this));
    //m_relatedFileList
}

void QResultModel::initAttributes()
{
}

QString QResultModel::displayName() const
{
    return m_displayName->value().toString();
}
void QResultModel::setDisplayName(QString displayName)
{
    m_displayName->setValue(displayName);
}

quint32 QResultModel::fileType() const
{
    return m_fileType->value().toUInt();
}

void QResultModel::setFileType(quint32 fileType)
{
    m_fileType->setValue(fileType);
}

QString QResultModel::typeName() const
{
    return m_typeName->value().toString();
}

void QResultModel::setTypeName(QString typeName)
{
    m_typeName->setValue(typeName);
}

QDateTime QResultModel::dateTime() const
{
    return m_dateTime->value().toDateTime();
}

void QResultModel::setDateTime(QDateTime dateTime)
{
    m_dateTime->setValue(dateTime);
}

QStringList QResultModel::relatedFileList() const
{
    return m_relatedFileList;
}
void QResultModel::setRelatedFileList(QStringList relatedFileList)
{
    m_relatedFileList = relatedFileList;
}

