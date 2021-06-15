#ifndef QUPLOADCOMMAND_H
#define QUPLOADCOMMAND_H

#include "qabstractcommand.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

using namespace QProfile;

namespace QRserver
{
    class QUploadCommand : public QAbstractCommand
    {
            Q_OBJECT

            Q_PROPERTY(QVariantList fileModelList READ fileModelList WRITE setFileModelList)

        public:
            QUploadCommand(QObject *parent = nullptr);
            QUploadCommand(QObject *parent, const QString &method,
                           const QVariant &fileModelList, const QString &topic = "");
            ~QUploadCommand();

            virtual bool isValid();
            virtual void pureVirtualMock() {};

            const QVariantList fileModelList() const;
            void setFileModelList(const QVariantList &fileModelList);
    };
}

Q_DECLARE_METATYPE(QRserver::QUploadCommand)

#endif // QUPLOADCOMMAND_H
