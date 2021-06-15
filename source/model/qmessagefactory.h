/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QMESSAGEFACTORY_H
#define QMESSAGEFACTORY_H

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
    class QMessageFactory
    {
        private:
            static QMessageFactory 		*instance;
            QMap<qint32, qint32>		m_type2MetaTypeMap;

        public:

            static void *create(qint32 type, const char *data, quint32 length);
            static QMessageFactory *Instance();
            static void *create(const QVariantMap messageVar);

            template<typename T>
            void registerObject(qint32 type)
            {
                if (isTypeRegistered(type)) {
                    qprofileDebug(QtDebugMsg) << T::staticMetaObject.className() << " type =  " <<
                                              type <<
                                              " had been registered before! " << "typeId	 = " << qMetaTypeId<T>();
                    return;
                }

                int tId = qRegisterMetaType<T>(T::staticMetaObject.className());
                qRegisterMetaTypeStreamOperators<T>(T::staticMetaObject.className());

                if (tId < 0) {
                    qprofileDebug(QtDebugMsg) << "Falied to register " <<
                                              T::staticMetaObject.className();
                } else {
                    qprofileDebug(QtDebugMsg) << "Register " << T::staticMetaObject.className() <<
                                              " successfully tId = " << tId;
                    m_type2MetaTypeMap[type] = tId;
                }
            }

            template<typename T>
            void registerObjectPointer(qint32 type)
            {
                if (isTypeRegistered(type)) {
                    qprofileDebug(QtDebugMsg) << T::staticMetaObject.className() << " type =  " <<
                                              type <<
                                              " had been registered before!";
                    return;
                }

                QByteArray typeName(T::staticMetaObject.className());
                typeName += " *";
                int tId = qRegisterMetaType<T *>(typeName.constData());
                qRegisterMetaTypeStreamOperators<T *>(typeName.constData());

                if (tId < 0) {
                    qprofileDebug(QtDebugMsg) << "Falied to register " <<
                                              T::staticMetaObject.className();
                } else {
                    qprofileDebug(QtDebugMsg) << "Register " << T::staticMetaObject.className() <<
                                              " successfully tId = " << tId;
                    m_type2MetaTypeMap[type] = tId;
                }
            }

            bool isTypeRegistered(qint32 type);
            qint32 type2Id(qint32 type);

            template<typename T>
            bool isRegisterObject()
            {
                int id = qMetaTypeId<T>();
                return QMetaType::isRegistered(id);
            }
    };
    template<typename T>
    class QMessageSelfRegisteration
    {
        public:
            QMessageSelfRegisteration(qint32 type)
            {
                QMessageFactory::Instance()->registerObject<T>(type);
            }
    };
    /*template<typename T>
    class QMessagePointerSelfRegisteration
    {
        public:
            QMessagePointerSelfRegisteration(qint32 type)
            {
                QMessageFactory::Instance()->registerObjectPointer<T>(type);
            }
    };*/
}

#endif // QMESSAGEFACTORY_H
