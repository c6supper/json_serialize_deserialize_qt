/*
 * qrservercompatibility605.h
 *
 *  Created on: Nov 24, 2020
 *      Author: duyi
 */

#ifndef QRSERVERCOMPATIBILITY605_H
#define QRSERVERCOMPATIBILITY605_H
#include "qrservercompatibility518.h"
#include <QString>

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QRserver
{
    class QServerVersion;
    class QRserverCompatibility605 : public QRserverCompatibility518
    {
            Q_OBJECT

            Q_PROPERTY(bool upgradeViaHttp READ upgradeViaHttp)

        public:
            QRserverCompatibility605(const QString version = "6.0.5.0",
                                     QObject *parent = nullptr);
            ~QRserverCompatibility605();
            QRserverCompatibility605(const QRserverCompatibility605 &compatibility);
            virtual bool upgradeViaHttp() const;

        public:
            static const QString FeatureUpgradeViaHttp;
    };
}

Q_DECLARE_METATYPE(QRserver::QRserverCompatibility605)


#endif /* QRSERVERCOMPATIBILITY605_H */
