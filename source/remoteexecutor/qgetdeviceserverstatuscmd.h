
#ifndef QGETDEVICESERVERSTATUSCMD_H
#define QGETDEVICESERVERSTATUSCMD_H

#include "qabstractremotecommand.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QRserver
{
    class QGetDeviceServerStatusCMD: public QProfile::QAbstractRemoteCommand
    {
            Q_OBJECT

        public:
            QGetDeviceServerStatusCMD(QObject *parent = nullptr);
            QGetDeviceServerStatusCMD(const QGetDeviceServerStatusCMD &cmd);
            ~QGetDeviceServerStatusCMD();

            virtual const QVariant parseExecOutput(const QVariant &output);
            virtual const QVariant toExecInput();
    };
}

Q_DECLARE_METATYPE(QRserver::QGetDeviceServerStatusCMD)

#endif

