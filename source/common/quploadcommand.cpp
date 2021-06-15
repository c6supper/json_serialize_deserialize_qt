#include "quploadcommand.h"
#include "qobjectfactory.h"

using namespace QRserver;

QObjectSelfRegisteration<QUploadCommand> registerQUploadCommand;

QUploadCommand::QUploadCommand(QObject *parent)
    : QAbstractCommand(parent)
{
}

QUploadCommand::QUploadCommand(QObject *parent, const QString &method,
                               const QVariant &fileModelList, const QString &topic)
    : QAbstractCommand(parent, topic, method, fileModelList)
{
}

QUploadCommand::~QUploadCommand()
{
}

bool QUploadCommand::isValid()
{
    return (QAbstractCommand::isValid() && !resource().toList().isEmpty());
}

const QVariantList QUploadCommand::fileModelList() const
{
    return resource().toList();
}

void QUploadCommand::setFileModelList(const QVariantList &fileModelList)
{
    setResource(QVariant(fileModelList));
}

