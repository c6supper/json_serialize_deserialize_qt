
#ifndef QGPONRESULTFILTER_H
#define QGPONRESULTFILTER_H

#include "qetherresultfilter.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QRserver
{
    class QGponResultFilter: public QEtherResultFilter
    {
            Q_OBJECT

        public:
            QGponResultFilter(QObject *parent = nullptr);
            ~QGponResultFilter();
    };
}


#endif /* QGPONRESULTFILTER_H */
