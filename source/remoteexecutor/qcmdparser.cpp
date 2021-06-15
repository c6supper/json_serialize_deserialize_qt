
#include <QCoreApplication>
#include <QFile>
#include "qcmdparser.h"
#include "qrserverenvironment.h"
#include "qabstractremotecommand.h"

#if defined(BUILD_X86)
#include "qjson/parser.h"
#include "qjson/qobjecthelper.h"
#else
#include <QJson/Parser>
#include <QJson/QObjectHelper>
#endif

using namespace QProfile;
using namespace QRserver;

QCMDParser *QCMDParser::m_instance = nullptr;

QCMDParser::QCMDParser(QObject *parent)
    : QObject(parent)
{
}

QCMDParser::~QCMDParser()
{
}

QVariantList QCMDParser::cmdList()
{
    return m_cmdList;
}

void QCMDParser::setCmdList(QVariantList &cmdList)
{
    m_cmdList = cmdList;
}

const QVariant QCMDParser::getCMDInfo(const QString cmd)
{
    QMutexLocker locker(&m_mutex);
    QVariant cmdInfo;

    foreach (QVariant var, m_cmdList) {
        if (QAbstractRemoteCommand::GetCMD(var) == cmd) {
            cmdInfo = var;
            break;
        }
    }

    return cmdInfo;
}

QCMDParser *QCMDParser::instance()
{
    if (m_instance == nullptr) {
        m_instance = new QCMDParser(QCoreApplication::instance());
        QFile file(QRserverEnvironment::RemoteExecutorCMDConfig());

        if (!file.exists()) {
            qprofileDebug(QtFatalMsg) << QRserverEnvironment::RemoteExecutorCMDConfig() <<
                                      " is missing!";
        }

        if (file.open(QIODevice::ReadOnly)) {
            QByteArray data = file.readAll();
            QJson::Parser parser;
            bool ok = false;
            QVariant variant = parser.parse(data, &ok);

            if (!ok) {
                qprofileDebug(QtFatalMsg) << "Failed to load remoteexecutorcmd.json!";
            }

            QJson::QObjectHelper::qvariant2qobject(variant.toMap(), m_instance);
            file.close();
        }
    }

    return m_instance;
}

const QVariant QCMDParser::GetCMDInfo(const QString cmd)
{
    return instance()->getCMDInfo(cmd);
}

