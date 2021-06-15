/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QABSTRACTBINMSG_H
#define QABSTRACTBINMSG_H

#include <QtGlobal>
#include <QDataStream>
#include <QMetaType>
#include <QMetaProperty>
#include "qprofile_global.h"
#include "RServerClient.h"
#include "qabstractattribute.h"
#include <QSharedPointer>
#include "qstringattribute.h"
#include "qabstractjsonelement.h"

namespace QRserver
{
    class QAbstractBinMsg : public QProfile::QAbstractJsonElement
    {
            Q_OBJECT

            Q_PROPERTY(quint32 id READ id WRITE setId)
            Q_PROPERTY(quint32 messageType READ type WRITE setType)
            Q_PROPERTY(QString serialNumber READ serialNumber WRITE setSerialNumber)

        public:
            QAbstractBinMsg(const quint32 type = 0,
                            QObject *parent = nullptr): QAbstractJsonElement(parent), m_type(type)
            {
                m_serialNumber = QSharedPointer<QProfile::QStringAttribute>
                                 (new QProfile::QStringAttribute(this));
            }
            QAbstractBinMsg(const QAbstractBinMsg &message): QAbstractJsonElement(
                    message.parent()),
                m_type(message.type())
            {
                m_serialNumber = QSharedPointer<QProfile::QStringAttribute>
                                 (new QProfile::QStringAttribute(this));
            }
            virtual ~QAbstractBinMsg() {}
            const quint32 type() const
            {
                return m_type;
            }
            void setType(const quint32 type)
            {
                m_type = type;
            }

            const quint32 id() const
            {
                return m_id;
            }
            void setId(const quint32 id)
            {
                m_id = id;
            }
            const QString serialNumber() const
            {
                return m_serialNumber->value().toString();
            }
            void setSerialNumber(const QString serialNumber)
            {
                m_serialNumber->setValue(serialNumber);
            }

            virtual void pureVirtualMock()
            {
            }
            virtual const QByteArray toByteArray() const = 0;

            bool operator==(const QAbstractBinMsg &other) const
            {
                for (int propertyIdx = 0; propertyIdx < metaObject()->propertyCount();
                     propertyIdx++) {
                    QMetaProperty metaProperty = metaObject()->property(propertyIdx);
                    int dstIdx = other.metaObject()->indexOfProperty(metaProperty.name());

                    if (dstIdx >= 0) {
                        if (property(metaProperty.name()) != other.property(metaProperty.name())) {
                            return false;
                        }
                    } else {
                        return false;
                    }
                }

                return true;
            }

            friend QDataStream &operator>>(QDataStream &in, QAbstractBinMsg &message)
            {
                return message.read(in);
            }
            friend QDataStream &operator<<(QDataStream &out, const QAbstractBinMsg &message)
            {
                return message.write(out);
            }

        protected:
            virtual QDataStream &read(QDataStream &in) = 0;
            virtual QDataStream &write(QDataStream &out) const = 0;

        protected:
            quint32		m_type;
            quint32		m_id;
            QSharedPointer<QProfile::QStringAttribute> m_serialNumber;
    };
}

#endif
