/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */


#include "qmacattribute.h"
#include "qprofile_debug.h"
#include "qobjectfactory.h"
#include <QtCore/QVariant>
#include <QStringList>

using namespace QProfile;

QObjectSelfRegisteration<QMacAttribute> registerQMacAttribute;

QMacAttribute::QMacAttribute(QObject *parent)
    : QAbstractAttribute(QVariant("00:00:00:00:00:00"), QVariant::String, parent)
{
    setRegExp("(([a-f0-9]{2}:)|([a-f0-9]{2}-)){5}[a-f0-9]{2}");
}

QMacAttribute::QMacAttribute(const QVariant &value, QObject *parent)
    : QAbstractAttribute(value, QVariant::String, parent)
{
    setRegExp("(([a-f0-9]{2}:)|([a-f0-9]{2}-)){5}[a-f0-9]{2}");
}

void QMacAttribute::toChar(char *mac)
{
    if (mac != nullptr) {
        QStringList macStringList = value().toString().split(QRegExp(":|-"));
        QStringList::const_iterator i;

        for (i = macStringList.constBegin(); i != macStringList.constEnd(); ++i) {
            if (i->length() > 0) {
                bool ok = false;
                uint hex = i->toUInt(&ok, 16);
                *mac = (char)hex;
                mac += 1;
            }
        }
    }
}

void QMacAttribute::fromChar(char *mac, int length)
{
    if (mac != nullptr) {
        QString macString;

        for (int i = 0; i < length; i++) {
            macString += QString("%1:").arg((quint32)mac[i], 2,
                                            16, QLatin1Char('0'));
        }

        macString = macString.replace(macString.lastIndexOf(":"), 1, "");
        setValue(QVariant(macString));
    }
}
