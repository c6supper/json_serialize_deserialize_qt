/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */


#include "qipattribute.h"
#include "qobjectfactory.h"
#include <QtCore/QVariant>

using namespace QProfile;

QObjectSelfRegisteration<QIpAttribute> registerQIpAttribute;

QIpAttribute::QIpAttribute(QObject *parent)
    : QAbstractAttribute(QVariant("127.0.0.1"), QVariant::String, parent)
{
    setRegExp("((25[0-5]|2[0-4]\\d|((1\\d{2})|([1-9]?\\d)))\\.){3}(25[0-5]|2[0-4]\\d|((1\\d{2})|([1-9]?\\d)))");
}

QIpAttribute::QIpAttribute(const QVariant &value, QObject *parent)
    : QAbstractAttribute(value, QVariant::String, parent)
{
    setRegExp("((25[0-5]|2[0-4]\\d|((1\\d{2})|([1-9]?\\d)))\\.){3}(25[0-5]|2[0-4]\\d|((1\\d{2})|([1-9]?\\d)))");
}

