#ifndef QDOWNLOADCOMMAND_H
#define QDOWNLOADCOMMAND_H

#include "quploadcommand.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

using namespace QProfile;

namespace QRserver
{
    class QDownloadCommand : public QUploadCommand
    {
            Q_OBJECT

        public:
            QDownloadCommand(QObject *parent = nullptr);
            QDownloadCommand(QObject *parent, const QString &method,
                             const QVariant &fileModelList, const QString &topic = "");
            ~QDownloadCommand();
    };
}

Q_DECLARE_METATYPE(QRserver::QDownloadCommand)

#endif // QDOWNLOADCOMMAND_H
