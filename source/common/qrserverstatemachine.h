/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QRSERVERSTATEMACHINE_H
#define QRSERVERSTATEMACHINE_H

#include "qprofile_global.h"
#include <QStateMachine>
#include <QVariant>

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QRserver
{
    class QRserverStateMachine: public QStateMachine
    {
            Q_OBJECT

        public:
            QRserverStateMachine(QObject *parent = nullptr);
            ~QRserverStateMachine();

            void registerPostBackParas(const QVariant postBackParas);
            const QVariant &postBackParas();

        private:
            QVariant	m_postBackParas;
    };
}


#endif /* QRSERVERSTATEMACHINE_H */
