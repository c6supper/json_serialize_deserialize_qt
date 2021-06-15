
#include "qexecutorfactory.h"
#include "qabstractremotecommand.h"

#if defined(BUILD_X86)
#include "qjson/parser.h"
#include "qjson/serializer.h"
#include "qjson/qobjecthelper.h"
#else
#include <QJson/Parser>
#include <QJson/Serializer>
#include <QJson/QObjectHelper>
#endif

using namespace QProfile;
using namespace QRserver;

QExecutorFactory *QExecutorFactory::instance = nullptr;

void *QExecutorFactory::create(const QVariant var)
{
    QString executor = QAbstractRemoteCommand::GetExecutor(var);

    if (executor.isEmpty() || executor.isNull()) {
        qprofileDebug(QtDebugMsg) << "Invalid message variant = " << var;
        return nullptr;
    }

    if (!QExecutorFactory::Instance()->isExecutorRegistered(executor)) {
        qprofileDebug(QtDebugMsg) << "executor =  " << executor <<
                                  " haven't been registered before!";
        return nullptr;
    }

    qint32 metaId = QExecutorFactory::Instance()->executor2Id(executor);

    if (metaId == QMetaType::Void) {
        qprofileDebug(QtDebugMsg) << "Tried to create unregistered executor =  " <<
                                  executor;
        return nullptr;
    }

    void *message = QMetaType::construct(metaId);

    if (message != nullptr) {
        QJson::QObjectHelper::qvariant2qobject(var.toMap(), (QObject *)message);
        return message;
    }

    qprofileDebug(QtDebugMsg) << "Failed to load executor =  " << executor;
    QMetaType::destroy(metaId, message);
    return nullptr;
}

bool QExecutorFactory::isExecutorRegistered(QString executor)
{
    return m_executor2MetaTypeMap.contains(executor);
}

qint32 QExecutorFactory::executor2Id(QString executor)
{
    if (!isExecutorRegistered(executor))
        return QMetaType::Void;

    return m_executor2MetaTypeMap[executor];
}

QExecutorFactory *QExecutorFactory::Instance()
{
    if (QExecutorFactory::instance == nullptr)
        QExecutorFactory::instance = new QExecutorFactory();

    return instance;
}

