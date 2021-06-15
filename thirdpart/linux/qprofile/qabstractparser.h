/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QABSTRACTPARSER_H
#define QABSTRACTPARSER_H

#include <QObject>
#include <QVariant>
#include <QIODevice>
#include "qprofile_global.h"

namespace QProfile
{
    class QAbstractParser : public QObject
    {
            Q_OBJECT

        public:
            explicit QAbstractParser(QObject *parent = nullptr);
            virtual ~QAbstractParser() {}

            virtual QVariant parse(QIODevice *io, bool *ok = 0);
            virtual QVariant parse(const QByteArray &qProfileString, bool *ok = 0) = 0;
            virtual QString errorString() const;
            virtual const QString tagKey() = 0;
            virtual int errorLine() const;
            void reset();

        protected:
            void setError(const QString &errorMsg, int line);
            bool m_error;
            int m_errorLine;
            int m_errorColumn;
            QString m_errorMsg;
    };
}

#endif // QABSTRACTPARSER_H
