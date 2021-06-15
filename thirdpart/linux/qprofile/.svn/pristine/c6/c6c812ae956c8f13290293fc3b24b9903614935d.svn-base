/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */


#include "qabstractparser.h"
#include <QBuffer>

using namespace QProfile;

QAbstractParser::QAbstractParser(QObject *parent) : QObject(parent)
{
}

void QAbstractParser::setError(const QString &errorMsg, int errorLine)
{
    m_error = true;
    m_errorMsg = errorMsg;
    m_errorLine = errorLine;
}

void QAbstractParser::reset()
{
    m_error = false;
    m_errorLine = 0;
    m_errorColumn = 0;
    m_errorMsg.clear();
}

QVariant QAbstractParser::parse(QIODevice *io, bool *ok)
{
    reset();

    if (!io->isOpen()) {
        if (!io->open(QIODevice::ReadOnly)) {
            if (ok != 0)
                *ok = false;

            qCritical("Error opening device");
            return QVariant();
        }
    }

    if (!io->isReadable()) {
        if (ok != 0)
            *ok = false;

        qCritical("Device is not readable");
        io->close();
        return QVariant();
    }

    if (io->atEnd()) {
        if (ok != 0)
            *ok = false;

        setError(QLatin1String("No data"), 0);
        io->close();
        return QVariant();
    }

    QByteArray buffer = io->readAll();
    QVariant result = parse(buffer, ok);
    io->close();
    return result;
}

QString QAbstractParser::errorString() const
{
    return m_errorMsg;
}

int QAbstractParser::errorLine() const
{
    return m_errorLine;
}
