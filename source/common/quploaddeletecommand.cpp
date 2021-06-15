#include "quploaddeletecommand.h"
#include "qobjectfactory.h"

using namespace QRserver;

QObjectSelfRegisteration<QUploadDeleteCommand> registerQUploadDeleteCommand;

QUploadDeleteCommand::QUploadDeleteCommand(QObject *parent)
    : QUploadCommand(parent)
{
}

QUploadDeleteCommand::QUploadDeleteCommand(QObject *parent,
        const QString &method, const QVariant &fileModelList, const QString &topic)
    : QUploadCommand(parent, method, fileModelList, topic)
{
}

QUploadDeleteCommand::~QUploadDeleteCommand()
{
}


