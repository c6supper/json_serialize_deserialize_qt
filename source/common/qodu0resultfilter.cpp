/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qodu0resultfilter.h"

using namespace QRserver;

QOdu0ResultFilter::QOdu0ResultFilter(QObject *parent)
    : QEtherResultFilter(parent)
{
    setMeasureMode("ODU0");
}

QOdu0ResultFilter::~QOdu0ResultFilter()
{
}
