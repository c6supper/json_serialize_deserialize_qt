/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include <QCoreApplication>
#include <QtGlobal>
#include "qrserverstate.h"
#include "qrservermessageevent.h"
#include "qabstractbinmsg.h"

using namespace QRserver;

QRserverState::QRserverState(QState *parent, QObject *messageDispather,
                             QVariant request, ChildMode childMode)
    : QState(childMode, parent), m_request(request),
      m_messageDispather(messageDispather)
{
    Q_ASSERT(parent != nullptr);
}

QRserverState::~QRserverState()
{
}

void QRserverState::onEntry(QEvent *event)
{
    if (m_messageDispather != nullptr && m_request.toMap().count() > 0) {
        QCoreApplication::postEvent(m_messageDispather,
                                    new QRserverMessageEvent(m_request));
        event->accept();
    }

    QState::onEntry(event);
}

