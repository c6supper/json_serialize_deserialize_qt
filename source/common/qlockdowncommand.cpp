#include "qlockdowncommand.h"
#include "qobjectfactory.h"

using namespace QRserver;

QObjectSelfRegisteration<QLockDownCommand> registerQLockDownCommand;

QLockDownCommand::QLockDownCommand(QObject *parent)
    : QAbstractCommand(parent)
{
}

QLockDownCommand::QLockDownCommand(QObject *parent, const QString &method,
                                   const QVariant &bLock, const QString &topic)
    : QAbstractCommand(parent, topic, method, bLock)
{
}

QLockDownCommand::~QLockDownCommand()
{
}


const bool QLockDownCommand::lockDown() const
{
    return resource().toBool();
}

void QLockDownCommand::setLockDown(const bool bLock)
{
    setResource(QVariant(bLock));
}

