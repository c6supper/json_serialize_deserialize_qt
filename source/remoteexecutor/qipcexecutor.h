
#ifndef QIPCEXECUTOR_H
#define QIPCEXECUTOR_H

#include "qabstractipcclient.h"
#include "qabstractexecutor.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

using namespace QProfile;

namespace QRserver
{
    class QIpcExecutor : public QAbstractExecutor
    {
            Q_OBJECT

        public:
            QIpcExecutor(QObject *parent = nullptr);
            QIpcExecutor(const QIpcExecutor &executor);
            ~QIpcExecutor();

            virtual int exec(QAbstractRemoteCommand *cmd);

        protected:
            QAbstractIpcClient *m_ipcClient;

        private:
            quint32 waitToConnected(int timeout);
            int toOutput(const QString &output, const QVariant &var);
            QAbstractExecutor::RetCode getEngineStatus();

        protected:
            virtual bool isFinished();
    };
}

Q_DECLARE_METATYPE(QRserver::QIpcExecutor)

#endif // QIPCEXECUTOR_H

