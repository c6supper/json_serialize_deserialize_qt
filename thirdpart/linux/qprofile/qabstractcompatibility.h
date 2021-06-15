
#ifndef QABSTRACTCOMPATIBILITY_H
#define QABSTRACTCOMPATIBILITY_H

#include "qprofile_export.h"
#include "qprofile_debug.h"
#include "qprofile_global.h"
#include "qabstractjsonelement.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

#include <QMutex>

namespace QProfile
{
    class QAbstractCompatibility : public QAbstractJsonElement
    {
            Q_OBJECT

        public:
            QAbstractCompatibility(QObject *parent = nullptr);
            ~QAbstractCompatibility();
            virtual void pureVirtualMock() {};
            static void SetInstance(QAbstractCompatibility *instance);
            static QAbstractCompatibility *Instance();
            static const QString GetCompatibilityClassName();
            static const qint32 GetCompatibilityMetaType();
            bool isCompatible(const QString feature);

        protected slots:
            virtual void initialize() = 0;

        private:
            static QAbstractCompatibility 		*m_instance;
            static QMutex						m_mutex;

        public:
            static const qint32 InvalidCompatibilityMetaType = QMetaType::Void;
    };
}

#endif // QABSTRACTCOMPATIBILITY_H
