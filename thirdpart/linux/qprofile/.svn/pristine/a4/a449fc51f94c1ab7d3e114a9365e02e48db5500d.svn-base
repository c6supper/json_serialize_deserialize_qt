/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */


#include "qgenericcommand.h"
#include "qobjectfactory.h"

using namespace QProfile;

QObjectSelfRegisteration<QGenericCommand> registerQGenericCommand;

QGenericCommand::QGenericCommand(QObject *parent): QAbstractCommand(parent)
{
}

QGenericCommand::QGenericCommand(QObject *parent, const QString &method,
                                 const QString &topic, const QVariant &resource): QAbstractCommand(parent,
                                             topic, method, resource)
{
}

QGenericCommand::~QGenericCommand()
{
}

bool QGenericCommand::isValid()
{
    return (!method().isEmpty() && QAbstractCommand::isValid());
}
