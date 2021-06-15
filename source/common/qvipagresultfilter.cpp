/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qvipagresultfilter.h"

using namespace QRserver;

QVipagResultFilter::QVipagResultFilter(QObject *parent)
    : QEtherResultFilter(parent)
{
    setMeasureMode("VIPAG");
}

QVipagResultFilter::~QVipagResultFilter()
{
}
