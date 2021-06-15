#include "qwifiresultfilter.h"

using namespace QRserver;

QWifiResultFilter::QWifiResultFilter(QObject *parent)
    : QEtherResultFilter(parent)
{
    setMeasureMode("WIFI");
}

QWifiResultFilter::~QWifiResultFilter()
{
}
