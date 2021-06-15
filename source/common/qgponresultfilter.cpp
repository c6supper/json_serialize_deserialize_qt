#include "qgponresultfilter.h"

using namespace QRserver;

QGponResultFilter::QGponResultFilter(QObject *parent)
    : QEtherResultFilter(parent)
{
    setMeasureMode("GPON");
}

QGponResultFilter::~QGponResultFilter()
{
}
