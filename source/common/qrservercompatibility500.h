
#ifndef QRSERVERCOMPATIBILITY500_H
#define QRSERVERCOMPATIBILITY500_H

#include "qrservercompatibility405.h"
#include <QString>

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QRserver
{
    class QServerVersion;
    class QRserverCompatibility500 : public QRserverCompatibility405
    {
            Q_OBJECT

            Q_PROPERTY(bool getSWVersionByType READ getSWVersionByType)

        public:
            QRserverCompatibility500(const QString version = "5.0.0.0",
                                     QObject *parent = nullptr);
            ~QRserverCompatibility500();
            QRserverCompatibility500(const QRserverCompatibility500 &compatibility);
            virtual bool getSWVersionByType() const;

        public:
            static const QString FeatureGetSWVersionByType;
    };
}

Q_DECLARE_METATYPE(QRserver::QRserverCompatibility500)

#endif // QRSERVERCOMPATIBILITY500_H
