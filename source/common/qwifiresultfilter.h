
#ifndef QWIFIRESULTFILTER_H
#define QWIFIRESULTFILTER_H

#include "qetherresultfilter.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QRserver
{
    class QWifiResultFilter: public QEtherResultFilter
    {
            Q_OBJECT

        public:
            QWifiResultFilter(QObject *parent = nullptr);
            ~QWifiResultFilter();
    };
}


#endif /* QWIFIRESULTFILTER_H */
