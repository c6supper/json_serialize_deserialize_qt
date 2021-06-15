#ifndef QMSGUPDATEDEVICEINFOLONGID_H
#define QMSGUPDATEDEVICEINFOLONGID_H

#include "qmsgupdatedeviceinfo.h"

namespace QRserver
{
    class QMsgUpdateDeviceInfoLongId : public QMsgUpdateDeviceInfo
    {
            Q_OBJECT

        public:
            QMsgUpdateDeviceInfoLongId(QObject *parent = nullptr);
            QMsgUpdateDeviceInfoLongId(const QMsgUpdateDeviceInfoLongId &message);
            virtual ~QMsgUpdateDeviceInfoLongId();

        protected:
            virtual QDataStream &write(QDataStream &out) const;
            virtual const QByteArray toByteArray() const;
    };
}

Q_DECLARE_METATYPE(QRserver::QMsgUpdateDeviceInfoLongId)

#endif
