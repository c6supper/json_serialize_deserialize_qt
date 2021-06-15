/*
 * qrserverdownloadprocess.h
 *
 *  Created on: Nov 19, 2020
 *      Author: duyi
 */

#ifndef QRSERVERDOWNLOADPROCESS_H
#define QRSERVERDOWNLOADPROCESS_H
#include "qdownloadprocess.h"

namespace QRserver
{
    class QRserverDownloadProcess: public QDownloadProcess
    {
            Q_OBJECT
        public:
            QRserverDownloadProcess(QObject *parent = nullptr);
            ~QRserverDownloadProcess();
            virtual bool wget(const QString url, const QString saveDirectory,
                              const QString fileName, qint64 fileSize = -1, const QString md5 = QString());
        protected slots:
            virtual void checkFinished(int exitCode, QProcess::ExitStatus exitStatus);
    };
}


#endif /* QRSERVERDOWNLOADPROCESS_H */
