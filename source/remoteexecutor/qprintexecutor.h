
#ifndef QPRINTEXECUTOR_H
#define QPRINTEXECUTOR_H

#include "qabstractexecutor.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

using namespace QProfile;

namespace QRserver
{
    class QPrintExecutor : public QAbstractExecutor
    {
            Q_OBJECT

        public:
            QPrintExecutor(QObject *parent = nullptr);
            QPrintExecutor(const QPrintExecutor &executor);
            ~QPrintExecutor();
            virtual int exec(QAbstractRemoteCommand *cmd);

        protected:
            virtual bool isFinished();
    };
}

Q_DECLARE_METATYPE(QRserver::QPrintExecutor)

#endif // QPRINTEXECUTOR_H

