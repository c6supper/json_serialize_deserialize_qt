#include "qabstracttrigger.h"
#include <QMetaProperty>
#include <QTimer>

using namespace QProfile;

const QString QAbstractTrigger::PropertyId = "id";
const QString QAbstractTrigger::PropertyType = "type";
const QString QAbstractTrigger::PropertyResource = "resource";
const QString QAbstractTrigger::DefaultTriggerTopic = "trigger";
const QString QAbstractTrigger::DefaultResponseTopic = "response";
const quint32 QAbstractTrigger::DefaultTriggerTimeout = 60 * 1000;

QAbstractTrigger::QAbstractTrigger(QObject *parent, const QVariant resource)
    : QAbstractCommand(parent, "", "", resource)
{
    QTimer::singleShot(0, this, SLOT(initAttributes()));
}

QAbstractTrigger::QAbstractTrigger(const QAbstractTrigger &trigger)
    : QAbstractCommand(trigger.parent())
{
    for (int propertyIdx = 0; propertyIdx < metaObject()->propertyCount();
         propertyIdx++) {
        QMetaProperty property = metaObject()->property(propertyIdx);
        int srcIdx = trigger.metaObject()->indexOfProperty(property.name());

        if (srcIdx > 0) {
            if (!setProperty(property.name(), trigger.property(property.name())))
                qprofileDebug(QtDebugMsg) << "set property " << property.name() <<
                                          "Failed, for class " << metaObject()->className();
        } else {
            qprofileDebug(QtDebugMsg) << "Miss property "  << property.name() <<
                                      ", for class" << metaObject()->className();
        }
    }
}

QAbstractTrigger::~QAbstractTrigger()
{
}

void QAbstractTrigger::initAttributes()
{
    setType(metaObject()->className());
}

bool QAbstractTrigger::isValid()
{
    return (!id().isEmpty() && !type().isEmpty()
            && !triggerTopic().isEmpty() && !responseTopic().isEmpty());
}

const quint32 QAbstractTrigger::timeout()
{
    return DefaultTriggerTimeout;
}

bool QAbstractTrigger::IsTrigger(const QVariant &variant)
{
    QVariantMap varMap = variant.toMap();
    return varMap.contains(QAbstractTrigger::PropertyType);
}

