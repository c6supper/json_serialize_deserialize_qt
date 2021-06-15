/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */


#include "qnumericattribute.h"
#include "qobjectfactory.h"
#include <QtCore/QVariant>

using namespace QProfile;

QObjectSelfRegisteration<QNumericAttribute> registerQNumericAttribute;

QNumericAttribute::QNumericAttribute(QObject *parent)
    : QAbstractAttribute(0, QVariant::Double, parent)
{
}

QNumericAttribute::QNumericAttribute(const QVariant &value,
                                     const QVariant::Type type, QObject *parent)
    : QAbstractAttribute(value, QVariant::Double, parent)
{
}

