
#ifndef QPRINTRESULTKEYFILECMD_H
#define QPRINTRESULTKEYFILECMD_H

#include "qabstractremotecommand.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QRserver
{
    class QPrintResultKeyFileCMD: public QProfile::QAbstractRemoteCommand
    {
            Q_OBJECT

        public:
            QPrintResultKeyFileCMD(QObject *parent = nullptr);
            QPrintResultKeyFileCMD(const QPrintResultKeyFileCMD &cmd);
            ~QPrintResultKeyFileCMD();

            virtual const QVariant toExecInput();
            virtual bool isValid();
    };
}

Q_DECLARE_METATYPE(QRserver::QPrintResultKeyFileCMD)

#endif

