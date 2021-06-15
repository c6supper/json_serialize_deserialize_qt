/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qcompatibilityfactory.h"
#include "qserverversion.h"

using namespace QRserver;

QCompatibilityFactory *QCompatibilityFactory::instance = nullptr;

void *QCompatibilityFactory::Create(const QString &version)
{
    QServerVersion serverVersion(version);

    if (!serverVersion.isValid()) {
        qprofileDebug(QtCriticalMsg) << "Invalid version =  " << version;
        return nullptr;
    }

    qint32 metaId = QCompatibilityFactory::Instance()->version2Id(
                        serverVersion.version());

    if (metaId == QMetaType::Void) {
        qprofileDebug(QtDebugMsg) << "Tried to create unregistered version =  " <<
                                  version;
        return nullptr;
    }

    void *compatibility = QMetaType::construct(metaId);

    if (compatibility != nullptr) {
        return compatibility;
    }

    qprofileDebug(QtDebugMsg) << "Failed to load version =  " << version;
    return nullptr;
}

bool QCompatibilityFactory::isVersionRegistered(qint32 version)
{
    return m_version2MetaTypeMap.contains(version);
}

qint32 QCompatibilityFactory::version2Id(qint32 version)
{
    QList<qint32> listVerions = m_version2MetaTypeMap.uniqueKeys();
    qSort(listVerions.begin(), listVerions.end());

    if (listVerions.size() <= 0) {
        return QServerVersion::invalidVersionIntger;
    }

    int idx = 0;
    qprofileDebug(QtDebugMsg) << listVerions << version;

    for (; idx < listVerions.size(); ++idx) {
        if (listVerions.at(idx) > version) {
            break;
        }
    }

    if (idx <= 0) {
        return m_version2MetaTypeMap[listVerions.at(0)];
    }

    return m_version2MetaTypeMap[listVerions.at(idx - 1)];
}

QCompatibilityFactory *QCompatibilityFactory::Instance()
{
    if (QCompatibilityFactory::instance == nullptr)
        QCompatibilityFactory::instance = new QCompatibilityFactory();

    return instance;
}
