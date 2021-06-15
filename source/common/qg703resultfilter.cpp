#include "qg703resultfilter.h"

using namespace QRserver;

QG703ResultFilter::QG703ResultFilter(QObject *parent)
    : QEtherResultFilter(parent)
{
    setMeasureMode("G703");
}

const QString QG703ResultFilter::xmlFileName(const QFileInfo keyInfo)
{
    return keyInfo.completeBaseName() + ".xml";
}

QG703ResultFilter::~QG703ResultFilter()
{
}
