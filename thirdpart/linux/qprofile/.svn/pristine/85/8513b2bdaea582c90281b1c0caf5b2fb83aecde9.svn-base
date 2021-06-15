
#ifndef QABSTRACTEXECUTOR_H
#define QABSTRACTEXECUTOR_H

#include "qprofile_export.h"
#include "qprofile_debug.h"
#include "qprofile_global.h"
#include <QObject>
#include "qabstractjsonelement.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE
namespace QProfile
{
    class QAbstractRemoteCommand;
}

namespace QProfile
{
    class QAbstractExecutor : public QAbstractJsonElement
    {
            Q_OBJECT

            Q_ENUMS(RetCode)

        public:
            enum RetCode { Succeed = 0, ArgumentError, Failed, ClientClosed,
                           NetworkError, IPCError, Timeout,
                           OutputError, InvalidCommand, CustomError, EngineIsDead,
                           NotChecked, Unregistered,
                           RegisteredUnauthorized, UnknownError
                         };

        public:
            QAbstractExecutor(QObject *parent = nullptr);
            QAbstractExecutor(const QAbstractExecutor &executor);
            ~QAbstractExecutor();
            virtual void pureVirtualMock();

            bool saveFile(const QString &pathName, const QByteArray &json);
            virtual int exec(QAbstractRemoteCommand *cmd) = 0;

        protected:
            virtual bool isFinished() = 0;
    };
}

#endif // QABSTRACTEXECUTOR_H

