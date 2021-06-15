
#ifndef QRSERVERCOMPATIBILITY503_H
#define QRSERVERCOMPATIBILITY503_H

#include "qrservercompatibility501.h"
#include <QString>

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QRserver
{
    class QServerVersion;
    class QRserverCompatibility503 : public QRserverCompatibility501
    {
            Q_OBJECT

            Q_PROPERTY(bool longTechId READ longTechId)

        public:
            QRserverCompatibility503(const QString version = "5.0.3.0",
                                     QObject *parent = nullptr);
            ~QRserverCompatibility503();
            QRserverCompatibility503(const QRserverCompatibility503 &compatibility);
            virtual bool longTechId() const;

        public:
            static const QString FeatureLongTechId;
    };
}

Q_DECLARE_METATYPE(QRserver::QRserverCompatibility503)

#endif // QRSERVERCOMPATIBILITY503_H
