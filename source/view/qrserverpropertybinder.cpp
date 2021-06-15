/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qrserverpropertybinder.h"

using namespace QRserver;
using namespace QProfile;

QRserverPropertyBinder::QRserverPropertyBinder(QObject *view, QObject *model)
    : QAbstractPropertyBinder(view, model)
{
    Q_ASSERT(parent != nullptr);
}

QRserverPropertyBinder::~QRserverPropertyBinder()
{
}


