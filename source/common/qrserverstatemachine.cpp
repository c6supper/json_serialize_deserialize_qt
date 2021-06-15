/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include <QtGlobal>
#include "qprofile_debug.h"
#include "qrserverstatemachine.h"

using namespace QRserver;

QRserverStateMachine::QRserverStateMachine(QObject *parent)
    : QStateMachine(parent)
{
    Q_ASSERT(parent != nullptr);
}

QRserverStateMachine::~QRserverStateMachine()
{
}

void QRserverStateMachine::registerPostBackParas(const QVariant postBackParas)
{
    m_postBackParas = postBackParas;
}

const QVariant &QRserverStateMachine::postBackParas()
{
    return m_postBackParas;
}


