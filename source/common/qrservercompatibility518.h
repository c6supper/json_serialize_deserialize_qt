/*
 * qrservercompatibility518.h
 *
 *  Created on: Jul 30, 2020
 *      Author: duyi
 */

#ifndef QRSERVERCOMPATIBILITY518_H
#define QRSERVERCOMPATIBILITY518_H

#include "qrservercompatibility510.h"
#include <QString>

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QRserver
{
    class QServerVersion;
    class QRserverCompatibility518 : public QRserverCompatibility510
    {
            Q_OBJECT

            Q_PROPERTY(bool updateTechName READ updateTechName)
            Q_PROPERTY(bool uploadProfileInfoList READ uploadProfileInfoList)

        public:
            QRserverCompatibility518(const QString version = "5.1.8.0",
                                     QObject *parent = nullptr);
            ~QRserverCompatibility518();
            QRserverCompatibility518(const QRserverCompatibility518 &compatibility);
            virtual bool updateTechName() const;
            virtual bool uploadProfileInfoList() const;

        public:
            static const QString FeatureUpdateTechName;
            static const QString FeatureUploadProfileInfoList;
    };
}

Q_DECLARE_METATYPE(QRserver::QRserverCompatibility518)

#endif /* QRSERVERCOMPATIBILITY518_H */
