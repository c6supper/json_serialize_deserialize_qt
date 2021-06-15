/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qotuxresultfilter.h"

using namespace QRserver;

QOtuxResultFilter::QOtuxResultFilter(QObject *parent)
    : QEtherResultFilter(parent)
{
    setMeasureMode("OTU-Xe(1e/2e)");
}

QOtuxResultFilter::~QOtuxResultFilter()
{
}
