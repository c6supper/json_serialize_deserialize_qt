#ifndef QLOCKDOWNCOMMAND_H
#define QLOCKDOWNCOMMAND_H

#include "qabstractcommand.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

using namespace QProfile;

namespace QRserver
{
    class QLockDownCommand : public QAbstractCommand
    {
            Q_OBJECT

            Q_PROPERTY(bool lockDown READ lockDown WRITE setLockDown)

        public:
            QLockDownCommand(QObject *parent = nullptr);
            QLockDownCommand(QObject *parent, const QString &method,
                             const QVariant &bLock, const QString &topic = "");
            ~QLockDownCommand();

            virtual void pureVirtualMock() {};

            const bool lockDown() const;
            void setLockDown(const bool bLock);
    };
}

Q_DECLARE_METATYPE(QRserver::QLockDownCommand)

#endif // QLOCKDOWNCOMMAND_H
