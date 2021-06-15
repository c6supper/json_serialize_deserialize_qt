
#include "qabstractcompatibility.h"
#include <QMutexLocker>
#include <QMetaProperty>

using namespace QProfile;

QAbstractCompatibility *QAbstractCompatibility::m_instance = nullptr;
QMutex QAbstractCompatibility::m_mutex(QMutex::NonRecursive);

QAbstractCompatibility::QAbstractCompatibility(QObject *parent)
    : QAbstractJsonElement(parent)
{
}

QAbstractCompatibility::~QAbstractCompatibility()
{
}

QAbstractCompatibility *QAbstractCompatibility::Instance()
{
    if (m_instance == nullptr) {
        qprofileDebug(QtFatalMsg) <<
                                  "You haven't set instance before calling this function.";
    }

    return m_instance;
}

const qint32 QAbstractCompatibility::GetCompatibilityMetaType()
{
    if (m_instance == nullptr) {
        return InvalidCompatibilityMetaType;
    }

    return QMetaType::type(m_instance->metaObject()->className());
}

const QString QAbstractCompatibility::GetCompatibilityClassName()
{
    if (m_instance == nullptr) {
        return "";
    }

    return QString(m_instance->metaObject()->className());
}
void QAbstractCompatibility::SetInstance(QAbstractCompatibility *instance)
{
    QMutexLocker locker(&m_mutex);

    if (m_instance != nullptr) {
        delete m_instance;
    }

    m_instance = instance;
    m_instance->initialize();
}

bool QAbstractCompatibility::isCompatible(const QString feature)
{
    QMutexLocker locker(&m_mutex);
    QVariant valueVar = property(feature.toStdString().c_str());

    if (!valueVar.isNull() && valueVar.isValid()
        && valueVar.canConvert(QVariant::Bool)) {
        return valueVar.toBool();
    }

    qprofileDebug(QtWarningMsg) << "Not supported feature " << feature <<
                                " value = " << valueVar;
    return false;
}
