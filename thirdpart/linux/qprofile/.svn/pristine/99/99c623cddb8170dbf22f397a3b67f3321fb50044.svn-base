/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QGENERICCOMMAND_H
#define QGENERICCOMMAND_H

#include "qabstractcommand.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QProfile
{
    class QGenericCommand : public QAbstractCommand
    {
            Q_OBJECT

        public:
            QGenericCommand(QObject *parent = nullptr);
            QGenericCommand(QObject *parent, const QString &method ,
                            const QString &topic = "",
                            const QVariant &resource = QVariant());
            ~QGenericCommand();

            virtual bool isValid();
            virtual void pureVirtualMock() {};
    };
}

Q_DECLARE_METATYPE(QProfile::QGenericCommand)

#endif // QGENERICCOMMAND_H
