/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qcpriresultfilter.h"

using namespace QRserver;

QCpriResultFilter::QCpriResultFilter(QObject *parent)
    : QEtherResultFilter(parent)
{
    setMeasureMode("CPRI");
}

const QString QCpriResultFilter::xmlFileName(const QFileInfo keyInfo)
{
    return keyInfo.completeBaseName() + ".xml";
}

QCpriResultFilter::~QCpriResultFilter()
{
}
