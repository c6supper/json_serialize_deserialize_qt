
#ifndef QCMDFACTORY_H
#define QCMDFACTORY_H

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
    class QCMDFactory
    {
        private:
            static QCMDFactory *instance;
            QMap<QString, qint32> m_cmd2MetaTypeMap;

        public:

            static QCMDFactory *Instance();
            static void *create(const QVariant var);
            static bool IsCMDRegistered(QString cmd);

            template<typename T>
            void registerObject(QString cmd)
            {
                if (isCMDRegistered(cmd)) {
                    qprofileDebug(QtDebugMsg) << T::staticMetaObject.className() << " cmd =  " <<
                                              cmd << " had been registered before! " << "typeId = " << qMetaTypeId<T>();
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
                    m_cmd2MetaTypeMap[cmd] = tId;
                }
            }

            bool isCMDRegistered(QString cmd);
            qint32 cmd2Id(QString cmd);

            template<typename T>
            bool isRegisterObject()
            {
                int id = qMetaTypeId<T>();
                return QMetaType::isRegistered(id);
            }
    };

    template<typename T>
    class QCMDSelfRegisteration
    {
        public:
            QCMDSelfRegisteration(QString cmd)
            {
                QCMDFactory::Instance()->registerObject<T>(cmd);
            }
    };
}

#endif /* QCMDFACTORY_H */
