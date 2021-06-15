#include "qc3794resultfilter.h"

using namespace QRserver;

QC3794ResultFilter::QC3794ResultFilter(QObject *parent)
    : QEtherResultFilter(parent)
{
    setMeasureMode("C3794");
}

const QString QC3794ResultFilter::xmlFileName(const QFileInfo keyInfo)
{
    return keyInfo.completeBaseName() + ".xml";
}

QC3794ResultFilter::~QC3794ResultFilter()
{
}
