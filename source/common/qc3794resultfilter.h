
#ifndef QC3794RESULTFILTER_H
#define QC3794RESULTFILTER_H

#include "qetherresultfilter.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QRserver
{
    class QC3794ResultFilter: public QEtherResultFilter
    {
            Q_OBJECT

        public:
            QC3794ResultFilter(QObject *parent = nullptr);
            ~QC3794ResultFilter();
        protected:
            virtual const QString xmlFileName(const QFileInfo keyInfo);
    };
}

#endif /* QC3794RESULTFILTER_H */
