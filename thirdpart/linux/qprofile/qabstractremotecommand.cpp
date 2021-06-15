
#include "qabstractremotecommand.h"
#include <QMetaProperty>
#include <QFile>

#if defined(BUILD_X86)
#include "qjson/serializer.h"
#include "qjson/parser.h"
#include "qjson/qobjecthelper.h"
#else
#include <QJson/Serializer>
#include <QJson/Parser>
#include <QJson/QObjectHelper>
#endif

using namespace QProfile;

const QString QAbstractRemoteCommand::PropertyCMD = "cmd";
const QString QAbstractRemoteCommand::PropertyExecutor = "executor";
const QString QAbstractRemoteCommand::PropertyMethod = "method";

const QString QAbstractRemoteCommand::DefaultRemoteExecutorCMDProfileConfig =
    "remoteexecutorcmd.json";

const QStringList QAbstractRemoteCommand::ArguList = arguList();

const QStringList QAbstractRemoteCommand::propertyList()
{
    QStringList list;
    QStringList ignoredPropertiesList = QAbstractJsonElement::propertyList();

    for (int propertyIdx = 0;
         propertyIdx < QAbstractRemoteCommand::staticMetaObject.propertyCount();
         propertyIdx++) {
        QMetaProperty property = QAbstractRemoteCommand::staticMetaObject.property(
                                     propertyIdx);

        if (!ignoredPropertiesList.contains(property.name())) {
            list.append(property.name());
        }
    }

    return list;
}

const QStringList QAbstractRemoteCommand::arguList()
{
    QStringList arguList = QAbstractRemoteCommand::propertyList();
    arguList.removeOne(QAbstractRemoteCommand::PropertyExecutor);
    arguList.removeOne(QAbstractRemoteCommand::PropertyMethod);
    return arguList;
}

const QString QAbstractRemoteCommand::GetCMD(const QVariant &variant)
{
    QVariantMap cmdVariantMap = variant.toMap();
    return cmdVariantMap[QAbstractRemoteCommand::PropertyCMD].toString();
}

const QString QAbstractRemoteCommand::GetExecutor(const QVariant &variant)
{
    QVariantMap cmdVariantMap = variant.toMap();
    return cmdVariantMap[QAbstractRemoteCommand::PropertyExecutor].toString();
}

QAbstractRemoteCommand::QAbstractRemoteCommand(QObject *parent)
    : QAbstractJsonElement(parent)
{
}

QAbstractRemoteCommand::QAbstractRemoteCommand(const QAbstractRemoteCommand
        &cmd)
    : QAbstractJsonElement(cmd.parent())
{
    for (int propertyIdx = 0; propertyIdx < metaObject()->propertyCount();
         propertyIdx++) {
        QMetaProperty property = metaObject()->property(propertyIdx);
        int srcIdx = cmd.metaObject()->indexOfProperty(property.name());

        if (srcIdx > 0) {
            if (!setProperty(property.name(), cmd.property(property.name())))
                qprofileDebug(QtDebugMsg) << "set property " << property.name() <<
                                          "Failed, for class " << metaObject()->className();
        } else {
            qprofileDebug(QtDebugMsg) << "Miss property "  << property.name() <<
                                      ", for class" << metaObject()->className();
        }
    }
}

QString QAbstractRemoteCommand::cmd() const
{
    return m_cmd;
}
void QAbstractRemoteCommand::setCmd(QString cmd)
{
    m_cmd = cmd;
}

QString QAbstractRemoteCommand::input() const
{
    return m_input;
}
void QAbstractRemoteCommand::setInput(QString input)
{
    m_input = input;
}

QString QAbstractRemoteCommand::output() const
{
    return m_output;
}
void QAbstractRemoteCommand::setOutput(QString output)
{
    m_output = output;
}

QString QAbstractRemoteCommand::inTopic() const
{
    return m_inTopic;
}
void QAbstractRemoteCommand::setInTopic(QString inTopic)
{
    m_inTopic = inTopic;
}

QString QAbstractRemoteCommand::outTopic() const
{
    return m_outTopic;
}
void QAbstractRemoteCommand::setOutTopic(QString outTopic)
{
    m_outTopic = outTopic;
}

quint32 QAbstractRemoteCommand::timeout() const
{
    return m_timeout;
}
void QAbstractRemoteCommand::setTimeout(quint32 timeout)
{
    m_timeout = timeout;
}

bool QAbstractRemoteCommand::isCache() const
{
    return m_isCache;
}
void QAbstractRemoteCommand::setIsCache(bool isCache)
{
    m_isCache = isCache;
}

QString QAbstractRemoteCommand::executor() const
{
    return m_executor;
}
void QAbstractRemoteCommand::setExecutor(QString executor)
{
    m_executor = executor;
}

QString QAbstractRemoteCommand::method() const
{
    return m_method;
}
void QAbstractRemoteCommand::setMethod(const QString method)
{
    m_method = method;
}

bool QAbstractRemoteCommand::isValid()
{
    return (!m_cmd.isEmpty() && !m_inTopic.isEmpty()
            && !m_outTopic.isEmpty() && !m_executor.isEmpty() && !m_method.isEmpty());
}

const QVariant QAbstractRemoteCommand::parseExecOutput(const QVariant &output)
{
    return QVariant();
}

const QVariant QAbstractRemoteCommand::toExecInput()
{
    return QVariant();
}

QAbstractRemoteCommand::~QAbstractRemoteCommand()
{
}

void QAbstractRemoteCommand::pureVirtualMock()
{
}
