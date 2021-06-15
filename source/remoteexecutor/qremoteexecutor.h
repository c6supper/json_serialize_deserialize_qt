
#ifndef QREMOTEEXECUTOR_H
#define QREMOTEEXECUTOR_H

#include <QObject>
#include <QPointer>
#include "qprofile_global.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QRserver
{
    class QRemoteExecutor: public QObject
    {
            Q_OBJECT

        public:
            QRemoteExecutor(QObject *parent = nullptr);
            ~QRemoteExecutor();

            int executor();

        private:
            const QVariant Argument();
    };
}

#endif /* QREMOTEEXECUTOR_H */
