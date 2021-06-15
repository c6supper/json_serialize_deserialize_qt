
#ifndef QRSERVERCOMPATIBILITY501_H
#define QRSERVERCOMPATIBILITY501_H

#include "qrservercompatibility500.h"
#include <QString>

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QRserver
{
    class QServerVersion;
    class QRserverCompatibility501 : public QRserverCompatibility500
    {
            Q_OBJECT

            Q_PROPERTY(bool registerChassisOnly READ registerChassisOnly)

        public:
            QRserverCompatibility501(const QString version = "5.0.1.0",
                                     QObject *parent = nullptr);
            ~QRserverCompatibility501();
            QRserverCompatibility501(const QRserverCompatibility501 &compatibility);
            virtual bool registerChassisOnly() const;

        public:
            static const QString FeatureRegisterChassisOnly;
    };
}

Q_DECLARE_METATYPE(QRserver::QRserverCompatibility501)

#endif // QRSERVERCOMPATIBILITY501_H
