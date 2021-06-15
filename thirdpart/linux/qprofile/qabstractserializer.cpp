/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qabstractserializer.h"

using namespace QProfile;

QAbstractSerializer::QAbstractSerializer(QObject *parent) : QObject(parent),
    m_doublePrecision(2)
{
    Q_ASSERT(parent != nullptr);
}
