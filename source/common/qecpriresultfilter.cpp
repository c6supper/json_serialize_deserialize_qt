/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qecpriresultfilter.h"

using namespace QRserver;

QEcpriResultFilter::QEcpriResultFilter(QObject *parent)
    : QEtherResultFilter(parent)
{
    setMeasureMode("eCPRI");
}

QEcpriResultFilter::~QEcpriResultFilter()
{
}
