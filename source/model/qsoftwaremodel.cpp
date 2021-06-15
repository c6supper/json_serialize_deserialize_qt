/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qsoftwaremodel.h"
#include "qobjectfactory.h"
#include "qmodelfactory.h"
#include <QMutexLocker>
#include <QWidget>

using namespace QProfile;
using namespace QRserver;

QObjectSelfRegisteration<QSoftwareModel> registerQSoftwareModel;

const QString QSoftwareModel::CommandUpgrade = "upgrade";
const quint32 QSoftwareModel::PackageTimeout = 60 * 1000;
const quint32 QSoftwareModel::PropertyChangedTimeout = 1 * 1000;
const quint32 QSoftwareModel::MaxPacSizeForOldUpgradingMethod = 100 * 1024 *
        1024;

QSoftwareModel::QSoftwareModel(QObject *parent)
    : QAbstractModel(parent), m_mutex(new QMutex())
{
    Q_ASSERT(parent != nullptr);
    m_serverPort = QSharedPointer<QNumericAttribute>(new QNumericAttribute(QVariant(
                       10001), QVariant::UInt, this));
    m_rserverModel = QModelFactory::constModel<QRserverModel>(this);
    m_registerModel = QModelFactory::constModel<QRegisterModel>(this);
    connect(m_rserverModel, SIGNAL(propertyChanged()), this,
            SIGNAL(propertyChanged()));
}

void QSoftwareModel::initAttributes()
{
}

quint16 QSoftwareModel::serverPort() const
{
    return (quint16)(m_serverPort->value().toUInt());
}

void QSoftwareModel::setServerPort(quint16 port)
{
    m_serverPort->setValue(port);
}

const QVariantList QSoftwareModel::deviceList() const
{
    return m_rserverModel->deviceList();
}

const QVariantList QSoftwareModel::upgradeInfoList() const
{
    QMutexLocker locker(m_mutex);
    QVariantList upgradeInfoList;
    QList<QUpgradeInfo *>::const_iterator i;

    for (i = m_upgradeInfoList.constBegin(); i != m_upgradeInfoList.constEnd();
         ++i) {
        QVariant upgradeInfoVar = (*i)->toVariant();
        upgradeInfoList.append(upgradeInfoVar);
    }

    return upgradeInfoList;
}
void QSoftwareModel::setUpgradeInfoList(const QVariantList upgradeInfoList)
{
    if (this->upgradeInfoList() == upgradeInfoList) {
        qprofileDebug(QtDebugMsg) << "upgradeInfo is the same";
        return;
    } else {
        qprofileDebug(QtDebugMsg) << "old = " << this->upgradeInfoList() << "new = " <<
                                  upgradeInfoList;
    }

    QMutexLocker locker(m_mutex);

    while (!m_upgradeInfoList.isEmpty())
        delete m_upgradeInfoList.takeFirst();

    QList<QVariant>::const_iterator i;

    for (i = upgradeInfoList.constBegin(); i != upgradeInfoList.constEnd(); ++i) {
        QUpgradeInfo *upgradeInfo = new QUpgradeInfo(this);
        upgradeInfo->fromVariant((*i).toMap());
        m_upgradeInfoList.append(upgradeInfo);
    }

    asynPropertyChangeNotify();
}

void QSoftwareModel::setUpgradeInfoMd5(const QString serialNumber,
                                       const QByteArray &md5)
{
    QMutexLocker locker(m_mutex);
    QList<QUpgradeInfo *>::iterator i;

    for (i = m_upgradeInfoList.begin(); i != m_upgradeInfoList.end();
         ++i) {
        if ((*i)->serialNumber() == serialNumber) {
            (*i)->setSoftwareMd5(md5);
            asynPropertyChangeNotify();
            break;
        }
    }
}

const QByteArray QSoftwareModel::getUpgradeInfoMd5(const QString serialNumber)
{
    QMutexLocker locker(m_mutex);
    QList<QUpgradeInfo *>::iterator i;

    for (i = m_upgradeInfoList.begin(); i != m_upgradeInfoList.end();
         ++i) {
        if ((*i)->serialNumber() == serialNumber) {
            return (*i)->softwareMd5();
        }
    }

    return QByteArray();
}

