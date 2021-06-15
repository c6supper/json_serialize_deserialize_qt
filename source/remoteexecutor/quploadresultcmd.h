
#ifndef QUPLOADRESULTCMD_H
#define QUPLOADRESULTCMD_H

#include "qabstractremotecommand.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QRserver
{
    class QFileModel;
}

namespace QRserver
{
    class QUploadResultCMD: public QProfile::QAbstractRemoteCommand
    {
            Q_OBJECT

        public:
            QUploadResultCMD(QObject *parent = nullptr);
            QUploadResultCMD(const QUploadResultCMD &cmd);
            ~QUploadResultCMD();

            virtual bool isValid();
            virtual const QVariant parseExecOutput(const QVariant &output);
            virtual const QVariant toExecInput();
            virtual void setInput(QString input);

        private:
            QFileModel	*m_fileModel;
    };
}

Q_DECLARE_METATYPE(QRserver::QUploadResultCMD)

#endif

