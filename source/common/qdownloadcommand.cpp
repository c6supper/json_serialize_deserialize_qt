#include "qdownloadcommand.h"
#include "qobjectfactory.h"

using namespace QRserver;

QObjectSelfRegisteration<QDownloadCommand> registerQDownloadCommand;

QDownloadCommand::QDownloadCommand(QObject *parent)
    : QUploadCommand(parent)
{
}

QDownloadCommand::QDownloadCommand(QObject *parent, const QString &method,
                                   const QVariant &fileModelList, const QString &topic)
    : QUploadCommand(parent, method, fileModelList, topic)
{
}

QDownloadCommand::~QDownloadCommand()
{
}

