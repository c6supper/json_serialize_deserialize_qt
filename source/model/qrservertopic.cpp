/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include <QVariant>
#include "qrservertopic.h"

using namespace QProfile;
using namespace QRserver;

QRserverTopic::QRserverTopic(QObject *parent, QAbstractIpcClient *ipcClient,
                             const QString &relativeTopic, RestfulApis api) : QAbstractTopic(parent,
                                         ipcClient, relativeTopic, api)
{
}

const QString QRserverTopic::absoluteTopic(const QString &relativeTopic)
{
    QObject *parentObject = parent();
    QString absoluteTopic = relativeTopic;

    while (parentObject != nullptr) {
        QString tagName = parentObject->property("tagName").toString();

        if (!tagName.isEmpty()) {
            absoluteTopic.prepend(tagName + "\\");
        }

        parentObject = parentObject->parent();
    }

    return absoluteTopic;
}

QRserverTopic::~QRserverTopic() {}
