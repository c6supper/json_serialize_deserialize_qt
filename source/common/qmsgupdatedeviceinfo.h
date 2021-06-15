#ifndef QMSGUPDATEDEVICEINFO_H
#define QMSGUPDATEDEVICEINFO_H

#include "qmsgregistertargetext.h"

namespace QRserver
{
    class QMsgUpdateDeviceInfo : public QMsgRegisterTargetExt
    {
            Q_OBJECT

            Q_PROPERTY(quint8 updateStatus READ updateStatus WRITE setUpdateStatus)

        public:
            QMsgUpdateDeviceInfo(QObject *parent = nullptr);
            QMsgUpdateDeviceInfo(const QMsgUpdateDeviceInfo &message);
            virtual ~QMsgUpdateDeviceInfo();

            enum UpdateStatus { Failed = 0};

            const quint8 updateStatus() const;
            void setUpdateStatus(const quint8 updateStatus);

        protected:
            virtual QDataStream &read(QDataStream &in) ;
            virtual void initAttributes();

        private:
            quint8 m_updateStatus;
    };
}

Q_DECLARE_METATYPE(QRserver::QMsgUpdateDeviceInfo)

#endif
