/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */


#include "qupgradeinfo.h"
#include "qprofile_debug.h"
#include "qobjectfactory.h"

using namespace QProfile;
using namespace QRserver;

QObjectSelfRegisteration<QRserver::QUpgradeInfo> registerQUpgradeInfo;

QUpgradeInfo::QUpgradeInfo(QObject *parent) : QDeviceInfo(parent),
    m_softwareMd5("")
{
    m_imageSize = QSharedPointer<QNumericAttribute>(new QNumericAttribute(0,
                  QVariant::Int, this));
    m_downloadedSize = QSharedPointer<QNumericAttribute>(new QNumericAttribute(0,
                       QVariant::Int, this));
    m_upgradeStatus =  QSharedPointer<QNumericAttribute>(new QNumericAttribute((
                           qint32)QUpgradeInfo::Idle, QVariant::Int, this));
    m_downloadedPercentage = QSharedPointer<QStringAttribute>(new QStringAttribute(
                                 this));
}

const QString QUpgradeInfo::getDeltaImageName()
{
    return QString("%1_to_%2_%3.tar.gz").arg(version()).arg(remoteVersion()).arg(
               deviceName().toLower());
}

const QByteArray QUpgradeInfo::softwareMd5() const
{
    return m_softwareMd5;
}
void QUpgradeInfo::setSoftwareMd5(const QByteArray softwareMd5)
{
    m_softwareMd5 = softwareMd5;
}

quint32 QUpgradeInfo::imageSize() const
{
    return m_imageSize->value().toUInt();
}
void QUpgradeInfo::setImageSize(quint32 imageSize)
{
    m_imageSize->setValue(imageSize);
}

quint32 QUpgradeInfo::downloadedSize() const
{
    return m_downloadedSize->value().toUInt();
}
void QUpgradeInfo::setDownloadedSize(quint32 downloadedSize)
{
    m_downloadedSize->setValue(downloadedSize);
}

qint32 QUpgradeInfo::upgradeStatus() const
{
    return m_upgradeStatus->value().toInt();
}
void QUpgradeInfo::setUpgradeStatus(qint32 upgradeStatus)
{
    m_upgradeStatus->setValue(upgradeStatus);
}

const QString QUpgradeInfo::downloadedPercentage() const
{
    return m_downloadedPercentage->value().toString();
}

void QUpgradeInfo::setDownloadedPercentage(const QString
        downloadedPercentage)
{
    m_downloadedPercentage->setValue(downloadedPercentage);
}

void QUpgradeInfo::initAttributes()
{
}

