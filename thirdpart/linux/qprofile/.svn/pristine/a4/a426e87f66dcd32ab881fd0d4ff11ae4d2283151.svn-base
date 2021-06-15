
#ifndef QABSTRACTJSONELEMENT_H
#define QABSTRACTJSONELEMENT_H

#include "qprofile_export.h"
#include "qprofile_debug.h"
#include "qprofile_global.h"
#include <QObject>
#include <QStringList>

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QProfile
{
    class QAbstractJsonElement : public QObject
    {
            Q_OBJECT

        protected:
            virtual const QStringList ignoredProperties() const;
            static const QStringList propertyList();

        public:
            QAbstractJsonElement(QObject *parent = nullptr);
            QAbstractJsonElement(const QAbstractJsonElement &cmd);
            ~QAbstractJsonElement();

            static const QVariant GetProperty(const QVariant variant ,
                                              const QString &propertyName);

            virtual void pureVirtualMock() = 0;
            virtual void fromVariant(const QVariant &variant);
            virtual const QVariant toVariant(const QStringList &ignoreList = QStringList())
            const;
            virtual const QByteArray toJson(const QStringList &ignoreList = QStringList())
            const;
            virtual bool fromFile(const QString &pathName);
            virtual bool toFile(const QString &pathName,
                                const QStringList &ignoreList = QStringList()) const;
            virtual QAbstractJsonElement &operator=(const QAbstractJsonElement &other);
    };
}

#endif // QABSTRACTJSONELEMENT_H
