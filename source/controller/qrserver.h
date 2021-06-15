/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QRSERVER_H
#define QRSERVER_H

#include "qrservercontainer.h"
#include "qregister.h"
#include "qrservermodel.h"
#include "qabstractbinmsg.h"
#include "qupload.h"
#include "qdownload.h"
#include "qsoftware.h"
#include <QTimer>
#include "qvconnectmodel.h"

using namespace QProfile;

namespace QRserver
{
    class QRserver: public QRserverContainer
    {
            Q_OBJECT

        public:
            QRserver(QObject *parent = nullptr);

        private:
            void setLocalDateTime(const QDateTime dateTime);
            void updateCheckAllInfo(QAbstractBinMsg *message);
            const QVariant updateVConnectInfo(QVariant argsVar);
            const QAbstractStatus::CommandStatus updateVConnectInfo2Server(
                const QVariant VConnectInfoVar);
            const QVariant pong();
            void getServerVersion(QAbstractBinMsg *message);
            void getDeviceVersion(QAbstractBinMsg *message);
            void updateTestsetStatus(QAbstractBinMsg *message);

        protected slots:
            virtual bool initPropertiesMap(const QVariantMap &properties =
                                               QVariantMap());
            virtual	void allocModel();
            void onCheckNetwork();
            void onUpdateVconnectInfo();
            void onRServerModeChanged(qint32 rserverMode);

        protected:
            virtual void processMessage(QAbstractBinMsg *message);
            virtual bool processCommands(const QString &commandId,
                                         const QString &commandType, QVariant commandVar,
                                         QVariant *status = nullptr);
            void onRegister(QAbstractBinMsg *message);
            void onUnregister(QAbstractBinMsg *message);
            virtual void postponeInit();

        private slots:
            void onSubscribeFinished();

        private:
            QSharedPointer<QRegister> 			m_register;
            QSharedPointer<QUpload> 			m_upload;
            QSharedPointer<QDownload> 			m_download;
            QSharedPointer<QSoftware> 			m_software;
            QPointer<QRserverModel>				m_rserverModel;
            QTimer								*m_networkCheckTimer;
            QTimer                              *updateVconnectInfoTimer;
            QVConnectModel						*m_vconnectModel;

        private:
            static const quint32 checkNetworkInterval = 15 * 1000;
            static const quint32 networkcheckTimeout = 10 * 1000;
            static const quint32 updateVconnectInfoInterval = 30 * 1000;
    };
}

Q_DECLARE_METATYPE(QRserver::QRserver)

#endif
