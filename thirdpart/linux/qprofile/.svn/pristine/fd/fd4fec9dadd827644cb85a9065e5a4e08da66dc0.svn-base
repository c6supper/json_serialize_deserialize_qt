/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QGENERICMESSAGE_H
#define QGENERICMESSAGE_H

#include <QVariant>
#include <QByteArray>
#include "qprofile_debug.h"
#include "qprofile_global.h"
#include "qabstractjsonelement.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QProfile
{
    class QGenericMessage : public QAbstractJsonElement
    {
            Q_OBJECT

            Q_PROPERTY(QString topic READ topic WRITE setTopic)
            Q_PROPERTY(QString id READ id WRITE setId)
            Q_PROPERTY(QString from READ from WRITE setFrom)
            Q_PROPERTY(QString to READ to WRITE setTo)
            Q_PROPERTY(QString type READ type WRITE setType)
            Q_PROPERTY(QVariant content READ content WRITE setContent)
            Q_PROPERTY(QVariant metadata READ metadata WRITE setMetadata)

        public:
            explicit QGenericMessage(QObject *parent, const QString &topic,
                                     const QVariant &messageVar);
            explicit QGenericMessage(QObject *parent = nullptr);
            explicit QGenericMessage(const QGenericMessage &message,
                                     QObject *parent = nullptr);
            /*            explicit QGenericMessage(QObject *parent, const QString &topic,
                                                 const QByteArray &message  = "", const QString &type = "",
                                                 const QByteArray &metaData  = "", const QString &id = "",
                                                 const QString &from  = "", const QString &to  = "");*/

            ~QGenericMessage() {}
            virtual void pureVirtualMock() {};

            const QString id() const
            {
                return m_id;
            }
            void setId(const QString id)
            {
                m_id = id;
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

            const QVariant content() const
            {
                return m_content;
            }
            void setContent(const QVariant content)
            {
                m_content = content;
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

        private:
            QString 	m_id, m_from, m_to, m_type, m_topic;
            QVariant	m_content, m_metadata;
    };
}

Q_DECLARE_METATYPE(QProfile::QGenericMessage)

#endif // QGENERICMESSAGE_H
