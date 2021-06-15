
#ifndef QG703RESULTFILTER_H
#define QG703RESULTFILTER_H

#include "qetherresultfilter.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QRserver
{
    class QG703ResultFilter: public QEtherResultFilter
    {
            Q_OBJECT

        public:
            QG703ResultFilter(QObject *parent = nullptr);
            ~QG703ResultFilter();
        protected:
            virtual const QString xmlFileName(const QFileInfo keyInfo);
    };
}

#endif /* QG703RESULTFILTER_H */
