
#ifndef QRSERVERCOMPATIBILITY405_H
#define QRSERVERCOMPATIBILITY405_H

#include "qrservercompatibility.h"
#include <QString>

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QRserver
{
    class QServerVersion;
    class QRserverCompatibility405 : public QRserverCompatibility
    {
            Q_OBJECT
            Q_PROPERTY(bool longFileName READ longFileName)

        public:
            QRserverCompatibility405(const QString version = "4.0.5.0",
                                     QObject *parent = nullptr);
            ~QRserverCompatibility405();
            QRserverCompatibility405(const QRserverCompatibility405 &compatibility);
            virtual bool longFileName() const;

        public:
            static const QString FeatureLongFileName;
            static const qint32	 MaxLongFileName = 128;
            static const qint32	 MaxFileName = 26;
    };
}

Q_DECLARE_METATYPE(QRserver::QRserverCompatibility405)

#endif // QRSERVERCOMPATIBILITY405_H
