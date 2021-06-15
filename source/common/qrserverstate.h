/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QRSERVERSTATE_H
#define QRSERVERSTATE_H

#include "qprofile_global.h"
#include <QState>
#include <QVariant>

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QRserver
{
    class QRserverState: public QState
    {
            Q_OBJECT

        public:
            QRserverState(QState *parent = nullptr, QObject *messageDispather = nullptr,
                          QVariant request = QVariant(),
                          ChildMode childMode = QState::ExclusiveStates);
            ~QRserverState();

        protected:
            virtual void onEntry(QEvent *event);

        private:
            QVariant	 	m_request;
            QObject 		*m_messageDispather;
    };
}


#endif /* QRSERVERSTATE_H */
