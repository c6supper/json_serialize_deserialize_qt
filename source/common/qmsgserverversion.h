/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QMSGSERVERVERSION_H
#define QMSGSERVERVERSION_H

#include <QMetaType>
#include "qmsggetsoftwareversion.h"

namespace QRserver
{
    class QMsgServerVersion : public QMsgGetSoftwareVersion
    {
            Q_OBJECT

            Q_PROPERTY(QString serverVersion READ serverVersion WRITE
                       setServerVersion)
        public:
            QMsgServerVersion(QObject *parent = nullptr);
            QMsgServerVersion(const QMsgServerVersion &message);
            virtual ~QMsgServerVersion();

            QString serverVersion() const;
            void setServerVersion(const QString serverVersion);
    };
}

Q_DECLARE_METATYPE(QRserver::QMsgServerVersion)

#endif