void QSoftwareModel::setUpgradeInfoStatus(const QString serialNumber,
        const quint32 status)
{
    QMutexLocker locker(m_mutex);
    QList<QUpgradeInfo *>::iterator i;

    for (i = m_upgradeInfoList.begin(); i != m_upgradeInfoList.end();
         ++i) {
        if ((*i)->serialNumber() == serialNumber) {
            (*i)->setUpgradeStatus(status);
            asynPropertyChangeNotify();
            break;
        }
    }
}

void QSoftwareModel::setUpgradeInfoImageSize(const QString serialNumber,
        const quint32 size)
{
    QMutexLocker locker(m_mutex);
    QList<QUpgradeInfo *>::iterator i;

    for (i = m_upgradeInfoList.begin(); i != m_upgradeInfoList.end();
         ++i) {
        if ((*i)->serialNumber() == serialNumber) {
            (*i)->setImageSize(size);
            asynPropertyChangeNotify();
            break;
        }
    }
}

const quint32 QSoftwareModel::getUpgradeInfoImageSize(const QString
        serialNumber)
{
    QMutexLocker locker(m_mutex);
    QList<QUpgradeInfo *>::iterator i;

    for (i = m_upgradeInfoList.begin(); i != m_upgradeInfoList.end();
         ++i) {
        if ((*i)->serialNumber() == serialNumber) {
            return (*i)->imageSize();
        }
    }

    return 0;
}

const quint32 QSoftwareModel::getUpgradeInfoDownloadedSize(
    const QString serialNumber)
{
    QMutexLocker locker(m_mutex);
    QList<QUpgradeInfo *>::iterator i;

    for (i = m_upgradeInfoList.begin(); i != m_upgradeInfoList.end();
         ++i) {
        if ((*i)->serialNumber() == serialNumber) {
            return (*i)->downloadedSize();
        }
    }

    return 0;
}

void QSoftwareModel::setUpgradeInfoDownloadedSize(const QString serialNumber,
        const quint32 size)
{
    QMutexLocker locker(m_mutex);
    QList<QUpgradeInfo *>::iterator i;

    for (i = m_upgradeInfoList.begin(); i != m_upgradeInfoList.end();
         ++i) {
        if ((*i)->serialNumber() == serialNumber) {
            (*i)->setDownloadedSize(size);
            asynPropertyChangeNotify();
            break;
        }
    }
}

const QString QSoftwareModel::getUpgradeInfoDeltaImageName(
    const QString serialNumber)
{
    QMutexLocker locker(m_mutex);
    QList<QUpgradeInfo *>::iterator i;

    for (i = m_upgradeInfoList.begin(); i != m_upgradeInfoList.end();
         ++i) {
        if ((*i)->serialNumber() == serialNumber) {
            return (*i)->getDeltaImageName();
        }
    }

    return QString();
}

const QString QSoftwareModel::getUpgradeInfoDownloadedPercentage(
    const QString serialNumber)
{
    QMutexLocker locker(m_mutex);
    QList<QUpgradeInfo *>::iterator i;

    for (i = m_upgradeInfoList.begin(); i != m_upgradeInfoList.end();
         ++i) {
        if ((*i)->serialNumber() == serialNumber) {
            return (*i)->downloadedPercentage();
        }
    }

    return QString();
}

void QSoftwareModel::setUpgradeInfoDownloadedPercentage(
    const QString serialNumber, const QString downloadedPercentage)
{
    QMutexLocker locker(m_mutex);
    QList<QUpgradeInfo *>::iterator i;

    for (i = m_upgradeInfoList.begin(); i != m_upgradeInfoList.end();
         ++i) {
        if ((*i)->serialNumber() == serialNumber) {
            (*i)->setDownloadedPercentage(downloadedPercentage);
            asynPropertyChangeNotify();
            break;
        }
    }
}

QSoftwareModel::~QSoftwareModel()
{
    m_mutex->lock();

    while (!m_upgradeInfoList.isEmpty())
        delete m_upgradeInfoList.takeFirst();

    m_mutex->unlock();
    delete m_mutex;
}

