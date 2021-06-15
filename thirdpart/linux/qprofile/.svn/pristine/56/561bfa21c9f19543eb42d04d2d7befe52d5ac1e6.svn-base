/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QABSTRACTTOPIC_H
#define QABSTRACTTOPIC_H

#include <QObject>
#include <QPointer>
#include <QStringList>
#include <QFlags>
#include "qprofile_global.h"

QT_BEGIN_NAMESPACE
namespace QProfile
{
    class QAbstractIpcClient;
}
QT_END_NAMESPACE

namespace QProfile
{
    class QAbstractTopic : public QObject
    {
            Q_OBJECT

        public:
            enum RestfulApi { get = 0x1, put = 0x2, command = 0x4, status = 0x8, trigger = 0x10, response = 0x20};
            Q_DECLARE_FLAGS(RestfulApis, RestfulApi)
            static const QString DefaultPutTopic;
            static const QString DefaultGetTopic;
            static const QString DefaultCommandTopic;
            static const QString DefaultStatusTopic;
            static const QString DefaultTriggerTopic;
            static const QString DefaultResponseTopic;

            explicit QAbstractTopic(QObject *parent, QAbstractIpcClient *ipcClient,
                                    const QString &relativeTopic = "",
                                    RestfulApis api = put);
            ~QAbstractTopic();

            static const QString relativeTopic(const QString &absoluteTopic);
            const QString getRestfulTopic(const RestfulApi api);

        protected:
            virtual const QString absoluteTopic(const QString &relativeTopic) = 0;

        private slots:
            void onConnected();
            void onSubscribed(const QString &);

        signals:
            void subscribeFinished();

        protected:
            QString							m_relativeTopic;
            QPointer<QAbstractIpcClient>	m_ipcClient;
            QStringList						m_subscriberdTopic;
            RestfulApis						m_api;
    };
}
Q_DECLARE_OPERATORS_FOR_FLAGS(QProfile::QAbstractTopic::RestfulApis)
#endif // QABSTRACTTOPIC_H
