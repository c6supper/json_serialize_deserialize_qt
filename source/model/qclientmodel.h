/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QCLIENTMODEL_H
#define QCLIENTMODEL_H

#include <QObject>
#include <QList>
#include <QVariant>
#include <QMutexLocker>
#include <QVariantList>
#include "qprofile_debug.h"
#include "qprofile_global.h"
#include "qmessageseries.h"
#include "qabstractcommand.h"
#include "qregistermodel.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QRserver
{
    class QClientModel: public QProfile::QAbstractModel
    {
            Q_OBJECT

        public:
            static const QString CommandProcessMessage;
            static QString RserverClientStatusTopic;
            static QString RserverClientCommandTopic;

        public:
            QClientModel(QObject *parent = nullptr);
            QClientModel(const QClientModel &model);
            ~QClientModel();

            const QRegisterModel &registerModel() const
            {
                return *m_registerModel;
            }

        public slots:
            virtual void initAttributes();

        private:
            QSharedPointer<const QRegisterModel>				m_registerModel;

    };
}

Q_DECLARE_METATYPE(QRserver::QClientModel)

#endif
