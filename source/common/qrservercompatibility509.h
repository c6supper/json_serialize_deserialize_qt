#ifndef QRSERVERCOMPATIBILITY509_H
#define QRSERVERCOMPATIBILITY509_H

#include "qrservercompatibility503.h"
#include <QString>

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QRserver
{
    class QServerVersion;
    class QRserverCompatibility509 : public QRserverCompatibility503
    {
            Q_OBJECT

            Q_PROPERTY(bool testsetStatus READ testsetStatus)

        public:
            QRserverCompatibility509(const QString version = "5.0.9.0",
                                     QObject *parent = nullptr);
            ~QRserverCompatibility509();
            QRserverCompatibility509(const QRserverCompatibility509 &compatibility);
            virtual bool testsetStatus() const;

        public:
            static const QString FeatureTestsetStatus;
    };
}

Q_DECLARE_METATYPE(QRserver::QRserverCompatibility509)

#endif // QRSERVERCOMPATIBILITY509_H
