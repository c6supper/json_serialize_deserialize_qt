/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */


#include "qmodelfactory.h"
#include "qabstractmodel.h"
#include "qabstractcontainer.h"
#include <QCoreApplication>

using namespace QProfile;

QModelFactory *QModelFactory::instance = nullptr;

QModelFactory::QModelFactory(QObject *parent) : QObject(parent)
{
    Q_ASSERT(parent != nullptr);
}

QModelFactory::~QModelFactory()
{
    QMutexLocker locker(&m_mutex);
    m_modelHash.clear();
}

void QModelFactory::quit()
{
    delete this;
}

QPointer<QAbstractModel> &QModelFactory::operator[](const QString &key)
{
    QMutexLocker locker(&m_mutex);

    if (!m_modelHash.contains(key))
        m_modelHash[key] = nullptr;

    return m_modelHash[key];
}

bool QModelFactory::contains(const QString &key)
{
    QMutexLocker locker(&m_mutex);
    return m_modelHash.contains(key);
}

void QModelFactory::debugPrint()
{
    QMutexLocker locker(&m_mutex);
    QHashIterator<QString, QPointer<QAbstractModel> > i(m_modelHash);

    while (i.hasNext()) {
        i.next();
        qprofileDebug(QtDebugMsg) << i.key() << ": " << i.value() << " " <<
                                  i.value().data() <<
                                  endl;
    }
}

QModelFactory *QModelFactory::Instance()
{
    if (instance == nullptr) {
        instance = new QModelFactory();
        connect(QCoreApplication::instance(), SIGNAL(aboutToQuit()), instance,
                SLOT(quit()));
    }

    return instance;
}
