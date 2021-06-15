
#ifndef QRSERVERCOMPATIBILITY_H
#define QRSERVERCOMPATIBILITY_H

#include "qabstractcompatibility.h"
#include <QString>

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QRserver
{
    class QServerVersion;
    class QRserverCompatibility : public QProfile::QAbstractCompatibility
    {
            Q_OBJECT

            Q_PROPERTY(QString version READ version)

        public:
            QRserverCompatibility(const QString version = defaultServerVersion,
                                  QObject *parent = nullptr);
            ~QRserverCompatibility();
            QRserverCompatibility(const QRserverCompatibility &compatibility);
            const QString version() const;

        protected slots:
            virtual void initialize();

        private:
            QServerVersion	*m_serverVersion;
            static const QString defaultServerVersion;
    };
}

Q_DECLARE_METATYPE(QRserver::QRserverCompatibility)

#endif // QRSERVERCOMPATIBILITY_H
