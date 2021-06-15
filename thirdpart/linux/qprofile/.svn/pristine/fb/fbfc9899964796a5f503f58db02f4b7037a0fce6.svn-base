/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QABSTRACTCOMMAND_H
#define QABSTRACTCOMMAND_H

#include <QString>
#include <QtGlobal>
#include <QUuid>
#include "qprofile_export.h"
#include "qprofile_debug.h"
#include "qprofile_global.h"
#include "qabstractjsonelement.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QProfile
{
    class QAbstractCommand : public QAbstractJsonElement
    {
            Q_OBJECT

            Q_PROPERTY(QString topic READ topic WRITE setTopic)
            Q_PROPERTY(QString id READ id WRITE setId)
            Q_PROPERTY(QString from READ from WRITE setFrom)
            Q_PROPERTY(QString method READ method WRITE setMethod)
            Q_PROPERTY(QString to READ to WRITE setTo)
            Q_PROPERTY(QString type READ type WRITE setType)
            Q_PROPERTY(QVariant resource READ resource WRITE setResource)
            Q_PROPERTY(QVariant metadata READ metadata WRITE setMetadata)

        public:
            static const quint32 RetryCountLimitation;
            static const QString PropertyId;

        public:
            QAbstractCommand(QObject *parent = nullptr);
            explicit QAbstractCommand(QObject *parent, const QString &topic,
                                      const QVariant &command);
            explicit QAbstractCommand(QObject *parent, const QString &topic,
                                      const QString &method, const QVariant &resource = QVariant());
            QAbstractCommand(const QAbstractCommand &command);
            virtual ~QAbstractCommand();

            virtual bool isValid();

            static bool IsCommand(const QVariant &variant);
            static const QString GetId(const QVariant &variant);

            const QString id() const
            {
                return m_id.toString();
            }
            void setId(const QString id)
            {
                m_id = 	QUuid(id);
            }

            const QString from() const
            {
                return m_from;
            }
            void setFrom(const QString from)
            {
                m_from = from;
            }

            const QString to() const
            {
                return m_to;
            }
            void setTo(const QString to)
            {
                m_to = to;
            }

            const QString type() const
            {
                return m_type;
            }
            void setType(const QString type)
            {
                m_type = type;
            }

            const QVariant resource() const
            {
                return m_resource;
            }
            void setResource(const QVariant resource)
            {
                m_resource = resource;
            }

            const QVariant metadata() const
            {
                return m_metadata;
            }
            void setMetadata(const QVariant metadata)
            {
                m_metadata = metadata;
            }

            const QString topic() const
            {
                return m_topic;
            }
            void setTopic(const QString topic)
            {
                m_topic = topic;
            }

            const QString method() const
            {
                return m_method;
            }
            void setMethod(const QString method)
            {
                m_method = method;
            }

        protected:
            bool isCompatible(const QString feature);

        private:
            QUuid		m_id;
            QString 	m_from, m_to, m_type, m_topic, m_method;
            QVariant	m_resource, m_metadata;
    };
}

#endif // QABSTRACTCOMMAND_H
