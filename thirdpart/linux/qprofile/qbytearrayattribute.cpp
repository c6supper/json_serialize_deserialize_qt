/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */


#include "qbytearrayattribute.h"
#include "qobjectfactory.h"
#include <QtCore/QVariant>

using namespace QProfile;

QObjectSelfRegisteration<QByteArrayAttribute> registerQByteArrayAttribute;

QByteArrayAttribute::QByteArrayAttribute(QObject *parent)
    : QAbstractAttribute(0, QVariant::ByteArray, parent)
{
}

QByteArrayAttribute::QByteArrayAttribute(const QVariant &value,
        const QVariant::Type type, QObject *parent)
    : QAbstractAttribute(value, QVariant::ByteArray, parent)
{
}

