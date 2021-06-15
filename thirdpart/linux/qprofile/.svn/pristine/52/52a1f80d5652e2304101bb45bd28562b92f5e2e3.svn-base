/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QABSTRACTSTATUS_H
#define QABSTRACTSTATUS_H

#include <QString>
#include <QtGlobal>
#include "qprofile_export.h"
#include "qprofile_debug.h"
#include "qprofile_global.h"
#include "qabstractjsonelement.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QProfile
{
    class QAbstractStatus : public QAbstractJsonElement
    {
            Q_OBJECT

            Q_ENUMS(CommandStatus)

            Q_PROPERTY(QString id READ id WRITE setId)
            Q_PROPERTY(QString from READ from WRITE setFrom)
            Q_PROPERTY(QString method READ method WRITE setMethod)
            Q_PROPERTY(QString to READ to WRITE setTo)
            Q_PROPERTY(QString type READ type WRITE setType)
            //            Q_PROPERTY(QVariant status READ status WRITE setStatus)
            Q_PROPERTY(QVariant resource READ resource WRITE setResource)
            Q_PROPERTY(QVariant metadata READ metadata WRITE setMetadata)
            Q_PROPERTY(CommandStatus commandStatus READ commandStatus WRITE
                       setCommandStatus)

        public:
            enum CommandStatus { Succeed = 0, Failed, ClientClosed, NetworkError, IPCError, Timeout, StatusButtom};

        public:
            static const QString PropertyTopic;
            static const QString PropertyCommandStatus;

        public:
            QAbstractStatus(QObject *parent = nullptr);
            explicit QAbstractStatus(QObject *parent, const QString &id,
                                     const QString &topic = "", const quint32 commandStatus = Failed);
            QAbstractStatus(const QAbstractStatus &status);
            QAbstractStatus(const QVariant &statusVar);
            virtual ~QAbstractStatus();
            virtual void pureVirtualMock() {};
            virtual bool isValid();

            virtual const CommandStatus commandStatus() const;
            virtual  void setCommandStatus(const CommandStatus commandStatus);

            const QString id() const
            {
                return m_id;
            }
            void setId(const QString id)
            {
                m_id = 	id;
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
#if 0
            const QVariant status() const
            {
                return m_status;
            }
            void setStatus(const QVariant status)
            {
                m_status = status;
            }
#endif
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

            virtual bool canRecover();

        private:
            QString 	m_id, m_from, m_to, m_type, m_topic, m_method;
            QVariant	m_resource, m_metadata, m_status;
            quint32        m_commandStatus;
    };
}

#endif // QABSTRACTSTATUS_H
