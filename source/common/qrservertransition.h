/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QRSERVERTRANSITION_H
#define QRSERVERTRANSITION_H

#include "qprofile_global.h"
#include <QAbstractTransition>
#include <QState>
#include <QVariant>
#include <QStringList>

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QRserver
{
    class QRserverTransition: public QAbstractTransition
    {
            Q_OBJECT

        public:
            static const QStringList INGORED_KEY_LIST;

        public:
            QRserverTransition(QVariant transition, QState *sourceState = nullptr);
            ~QRserverTransition();

        protected:
            virtual bool eventTest(QEvent *e);

            virtual void onTransition(QEvent *e);

        private:
            QVariant	m_transition;

    };
}


#endif /* QRSERVERTRANSITION_H */
