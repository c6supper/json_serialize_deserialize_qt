
#ifndef QGETRESULTCMD_H
#define QGETRESULTCMD_H

#include "qabstractremotecommand.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QRserver
{
    class QGetResultCMD: public QProfile::QAbstractRemoteCommand
    {
            Q_OBJECT

        public:
            QGetResultCMD(QObject *parent = nullptr);
            QGetResultCMD(const QGetResultCMD &cmd);
            ~QGetResultCMD();

            virtual bool isValid();
            virtual const QVariant parseExecOutput(const QVariant &output);
            virtual const QVariant toExecInput();
    };
}

Q_DECLARE_METATYPE(QRserver::QGetResultCMD)

#endif

