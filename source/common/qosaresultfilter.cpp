/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qosaresultfilter.h"

using namespace QRserver;

QOSAResultFilter::QOSAResultFilter(QObject *parent)
    : QEtherResultFilter(parent)
{
    setMeasureMode("OSA");
}

QOSAResultFilter::~QOSAResultFilter()
{
}
