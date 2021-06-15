#ifndef QMSGDEVICEBONDINFO_H
#define QMSGDEVICEBONDINFO_H

#include <QMetaType>
#include "qabstractbinmsg.h"
#include "qprofile_debug.h"
#include "qprofile_global.h"
#include "qdeviceinfo.h"

namespace QRserver
{
    class QMsgDeviceBondInfo : public QAbstractBinMsg
    {
            Q_OBJECT

            Q_PROPERTY(QVariantList deviceList READ deviceList WRITE setDeviceList)

        public:
            QMsgDeviceBondInfo(QObject *parent = nullptr);
            QMsgDeviceBondInfo(const QMsgDeviceBondInfo &message);
            virtual ~QMsgDeviceBondInfo();

            const QVariantList deviceList() const;
            void setDeviceList(const QVariantList deviceList);

            qint32 moduleCanAssignOptionCount() const;

        protected:
            virtual QDataStream &read(QDataStream &in);
            virtual QDataStream &write(QDataStream &out) const;
            virtual const QByteArray toByteArray() const;

        private:
            QList<QDeviceInfo *> m_deviceList;
            QMutex *m_mutex;
    };
}

Q_DECLARE_METATYPE(QRserver::QMsgDeviceBondInfo)

#endif // QMSGDEVICEBONDINFO_H
