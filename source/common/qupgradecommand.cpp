#include "qupgradecommand.h"
#include "qobjectfactory.h"

using namespace QRserver;

QObjectSelfRegisteration<QUpgradeCommand> registerQUpgradeCommand;

QUpgradeCommand::QUpgradeCommand(QObject *parent)
    : QAbstractCommand(parent)
{
}

QUpgradeCommand::QUpgradeCommand(QObject *parent, const QString &method,
                                 const QVariant &deviceInfoList, const QString &topic)
    : QAbstractCommand(parent, topic, method, QVariant(deviceInfoList))
{
}

QUpgradeCommand::~QUpgradeCommand()
{
}

bool QUpgradeCommand::isValid()
{
    return (QAbstractCommand::isValid() && !resource().toList().isEmpty());
}

const QVariantList QUpgradeCommand::deviceInfoList() const
{
    return resource().toList();
}

void QUpgradeCommand::setDeviceInfoList(const QVariantList &deviceInfoList)
{
    setResource(QVariant(deviceInfoList));
}

