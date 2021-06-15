/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QREGISTER_H
#define QREGISTER_H

#include "qregistermodel.h"
#include "qrservercontainer.h"
#include "qstringattribute.h"
#include <QPointer>
#include "qregisterstatus.h"

using namespace QProfile;

namespace QRserver
{
    class QRegister: public QRserverContainer
    {
            Q_OBJECT

        public:
            QRegister(QObject *parent = nullptr);

        protected slots:
            virtual bool initPropertiesMap(const QVariantMap &properties =
                                               QVariantMap());
            virtual	void allocModel();
            void serverIpChanged();

        protected:
            virtual void processMessage(QRserver::QAbstractBinMsg *message);
            virtual bool processCommands(const QString &commandId,
                                         const QString &commandType, QVariant commandVar,
                                         QVariant *status = nullptr);

        private:
            const QVariant registerDevice(const QVariant argsVar);
            const QAbstractStatus::CommandStatus registerSingleDevice(
                const QVariant deviceVar);
            const QVariant unregisterDevice(const QVariant argsVar);
            const QAbstractStatus::CommandStatus unregisterSingleDevice(
                const QVariant deviceVar);
            const QVariant checkDevice(const QVariant argsVar);
            const QAbstractStatus::CommandStatus checkSingleDevice(
                const QVariant deviceVar);
            const QVariant upateDeviceInfo(const QVariant argsVar);
            const QAbstractStatus::CommandStatus upateChassisDeviceInfo(
                const QVariant deviceVar);
            void updateCheckAllInfo(QAbstractBinMsg *message);
            void updateRegistrationBomb(QAbstractBinMsg *message);
            void updateLockVeexpressUpgrade(QAbstractBinMsg *message);
            void updateBlockProfileAndChangeTechName(QAbstractBinMsg *message);
            void updateTechNameResult(QAbstractBinMsg *message);
            void updateDeviceInfoStatus(QAbstractBinMsg *message);
            void onUnregister(QAbstractBinMsg *message);
            const QVariant getServerVersion(const QVariant argsVar);
            const QVariant updateTechName(const QVariant argsVar);
            const QAbstractStatus::CommandStatus sendUpdateTechNameMsg(
                const QVariant deviceVar);
            bool isNeedSettingAdminProfile();

        private:
            QPointer<QRegisterModel>	m_registerModel;
            quint8 						m_updateDeviceInfoStatus;
            bool                        m_updataTechNameResult;
    };
}

Q_DECLARE_METATYPE(QRserver::QRegister)

#endif
