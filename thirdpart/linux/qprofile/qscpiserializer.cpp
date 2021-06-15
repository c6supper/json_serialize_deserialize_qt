/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qscpiserializer.h"
#include "qprofile_debug.h"

using namespace QProfile;

QScpiSerializer::QScpiSerializer(QObject *parent): QAbstractSerializer(parent)
{
}

QByteArray 	QScpiSerializer::serialize(const QVariant &variant, bool *ok)
{
    return QByteArray();
}
