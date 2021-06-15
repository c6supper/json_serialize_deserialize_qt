
#ifndef QEXECUTORFACTORY_H
#define QEXECUTORFACTORY_H

#include "qprofile_global.h"
#include "qprofile_debug.h"
#include <QMetaType>
#include <QByteArray>
#include <QMap>

QT_BEGIN_NAMESPACE
class QString;
class QMetaObject;
QT_END_NAMESPACE

namespace QRserver
{
    class QExecutorFactory
    {
        private:
            static QExecutorFactory *instance;
            QMap<QString, qint32> m_executor2MetaTypeMap;

        public:

            static QExecutorFactory *Instance();
            static void *create(const QVariant var);

            template<typename T>
            void registerObject(QString executor)
            {
                if (isExecutorRegistered(executor)) {
                    qprofileDebug(QtDebugMsg) << T::staticMetaObject.className() << " Executor =  "
                                              <<
                                              executor << " had been registered before! " << "typeId = " << qMetaTypeId<T>();
                    return;
                }

                int tId = qRegisterMetaType<T>(T::staticMetaObject.className());
                //qRegisterMetaTypeStreamOperators<T>(T::staticMetaObject.className());

                if (tId < 0) {
                    qprofileDebug(QtDebugMsg) << "Falied to register " <<
                                              T::staticMetaObject.className();
                } else {
                    qprofileDebug(QtDebugMsg) << "Register " << T::staticMetaObject.className() <<
                                              " successfully tId = " << tId;
                    m_executor2MetaTypeMap[executor] = tId;
                }
            }

            bool isExecutorRegistered(QString executor);
            qint32 executor2Id(QString executor);

            template<typename T>
            bool isRegisterObject()
            {
                int id = qMetaTypeId<T>();
                return QMetaType::isRegistered(id);
            }
    };

    template<typename T>
    class QExecutorSelfRegisteration
    {
        public:
            QExecutorSelfRegisteration(QString executor)
            {
                QExecutorFactory::Instance()->registerObject<T>(executor);
            }
    };
}

#endif /* QEXECUTORFACTORY_H */