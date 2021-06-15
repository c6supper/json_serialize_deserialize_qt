/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */


#include "qdatetimeattribute.h"
#include "qobjectfactory.h"
#include <QtCore/QVariant>
#include <QDateTime>

using namespace QProfile;

QObjectSelfRegisteration<QDateTimeAttribute> registerQDateTimeAttribute;

QDateTimeAttribute::QDateTimeAttribute(QObject *parent)
    : QAbstractAttribute(0, QVariant::DateTime, parent)
{
    setValue(QDateTime::currentDateTime());
}

QDateTimeAttribute::QDateTimeAttribute(const QVariant &value,
                                       const QVariant::Type type, QObject *parent)
    : QAbstractAttribute(value, QVariant::DateTime, parent)
{
}

