#ifndef QSOFTWARE_H
#define QSOFTWARE_H

#include "qrservercontainer.h"
#include "qsoftwaremodel.h"
#include "qrserversoftwareclient.h"
#include <QTimer>
#include <QElapsedTimer>
#include "qrserverdownloadprocess.h"

namespace QRserver
{
    class QSoftware: public QRserverContainer
    {
            Q_OBJECT

        public:
            QSoftware(QObject *parent = nullptr);
            static void rserverMessageCallback(void *obj, unsigned int type,
                                               const unsigned char *data, unsigned int length);
            const QVariant getUpgradeInfo();

        private slots:
            void timeout();
            void onReportHttpDownloadStatus();

        protected slots:
            virtual bool initPropertiesMap(const QVariantMap &properties = QVariantMap());
            virtual	void allocModel();

        protected:
            virtual void processMessage(QRserver::QAbstractBinMsg *message);
            virtual bool processCommands(const QString &commandId,
                                         const QString &commandType, QVariant commandVar,
                                         QVariant *status = nullptr);

        private:
            void setUpgradeInfoStatus(const QString serialNumber, const quint32 status);
            bool upgrade(const QVariant argsVar);
            bool saveSoftware(QAbstractBinMsg *message);
            void executeDeltaUpgrade(const QStringList &upgradeParam);
            bool isUpgrading();
            const QString getDeltaImagePathName(const QString serialNumber);
            void onUpgradeStop(const QString &serialNumber, qint32 status);
            qint64 getFreeSpace(const QString &dir);
            bool onTestSwInfo(QAbstractBinMsg *message);
            bool downloadPacViaHttp(QAbstractBinMsg *message);

        private:
            QSoftwareModel 				*m_softwareModel;
            QRserverSoftwareClient 		*m_rserverSoftwareClient;
            QTimer						*m_upgradeTimer;
            QElapsedTimer				m_reportTimer;
            QRserverDownloadProcess           *m_download;
            QTimer                      *m_httpDownloadReportTimer;
    };
}

Q_DECLARE_METATYPE(QRserver::QSoftware)

#endif /* QSOFTWARE_H */
