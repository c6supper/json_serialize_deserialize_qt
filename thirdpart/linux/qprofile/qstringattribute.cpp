/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qstringattribute.h"
#include "qobjectfactory.h"
#include <QtCore/QVariant>

using namespace QProfile;

QObjectSelfRegisteration<QStringAttribute> registerQStringAttribute;

QStringAttribute::QStringAttribute(const QVariant &value, QObject *parent) :
    QAbstractAttribute(value, QVariant::String, parent)
{
}

QStringAttribute::QStringAttribute(QObject *parent) :
    QAbstractAttribute("", QVariant::String, parent)
{
}

void QStringAttribute::toChar(char *string, int length)
{
    if (string != nullptr && length > 0) {
        std::string stdString = value().toString().toStdString();

        if (length < value().toString().length()) {
            stdString.copy(string, length);
        } else {
            stdString.copy(string, value().toString().length());
        }

        string[length - 1] = '\0';
    }
}

void QStringAttribute::StringToChar(const QString &qString, char *string,
                                    int length)
{
    if (string != nullptr && length > 0) {
        std::string stdString = qString.toStdString();

        if (length < qString.length()) {
            stdString.copy(string, length);
        } else {
            stdString.copy(string, qString.length());
        }

        string[length - 1] = '\0';
    }
}
