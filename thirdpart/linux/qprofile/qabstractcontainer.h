/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QABSTRACTCONTAINER_H
#define QABSTRACTCONTAINER_H

#include "qprofile_export.h"
#include "qprofile_global.h"
#include <QMetaType>
#include <QObject>
#include <QPointer>
#include "qabstractparser.h"
#include "qabstractserializer.h"
#include "qabstractmodel.h"

QT_BEGIN_NAMESPACE
namespace QProfile
{
    class QAbstractIpcClient;
    class QAbstractTopic;
    class QGenericMessage;
}
QT_END_NAMESPACE

namespace QProfile
{
    class QAbstractContainer : public QObject
    {
            Q_OBJECT

        public:
            QAbstractContainer(QObject *parent = nullptr, bool isRetainMessage = true);
            QAbstractContainer(const QAbstractContainer &container);
            ~QAbstractContainer();
            QAbstractContainer &operator=(const QAbstractContainer &container);
            virtual bool deSerialize(const QByteArray &qProfileString,
                                     QAbstractParser *paser) = 0;
            virtual QVariantMap serialize(QAbstractSerializer *serializer, bool *ok);
            virtual const QString tagName(const QString &key);

            void setModel(QAbstractModel *model)
            {
                if (!m_model.isNull()) {
                    delete m_model.data();
                }

                m_model = model;
            }
            QAbstractModel *model()
            {
                return m_model.data();
            }

        protected:
            void copyProperty(const QAbstractContainer &container);
            virtual void copyChildren(const QAbstractContainer &container) = 0;
            virtual void customEvent(QEvent *event);
            virtual bool processCommands(const QString &commandId,
                                         const QString &commandType, QVariant commandVar,
                                         QVariant *status = nullptr);
            virtual bool processTriggers(const QVariant triggerVar, QVariant *response);
            void overwriteRetainModelMessage(quint32 api);
            void reportStatus(const QVariant &status);
            void reportResponse(const QVariant &response);
            bool isCompatible(const QString feature);

        protected slots:
            virtual void onModelChanged();
            void onCommand();
            void onTrigger();

        protected slots:
            virtual bool initPropertiesMap(const QVariantMap &properties = QVariantMap()) =
                0;

            virtual const QByteArray put(const QGenericMessage &message);
            virtual	void allocTopic() = 0;
            virtual	void allocModel() = 0;
            virtual void postponeInit();

        protected:
            QPointer<QAbstractIpcClient>	m_ipcClient;
            QPointer<QAbstractTopic>		m_topic;

        private:
            QPointer<QAbstractModel> 		m_model;
            QMutex							*m_mutex;
            QMutex							*m_triggerMutex;
            bool							m_isRetainMessage;
    };
}

#endif // QABSTRACTCONTAINER_H
