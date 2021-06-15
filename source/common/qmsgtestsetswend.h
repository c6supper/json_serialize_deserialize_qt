#ifndef QMSGTESTSETSWEND_H
#define QMSGTESTSETSWEND_H

#include <QMetaType>
#include <QSharedPointer>
#include <QDateTime>
#include "qmsgtestsetswblk.h"

namespace QRserver
{
    class QMsgTestsetSwEnd : public QMsgTestsetSwBlk
    {
            Q_OBJECT

        public:
            QMsgTestsetSwEnd(QObject *parent = nullptr);
            QMsgTestsetSwEnd(const QMsgTestsetSwEnd &message);
            virtual ~QMsgTestsetSwEnd();
    };
}

Q_DECLARE_METATYPE(QRserver::QMsgTestsetSwEnd)

#endif
