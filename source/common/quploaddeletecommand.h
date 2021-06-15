#ifndef QUPLOADDELETECOMMAND_H
#define QUPLOADDELETECOMMAND_H

#include "quploadcommand.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

using namespace QProfile;

namespace QRserver
{
    class QUploadDeleteCommand : public QUploadCommand
    {
            Q_OBJECT

        public:
            QUploadDeleteCommand(QObject *parent = nullptr);
            QUploadDeleteCommand(QObject *parent, const QString &method,
                                 const QVariant &fileModelList, const QString &topic = "");
            ~QUploadDeleteCommand();
    };
}

Q_DECLARE_METATYPE(QRserver::QUploadDeleteCommand)

#endif // QUPLOADDELETECOMMAND_H
