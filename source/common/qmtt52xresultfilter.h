
#ifndef QMTT52XRESULTFILTER_H
#define QMTT52XRESULTFILTER_H

#include "qetherresultfilter.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QRserver
{
    class QMtt52xResultFilter: public QEtherResultFilter
    {
            Q_OBJECT

        public:
            QMtt52xResultFilter(QObject *parent = nullptr);
            ~QMtt52xResultFilter();
    };
}

#endif /* QMTT52XRESULTFILTER_H */
