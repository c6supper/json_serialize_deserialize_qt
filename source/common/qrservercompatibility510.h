#ifndef QRSERVERCOMPATIBILITY510_H
#define QRSERVERCOMPATIBILITY510_H

#include "qrservercompatibility509.h"
#include <QString>

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QRserver
{
    class QServerVersion;
    class QRserverCompatibility510 : public QRserverCompatibility509
    {
            Q_OBJECT

            Q_PROPERTY(bool uploadProfileAck READ uploadProfileAck)

        public:
            QRserverCompatibility510(const QString version = "5.1.0.0",
                                     QObject *parent = nullptr);
            ~QRserverCompatibility510();
            QRserverCompatibility510(const QRserverCompatibility510 &compatibility);
            virtual bool uploadProfileAck() const;

        public:
            static const QString FeatureUploadProfileAck;
    };
}

Q_DECLARE_METATYPE(QRserver::QRserverCompatibility510)



#endif /* QRSERVERCOMPATIBILITY510_H */
