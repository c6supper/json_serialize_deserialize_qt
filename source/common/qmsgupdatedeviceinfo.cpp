#include "qmessagefactory.h"
#include "qmsgupdatedeviceinfo.h"

using namespace QRserver;
using namespace QProfile;
using namespace rserver;

QMessageSelfRegisteration<QMsgUpdateDeviceInfo> registerQMsgUpdateDeviceInfo(
    rserver::eUpdateDeviceInfo);

QDataStream &QMsgUpdateDeviceInfo::read(QDataStream &in)
{
    in >> m_updateStatus;
    return in;
}

QMsgUpdateDeviceInfo::QMsgUpdateDeviceInfo(QObject *parent)
    : QMsgRegisterTargetExt(parent)
{
    setType(rserver::eUpdateDeviceInfo);
    initAttributes();
}

QMsgUpdateDeviceInfo::QMsgUpdateDeviceInfo(const QMsgUpdateDeviceInfo
        &message)
    : QMsgRegisterTargetExt(message)
{
    setType(rserver::eUpdateDeviceInfo);
    initAttributes();
}

void QMsgUpdateDeviceInfo::initAttributes()
{
    QMsgRegisterTargetExt::initAttributes();
    m_updateStatus = Failed;
}

QMsgUpdateDeviceInfo::~QMsgUpdateDeviceInfo()
{
}

const quint8 QMsgUpdateDeviceInfo::updateStatus() const
{
    return m_updateStatus;
}
void QMsgUpdateDeviceInfo::setUpdateStatus(const quint8 updateStatus)
{
    m_updateStatus = updateStatus;
}

