/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QRSERVERCONTAINER_H
#define QRSERVERCONTAINER_H

#include "qabstractcontainer.h"
#include <QSharedPointer>
#include <QEvent>

QT_BEGIN_NAMESPACE
namespace QRserver
{
    class QAbstractBinMsg;
}
QT_END_NAMESPACE

namespace QRserver
{
    class QRserverContainer: public QProfile::QAbstractContainer
    {
            Q_OBJECT

        public:
            QRserverContainer(QObject *parent = nullptr, bool isRetainMessage = true);
            QRserverContainer(const QRserverContainer &container);

            virtual void copyChildren(const QAbstractContainer &container);
            virtual bool deSerialize(const QByteArray &qProfileString,
                                     QProfile::QAbstractParser *paser);
            static const QString qRserverEnvKey()
            {
                return ("QRSERVER_DIR");
            }

        protected slots:
            virtual bool initPropertiesMap(const QVariantMap &properties =
                                               QVariantMap());
            virtual void allocTopic();
            virtual	void allocModel() {}
            virtual void postponeInit();

        protected:
            void subscribeMessage(const quint32 type);
            void customEvent(QEvent *event);
            virtual void processMessage(QRserver::QAbstractBinMsg *message);
            quint16 publish(const QString &topic, const QByteArray &json);
    };
}

Q_DECLARE_METATYPE(QRserver::QRserverContainer)

#endif
