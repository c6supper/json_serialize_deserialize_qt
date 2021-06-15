/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qmessagefactory.h"
#if defined(BUILD_X86)
#include "qjson/parser.h"
#include "qjson/serializer.h"
#include "qjson/qobjecthelper.h"
#else
#include <QJson/Parser>
#include <QJson/Serializer>
#include <QJson/QObjectHelper>
#endif

using namespace QRserver;

QMessageFactory *QMessageFactory::instance = nullptr;

void *QMessageFactory::create(qint32 type, const char *data,
                              quint32 length)
{
    if (!QMessageFactory::Instance()->isTypeRegistered(type)) {
        qprofileDebug(QtDebugMsg) << "type =  " << type <<
                                  " haven't been registered before!";
        return nullptr;
    }

    qint32 metaId = QMessageFactory::Instance()->type2Id(type);

    if (metaId == QMetaType::Void) {
        qprofileDebug(QtDebugMsg) << "Tried to create unregistered type =  " << type;
        return nullptr;
    }

    void *message = QMetaType::construct(metaId);
    QByteArray messageArray(data, length);
    QDataStream in(messageArray);

    if (QMetaType::load(in, metaId, message)) {
        QObject *abstractMessage = static_cast<QObject *>(message);
        abstractMessage->setProperty("messageType", QVariant(type));
        return message;
    }

    qprofileDebug(QtDebugMsg) << "Failed to load type =  " << type;
    QMetaType::destroy(metaId, message);
    return nullptr;
}

void *QMessageFactory::create(const QVariantMap messageVar)
{
    qint32 type = 0;

    if (messageVar.contains("messageType"))
        type = messageVar["messageType"].toInt();

    if (type == 0) {
        qprofileDebug(QtDebugMsg) << "Invalid message variant = " << messageVar;
        return nullptr;
    }

    if (!QMessageFactory::Instance()->isTypeRegistered(type)) {
        qprofileDebug(QtDebugMsg) << "messageType =  " << type <<
                                  " haven't been registered before!";
        return nullptr;
    }

    qint32 metaId = QMessageFactory::Instance()->type2Id(type);

    if (metaId == QMetaType::Void) {
        qprofileDebug(QtDebugMsg) << "Tried to create unregistered messageType =  " <<
                                  type;
        return nullptr;
    }

    void *message = QMetaType::construct(metaId);

    if (message != nullptr) {
        QJson::QObjectHelper::qvariant2qobject(messageVar, (QObject *)message);
        return message;
    }

    qprofileDebug(QtDebugMsg) << "Failed to load messageType =  " << type;
    QMetaType::destroy(metaId, message);
    return nullptr;
}

bool QMessageFactory::isTypeRegistered(qint32 type)
{
    return m_type2MetaTypeMap.contains(type);
}

qint32 QMessageFactory::type2Id(qint32 type)
{
    if (!isTypeRegistered(type))
        return QMetaType::Void;

    return m_type2MetaTypeMap[type];
}

QMessageFactory *QMessageFactory::Instance()
{
    if (QMessageFactory::instance == nullptr)
        QMessageFactory::instance = new QMessageFactory();

    return instance;
}
