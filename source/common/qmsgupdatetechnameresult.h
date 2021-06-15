/*
 * qmsgupdatetechnameresult.h
 *
 *  Created on: Aug 3, 2020
 *      Author: duyi
 */

#ifndef QMSGUPDATETECHNAMERESULT_H
#define QMSGUPDATETECHNAMERESULT_H

#include "qmsgupdatetechname.h"

namespace QRserver
{
    class QMsgUpdateTechNameResult : public QMsgUpdateTechName
    {
            Q_OBJECT

        public:
            QMsgUpdateTechNameResult(QObject *parent = nullptr);
            QMsgUpdateTechNameResult(const QMsgUpdateTechNameResult &message);
            virtual ~QMsgUpdateTechNameResult();
    };
}

Q_DECLARE_METATYPE(QRserver::QMsgUpdateTechNameResult)
#endif /* QMSGUPDATETECHNAMERESULT_H */
