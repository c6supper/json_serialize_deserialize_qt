/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QRSERVERTOPIC_H
#define QRSERVERTOPIC_H

#include "qprofile_global.h"
#include "qabstracttopic.h"

QT_BEGIN_NAMESPACE

QT_END_NAMESPACE

namespace QRserver
{
    class QRserverTopic : public QProfile::QAbstractTopic
    {
            Q_OBJECT
        public:
            explicit QRserverTopic(QObject *parent, QProfile::QAbstractIpcClient *ipcClient,
                                   const QString &relativeTopic = "", RestfulApis api = put | command | trigger);
            ~QRserverTopic();

        protected:
            virtual const QString absoluteTopic(const QString &relativeTopic);
    };
}
#endif // QRSERVERTOPIC_H
