
#include "qcmdfactory.h"
#include "qabstractremotecommand.h"
#include "qcmdparser.h"

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

QCMDFactory *QCMDFactory::instance = nullptr;

void *QCMDFactory::create(const QVariant var)
{
    QString cmd = QAbstractRemoteCommand::GetCMD(var);

    if (cmd.isEmpty() || cmd.isNull()) {
        qprofileDebug(QtDebugMsg) << "Invalid variant = " << var;
        return nullptr;
    }

    /* load cmd info from profile by cmd. */
    QVariant cmdInfo = QCMDParser::GetCMDInfo(cmd);

    if (cmdInfo.isNull()) {
        qprofileDebug(QtDebugMsg) << "get cmd info failed, variant =" << var;
        return nullptr;
    }

    if (!QCMDFactory::Instance()->isCMDRegistered(cmd)) {
        qprofileDebug(QtDebugMsg) << "cmd =  " << cmd <<
                                  " haven't been registered before!";
        return nullptr;
    }

    qint32 metaId = QCMDFactory::Instance()->cmd2Id(cmd);

    if (metaId == QMetaType::Void) {
        qprofileDebug(QtDebugMsg) << "Tried to create unregistered cmd =  " <<
                                  cmd;
        return nullptr;
    }

    void *message = QMetaType::construct(metaId);

    if (message != nullptr) {
        /* cmdInfo: cmd info from profile */
        QJson::QObjectHelper::qvariant2qobject(cmdInfo.toMap(), (QObject *)message);
        /* var: cmd info from argu */
        QJson::QObjectHelper::qvariant2qobject(var.toMap(), (QObject *)message);
        return message;
    }

    qprofileDebug(QtDebugMsg) << "Failed to load CMD =  " << cmd;
    QMetaType::destroy(metaId, message);
    return nullptr;
}

bool QCMDFactory::IsCMDRegistered(QString cmd)
{
    return QCMDFactory::Instance()->isCMDRegistered(cmd);
}

bool QCMDFactory::isCMDRegistered(QString cmd)
{
    return m_cmd2MetaTypeMap.contains(cmd);
}

qint32 QCMDFactory::cmd2Id(QString cmd)
{
    if (!isCMDRegistered(cmd))
        return QMetaType::Void;

    return m_cmd2MetaTypeMap[cmd];
}

QCMDFactory *QCMDFactory::Instance()
{
    if (QCMDFactory::instance == nullptr)
        QCMDFactory::instance = new QCMDFactory();

    return instance;
}

