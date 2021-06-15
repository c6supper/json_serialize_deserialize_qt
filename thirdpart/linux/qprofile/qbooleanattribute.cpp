/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */


#include "qbooleanattribute.h"
#include "qobjectfactory.h"
#include <QtCore/QVariant>

using namespace QProfile;

QObjectSelfRegisteration<QBooleanAttribute> registerQBooleanAttribute;

QBooleanAttribute::QBooleanAttribute(const bool &value, QObject *parent)
    : QAbstractAttribute(value, QVariant::Bool, parent)
{
}

QBooleanAttribute::QBooleanAttribute(QObject *parent): QAbstractAttribute(false,
            QVariant::Bool, parent)
{
}
