/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QSERVERVERSION_H
#define QSERVERVERSION_H

#include "qabstractjsonelement.h"
#include <QRegExp>

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QRserver
{
    class QServerVersion : public QProfile::QAbstractJsonElement
    {
            Q_OBJECT

            Q_PROPERTY(qint32 version READ version)

        public:
            static const QString defaultRserverVersion;
            static const qint32 invalidVersionIntger = -1;

        public:
            QServerVersion(const QString &version = "",
                           QObject *parent = nullptr);
            ~QServerVersion() {}
            virtual void pureVirtualMock() {};
            const qint32 version() const;
            const QString versionString() const;
            bool isValid() const;

        private:
            qint32					m_major;
            qint32					m_minor;
            qint32					m_patch;
            qint32					m_build;
            static const QRegExp	m_versionRegExp;
    };
}

#endif // QSERVERVERSION_H
