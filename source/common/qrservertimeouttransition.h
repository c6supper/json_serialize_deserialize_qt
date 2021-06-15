/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QRSERVERTIMEOUTTRANSITION_H
#define QRSERVERTIMEOUTTRANSITION_H

#include "qprofile_global.h"
#include <QAbstractTransition>
#include <QState>
#include <QVariant>
#include <QStringList>

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QRserver
{
    class QRserverTimeoutTransition: public QAbstractTransition
    {
            Q_OBJECT

        public:
            QRserverTimeoutTransition(QState *sourceState = nullptr);
            ~QRserverTimeoutTransition();

        protected:
            virtual bool eventTest(QEvent *e);
            virtual void onTransition(QEvent *e) {}
    };
}


#endif /* QRSERVERTIMEOUTTRANSITION_H */
