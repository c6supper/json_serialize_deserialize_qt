/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QOBJECTFACTORY_H
#define QOBJECTFACTORY_H

#include "qprofile_global.h"
#include "qprofile_debug.h"
#include <QMetaType>

QT_BEGIN_NAMESPACE
class QString;
class QMetaObject;
QT_END_NAMESPACE

namespace QProfile
{
    class QObjectFactory
    {
        private:
            static QObjectFactory *instance;

        public:
            template<class T>
            void registerObject()
            {
                if (isRegisterObject<T>())
                    return;

                int tId = qRegisterMetaType<T>();

                if (tId < 0) {
                    qprofileDebug(QtDebugMsg) << "Falied to register " <<
                                              T::staticMetaObject.className();
                } else {
                    qprofileDebug(QtDebugMsg) << "Register " << T::staticMetaObject.className() <<
                                              " successfully tId = " << tId;
                }
            }

            template<class T>
            void registerObjectPointer()
            {
                if (isRegisterObject<T *>())
                    return;

                int tId = qRegisterMetaType<T *>();

                if (tId < 0) {
                    qprofileDebug(QtDebugMsg) << "Falied to register " <<
                                              T::staticMetaObject.className();
                } else {
                    qprofileDebug(QtDebugMsg) << "Register " << T::staticMetaObject.className() <<
                                              " successfully tId = " << tId;
                }
            }

            template<class T>
            void registerObjectContainer()
            {
                if (isRegisterObject<T>())
                    return;

                int tId = qRegisterMetaType<T>();

                if (tId < 0) {
                    qprofileDebug(QtDebugMsg) << "Falied to register container class.";
                } else {
                    qprofileDebug(QtDebugMsg) << "Register container class " <<
                                              " successfully tId = " << tId;
                }
            }

            template<class T>
            bool isRegisterObject()
            {
                int id = qMetaTypeId<T>();
                return QMetaType::isRegistered(id);
            }
            static QObjectFactory *Instance()
            {
                if (QObjectFactory::instance == nullptr)
                    QObjectFactory::instance = new QObjectFactory();

                return instance;
            }
    };
    template<typename T>
    class QObjectSelfRegisteration
    {
        public:
            QObjectSelfRegisteration()
            {
                QObjectFactory::Instance()->registerObject<T>();
            }
    };
    template<typename T>
    class QObjectPointerSelfRegisteration
    {
        public:
            QObjectPointerSelfRegisteration()
            {
                QObjectFactory::Instance()->registerObjectPointer<T>();
            }
    };
}

#endif // QOBJECTFACTORY_H
