#include "qmtt52xresultfilter.h"

using namespace QRserver;

QMtt52xResultFilter::QMtt52xResultFilter(QObject *parent)
    : QEtherResultFilter(parent)
{
    setMeasureMode("MTT52X");
}

QMtt52xResultFilter::~QMtt52xResultFilter()
{
}
