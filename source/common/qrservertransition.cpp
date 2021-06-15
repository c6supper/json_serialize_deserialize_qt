/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include <QtGlobal>
#include "qrservertransition.h"
#include "qrservermessageevent.h"

using namespace QRserver;

const QStringList QRserverTransition::INGORED_KEY_LIST = QStringList() <<
        QString("jsoncomment") << QString("id") << QString("sn");

QRserverTransition::QRserverTransition(QVariant transition,
                                       QState *sourceState)
    : QAbstractTransition(sourceState), m_transition(transition)
{
    Q_ASSERT(sourceState != nullptr);
}

QRserverTransition::~QRserverTransition()
{
}

bool QRserverTransition::eventTest(QEvent *e)
{
    if (e->type() == QRserverMessageEvent::messageEventType()
        && e->type() != QEvent::None) {
        QRserverMessageEvent *messageEvent = (QRserverMessageEvent *)e;
        QVariantMap messageMap = messageEvent->message().toMap();
        QVariantMap transitionMap = m_transition.toMap();
        QVariantMap::const_iterator iter = transitionMap.constBegin();

        while (iter != transitionMap.constEnd()) {
            if (messageMap.value(iter.key()) != iter.value()
                && !INGORED_KEY_LIST.contains(iter.key())) {
                qprofileDebug(QtDebugMsg) << this << "receive a unmatched message = " <<
                                          messageMap["messageType"]
                                          << "The unmatched key = " << iter.key() << "expected value = " << iter.value()
                                          << ",unexpected value = " <<
                                          messageMap.value(iter.key());
                return false;
            }

            ++iter;
        }

        qprofileDebug(QtDebugMsg) << this << "got matched message = " <<
                                  messageMap["messageType"];
        return true;
    }

    return false;
}

void QRserverTransition::onTransition(QEvent *e)
{
}


