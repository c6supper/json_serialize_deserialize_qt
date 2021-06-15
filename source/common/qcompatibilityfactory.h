/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QCOMPATIBILITYFACTORY_H
#define QCOMPATIBILITYFACTORY_H

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
    class QCompatibilityFactory
    {
        private:
            static QCompatibilityFactory	*instance;
            QMap<qint32, qint32>			m_version2MetaTypeMap;

        public:
            static void *Create(const QString &version);
            static QCompatibilityFactory *Instance();

            template<typename T>
            void registerObject(qint32 version)
            {
                if (isVersionRegistered(version)) {
                    qprofileDebug(QtDebugMsg) << T::staticMetaObject.className() << " version =  "
                                              <<
                                              version <<
                                              " had been registered before! " << "typeId	 = " << qMetaTypeId<T>();
                    return;
                }

                int tId = qRegisterMetaType<T>(T::staticMetaObject.className());

                if (tId < 0) {
                    qprofileDebug(QtDebugMsg) << "Falied to register " <<
                                              T::staticMetaObject.className();
                } else {
                    qprofileDebug(QtDebugMsg) << "Register " << T::staticMetaObject.className() <<
                                              " successfully tId = " << tId;
                    m_version2MetaTypeMap[version] = tId;
                }
            }

            bool isVersionRegistered(qint32 version);
            qint32 version2Id(qint32 version);

            template<typename T>
            bool isRegisterObject()
            {
                int id = qMetaTypeId<T>();
                return QMetaType::isRegistered(id);
            }
    };
    template<typename T>
    class QCompatibilitySelfRegisteration
    {
        public:
            QCompatibilitySelfRegisteration(qint32 version)
            {
                QCompatibilityFactory::Instance()->registerObject<T>(version);
            }
    };
}

#endif // QCOMPATIBILITYFACTORY_H
