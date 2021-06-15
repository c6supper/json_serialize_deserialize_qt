/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */


#include "qmessageseries.h"

#if defined(BUILD_X86)
#include "qjson/parser.h"
#include "qjson/serializer.h"
#include "qjson/qobjecthelper.h"
#else
#include <QJson/Parser>
#include <QJson/Serializer>
#include <QJson/QObjectHelper>
#endif

using namespace QProfile;
using namespace QRserver;


QMessageSeries::QMessageSeries(QObject *parent) : QAbstractModel(parent)
{
    Q_ASSERT(parent != nullptr);
    initAttributes();
}

void QMessageSeries::initAttributes()
{
}

const QVariant QMessageSeries::device() const
{
    QVariant modeVar = QVariant(QJson::QObjectHelper::qobject2qvariant(
                                    (QObject *)(&m_device)));
    return modeVar;
}

void QMessageSeries::setDevice(const QVariant device)
{
    QJson::QObjectHelper::qvariant2qobject(device.toMap(), &m_device);
}

const QVariantList &QMessageSeries::messageList()
{
    return m_messageList;
}

void QMessageSeries::setMessageList(const QVariantList &messageList)
{
    QMutexLocker locker(&m_mutex);
    m_messageList = messageList;
}

bool QMessageSeries::appendMessage(const quint32 parentMessageId,
                                   QAbstractBinMsg *message)
{
    QMutexLocker locker(&m_mutex);
    message->setId(m_messageList.count() + 1);
    m_messageList.append(message->toVariant());
    bool ok = relateMessage(parentMessageId, message->id());

    if (!ok)
        m_messageList.removeLast();

    return ok;
}

//not thread-safe should not call without mutex lock
bool QMessageSeries::relateMessage(quint32 parentMessageId,
                                   quint32 childMessageId)
{
    if (m_mutex.tryLock()) {
        qprofileDebug(QtFatalMsg) <<
                                  "It is not thread-safe, should not call without mutex lock outside this function.";
    }

    bool isRelated = false;

    if (parentMessageId == 0) {
        QVariantList messageIdList;
        messageIdList.append(QVariant(childMessageId));
        m_messageRouteList.append(QVariant(messageIdList));
        isRelated = true;
    } else {
        int routeIdx = 0;
        QVariantList containedList;

        for (; routeIdx < m_messageRouteList.size(); routeIdx++) {
            QVariantList messageIdList = m_messageRouteList[routeIdx].toList();

            if (messageIdList.last().toUInt() == parentMessageId) {
                messageIdList.append(QVariant(childMessageId));
                m_messageRouteList.replace(routeIdx, QVariant(messageIdList));
                isRelated = true;
            } else {
                if (messageIdList.contains(QVariant(parentMessageId))) {
                    containedList.append(QVariant(messageIdList));
                }
            }
        }

        if (!isRelated && containedList.size() <= 0) {
            QVariantList messageIdList;
            messageIdList.append(QVariant(parentMessageId));
            messageIdList.append(QVariant(childMessageId));
            m_messageRouteList.append(QVariant(messageIdList));
            isRelated = true;
        }

        if (containedList.size() > 0) {
            for (int routeIdx = 0; routeIdx < containedList.size(); routeIdx++) {
                QVariantList messageIdList = containedList[routeIdx].toList();

                if (messageIdList.contains(QVariant(parentMessageId))) {
                    QVariantList::iterator i;

                    for (i = messageIdList.begin(); i != messageIdList.end(); ++i) {
                        if (*i == QVariant(parentMessageId)) {
                            messageIdList.erase(++i, messageIdList.end());
                            break;
                        }
                    }

                    messageIdList.append(QVariant(childMessageId));

                    if (!m_messageRouteList.contains(QVariant(messageIdList))) {
                        m_messageRouteList.append(QVariant(messageIdList));
                        isRelated = true;
                    }
                }
            }
        }
    }

    return isRelated;
}

const quint32 QMessageSeries::messageIdx()
{
    return m_messageList.count();
}

void QMessageSeries::appendMessageRoute(QList<quint32> routeList)
{
    QMutexLocker locker(&m_mutex);
    QVariantList routeListListVar;
    QList<quint32>::const_iterator iter;

    for (iter = routeList.constBegin(); iter != routeList.constEnd(); ++iter) {
        routeListListVar.append(*iter);
    }

    m_messageRouteList.append(QVariant(routeListListVar));
}

const QVariantList &QMessageSeries::messageRouteList()
{
    return m_messageRouteList;
}

const QMap<quint32, QVariantMap> QMessageSeries::getMessageMap()
{
    QMutexLocker locker(&m_mutex);
    QMap<quint32, QVariantMap> messageMap;
    QVariantList::const_iterator iter;

    for (iter = m_messageList.constBegin(); iter != m_messageList.constEnd();
         ++iter) {
        QVariantMap messageVar = iter->toMap();

        if (!messageVar.contains("id")) {
            qprofileDebug(QtDebugMsg) << "Invalid message = " << messageVar;
        }

        quint32 id = messageVar["id"].toUInt();
        messageVar.remove("id");
        messageMap[id] = messageVar;
    }

    return messageMap;
}

const QList<QList<quint32> > QMessageSeries::getMessageRouteList()
{
    QMutexLocker locker(&m_mutex);
    QList<QList<quint32> > messageRouteList;
    QVariantList::const_iterator iter;

    for (iter = m_messageRouteList.constBegin();
         iter != m_messageRouteList.constEnd(); ++iter) {
        QList<quint32> messageRoute;
        QVariantList::const_iterator routeIter;
        QVariantList routeVarList = iter->toList();

        for (routeIter = routeVarList.constBegin();
             routeIter != routeVarList.constEnd(); ++routeIter) {
            messageRoute.append(routeIter->toUInt());
        }

        messageRouteList.append(messageRoute);
    }

    return messageRouteList;
}

void QMessageSeries::setMessageRouteList(const QVariantList &messageRouteList)
{
    QMutexLocker locker(&m_mutex);
    m_messageRouteList = messageRouteList;
}

QMessageSeries::~QMessageSeries()
{
    QMutexLocker locker(&m_mutex);
    m_messageRouteList.clear();
    m_messageList.clear();
}

bool QMessageSeries::isValid()
{
    QList<QList<quint32> > messageRouteList = getMessageRouteList();
    QMap<quint32, QVariantMap> messageMap = getMessageMap();
    quint32 firstIdInRoute = 0;

    if (!device().isValid())
        return false;

    for (int idx = 0; idx < messageRouteList.size(); ++idx) {
        QList<quint32> messageRoute = messageRouteList.at(idx);

        if (messageRoute.count() % 2 != 0 || messageRoute.count() <= 0)
            return false;

        if (firstIdInRoute == 0)
            firstIdInRoute = messageRoute.at(0);

        if (firstIdInRoute != messageRoute.at(0)) {
            qprofileDebug(QtDebugMsg) << "Message Id must start at the same id." ;
            return false;
        }

        QList<quint32>::const_iterator iterator;

        for (iterator = messageRoute.constBegin(); iterator != messageRoute.constEnd();
             ++iterator) {
            if (!messageMap.contains(*iterator)) {
                qprofileDebug(QtDebugMsg) << "Message whose Id = " << *iterator <<
                                          " is missing.";
                return false;
            }
        }
    }

    return (messageRouteList.count() > 0 && messageMap.count() > 0);
}
