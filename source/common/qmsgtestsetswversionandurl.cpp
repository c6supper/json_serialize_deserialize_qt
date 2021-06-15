
#include "qmessagefactory.h"
#include "qmsgtestsetswversionandurl.h"

using namespace QRserver;
using namespace QProfile;
using namespace rserver;

QMessageSelfRegisteration<QMsgTestsetSWVersionAndURL>
registerQMsgTestsetSWVersionAndURL(
    rserver::eTestsetSWVersionAndURL);

QDataStream &QMsgTestsetSWVersionAndURL::read(QDataStream &in)
{
    st_TestsetSWVersionAndURL stMessage;

    if ((qint32)sizeof(stMessage) <= in.readRawData((char *)&stMessage,
            sizeof(stMessage))) {
        m_deviceType->setValue(QVariant(stMessage.Type));
        m_version->setValue(QVariant(stMessage.Version));
        m_fullImageURL->setValue(QVariant(stMessage.FullImageURL));
        m_deltaImageURL->setValue(QVariant(stMessage.DeltaImageURL));
        m_upgradeFromVersion->setValue(QVariant(stMessage.UpgradeFromVersion));
        m_fullImageChecksum->setValue(QVariant(stMessage.FullImageChecksum));
        m_deltaImageChecksum->setValue(QVariant(stMessage.DeltaImageChecksum));
        qprofileDebug(QtDebugMsg) << toJson();
    }

    return in;
}

const QByteArray QMsgTestsetSWVersionAndURL::toByteArray() const
{
    qint32 type;
    bzero(&type, sizeof(type));
    QByteArray messageArray((const char *)&type, sizeof(type));
    QDataStream out(&messageArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_6);
    out << *this;
    return messageArray;
}

QDataStream &QMsgTestsetSWVersionAndURL::write(QDataStream &out) const
{
    qint32 type = m_deviceType->value().toInt();
    out.writeRawData((const char *)&type, sizeof(type));
    qprofileDebug(QtDebugMsg) << toJson();
    return out;
}

QMsgTestsetSWVersionAndURL::QMsgTestsetSWVersionAndURL(QObject *parent)
    : QAbstractBinMsg(rserver::eTestsetSWVersionAndURL, parent)
{
    initAttributes();
}

QMsgTestsetSWVersionAndURL::QMsgTestsetSWVersionAndURL(const
        QMsgTestsetSWVersionAndURL
        &message)
    : QAbstractBinMsg(message)
{
    initAttributes();
}

void QMsgTestsetSWVersionAndURL::initAttributes()
{
    m_deviceType = QSharedPointer<QNumericAttribute>(new QNumericAttribute(0,
                   QVariant::Int, this));
    m_version = QSharedPointer<QStringAttribute>(new QStringAttribute(this));
    m_fullImageURL = QSharedPointer<QStringAttribute>(new QStringAttribute(this));
    m_deltaImageURL = QSharedPointer<QStringAttribute>(new QStringAttribute(this));
    m_upgradeFromVersion = QSharedPointer<QStringAttribute>(new QStringAttribute(
                               this));
    m_fullImageChecksum = QSharedPointer<QStringAttribute>(new QStringAttribute(
                              this));
    m_deltaImageChecksum = QSharedPointer<QStringAttribute>(new QStringAttribute(
                               this));
}

qint32 QMsgTestsetSWVersionAndURL::deviceType() const
{
    return m_deviceType->value().toInt();
}
void QMsgTestsetSWVersionAndURL::setDeviceType(qint32 deviceType)
{
    m_deviceType->setValue(deviceType);
}

QString QMsgTestsetSWVersionAndURL::version() const
{
    return m_version->value().toString();
}
void QMsgTestsetSWVersionAndURL::setVersion(QString version)
{
    m_version->setValue(version);
}

QString QMsgTestsetSWVersionAndURL::fullImageURL() const
{
    return m_fullImageURL->value().toString();
}
void QMsgTestsetSWVersionAndURL::setFullImageURL(QString fullImageURL)
{
    m_fullImageURL->setValue(fullImageURL);
}

QString QMsgTestsetSWVersionAndURL::deltaImageURL() const
{
    return m_deltaImageURL->value().toString();
}
void QMsgTestsetSWVersionAndURL::setDeltaImageURL(QString deltaImageURL)
{
    m_deltaImageURL->setValue(deltaImageURL);
}

QString QMsgTestsetSWVersionAndURL::upgradeFromVersion() const
{
    return m_upgradeFromVersion->value().toString();
}
void QMsgTestsetSWVersionAndURL::setUpgradeFromVersion(QString
        upgradeFromVersion)
{
    m_upgradeFromVersion->setValue(upgradeFromVersion);
}

QString QMsgTestsetSWVersionAndURL::fullImageChecksum() const
{
    return m_fullImageChecksum->value().toString();
}
void QMsgTestsetSWVersionAndURL::setFullImageChecksum(QString fullImageChecksum)
{
    m_fullImageChecksum->setValue(fullImageChecksum);
}

QString QMsgTestsetSWVersionAndURL::deltaImageChecksum() const
{
    return m_deltaImageChecksum->value().toString();
}
void QMsgTestsetSWVersionAndURL::setDeltaImageChecksum(QString
        deltaImageChecksum)
{
    m_deltaImageChecksum->setValue(deltaImageChecksum);
}

QMsgTestsetSWVersionAndURL::~QMsgTestsetSWVersionAndURL() {}
