/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */


#ifndef QXMLPARSER_H
#define QXMLPARSER_H

#include "qabstractparser.h"
#include "qprofile_global.h"

namespace QProfile
{
    class QXmlParser : public QAbstractParser
    {
            Q_OBJECT
        public:
            QXmlParser(QObject *parent = nullptr);
            virtual QVariant parse(const QByteArray &qProfileString, bool *ok = 0) ;
            virtual ~QXmlParser() {}
            virtual const QString tagKey()
            {
                return "xml";
            }
    };
}

#endif // QXMLPARSER_H
