/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qfiberresultfilter.h"

using namespace QRserver;

QFiberResultFilter::QFiberResultFilter(QObject *parent)
    : QEtherResultFilter(parent)
{
    setMeasureMode("Fiber Channel");
}

QFiberResultFilter::~QFiberResultFilter()
{
}
