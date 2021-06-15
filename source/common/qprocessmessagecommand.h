/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QPROCESSMESSAGECOMMAND_H
#define QPROCESSMESSAGECOMMAND_H

#include "qabstractcommand.h"
#include "qmessageseries.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

using namespace QProfile;

namespace QRserver
{
    class QProcessMessageCommand : public QAbstractCommand
    {
            Q_OBJECT

        public:
            QProcessMessageCommand(const QVariant device, QObject *parent = nullptr,
                                   bool needBind = true);
            ~QProcessMessageCommand();

            virtual bool isValid();
            bool appendMessage(const quint32 parentMessageId, QAbstractBinMsg *message);
            static void RemoveBindDevice();
            virtual void pureVirtualMock() {};

        private:
            void bindDevice(const QVariant &device);

        private:
            QMessageSeries			m_messageSeries;
            static QVariant			m_bindedDevice;
    };
}

#endif // QPROCESSMESSAGECOMMAND_H
