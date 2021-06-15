/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QMESSAGESERIES_H
#define QMESSAGESERIES_H

#include <QObject>
#include <QVariant>
#include <QMap>
#include <QMutexLocker>
#include "qprofile_debug.h"
#include "qprofile_global.h"
#include "qabstractmodel.h"
#include "qdeviceinfo.h"
#include "qabstractbinmsg.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QRserver
{
    class QMessageSeries: public QProfile::QAbstractModel
    {
            Q_OBJECT

            Q_PROPERTY(QVariant device READ device WRITE setDevice)
            Q_PROPERTY(QVariantList messageList READ messageList WRITE setMessageList)
            Q_PROPERTY(QVariantList messageRouteList READ messageRouteList WRITE
                       setMessageRouteList)

        public:
            QMessageSeries(QObject *parent = nullptr);
            ~QMessageSeries();

            const QVariant device() const;
            void setDevice(const QVariant device);

            bool appendMessage(const quint32 parentMessageId, QAbstractBinMsg *message);
            void appendMessageRoute(QList<quint32> routeList);
            const QVariantList &messageList();
            void setMessageList(const QVariantList &messageList);
            const QVariantList &messageRouteList();
            void setMessageRouteList(const QVariantList &messageRouteList);

            const QMap<quint32, QVariantMap> 	getMessageMap();
            const QList<QList<quint32> > 		getMessageRouteList();
            bool isValid();
            const quint32						messageIdx();

        private:
            void initAttributes();
            bool relateMessage(quint32 parentMessageId, quint32 childMessageId);

        private:
            QDeviceInfo						m_device;
            QVariantList					m_messageList;
            QVariantList					m_messageRouteList;
            QMutex							m_mutex;
    };
}

#endif
