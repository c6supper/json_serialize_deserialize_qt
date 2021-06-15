/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */


#ifndef QMODELFACTORY_H
#define QMODELFACTORY_H

#include "qprofile_export.h"
#include "qprofile_debug.h"
#include "qprofile_global.h"
#include "qabstractcontainer.h"

#include <QObject>
#include <QHash>
#include <QPointer>
#include <QMutexLocker>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace QProfile
{
    class QAbstractModel;
}
QT_END_NAMESPACE

namespace QProfile
{
    class QModelFactory : public QObject
    {
            Q_OBJECT

        private:
            QHash<QString, QPointer<QAbstractModel> >	m_modelHash;
            QMutex											m_mutex;

        public:
            static QModelFactory *instance;
            static QModelFactory *Instance();

            template<typename T>
            static T *construct(QObject *parent)
            {
                const QString className = T::staticMetaObject.className();
                qint32 metaId = QMetaType::type(T::staticMetaObject.className());
                instance = Instance();
                T *pointer = nullptr;

                if (instance->contains(className)) {
                    pointer = (T *)((*instance)[className]).data();
                } else {
                    pointer = (T *)QMetaType::construct(metaId);
                }

                if (pointer == nullptr) {
                    qprofileDebug(QtDebugMsg) << "Failed to construct model = " << className;
                } else {
                    (*instance)[className] = pointer;
                    Q_ASSERT(parent != nullptr);

                    if (pointer->parent() != nullptr) {
                        qprofileDebug(QtDebugMsg) << "Parent = " <<
                                                  pointer->parent()->staticMetaObject.className()
                                                  << "had been set before!";
                    }

                    pointer->setParent(parent);

                    if (qobject_cast<QAbstractModel *>(parent) != nullptr) {
                        connect(pointer, SIGNAL(propertyChanged()), parent, SLOT(propertyChanged()));
                    }

                    if (qobject_cast<QAbstractContainer *>(parent) != nullptr) {
                        connect(pointer, SIGNAL(propertyChanged()), parent, SLOT(onModelChanged()));
                        connect(pointer, SIGNAL(commandPropertyChanged()), parent, SLOT(onCommand()));
                        connect(pointer, SIGNAL(triggerPropertyChanged()), parent, SLOT(onTrigger()));
                    }

                    if (qobject_cast<QWidget *>(parent) != nullptr) {
                        const QMetaObject *metaObj = parent->metaObject();

                        if (metaObj->indexOfSlot("onModelChanged()") < 0) {
                            qprofileDebug(QtFatalMsg) << "Parent = " << metaObj->className() <<
                                                      "haven't slot onModelChanged!";
                        }

                        connect(pointer, SIGNAL(propertyChanged()), parent, SLOT(onModelChanged()));
                        /*if (metaObj->indexOfSlot("issueCommand()") < 0) {
                            qprofileDebug(QtFatalMsg) << "Parent = " << metaObj->className() <<
                                                      "haven't slot onModelChanged!";
                        }

                        connect(pointer, SIGNAL(commandPropertyChanged()), parent,
                                SLOT(issueCommand()));*/
                    }
                }

                return pointer;
            }

            template<typename T>
            static const T *constModel(QObject *container)
            {
                const QString className = T::staticMetaObject.className();
                qint32 metaId = QMetaType::type(T::staticMetaObject.className());
                instance = Instance();
                T *pointer = nullptr;

                if (instance->contains(className)) {
                    pointer = (T *)((*instance)[className]).data();
                } else {
                    pointer = (T *)QMetaType::construct(metaId);
                }

                if (pointer == nullptr) {
                    qprofileDebug(QtFatalMsg) << "Failed to get const model = " << className;
                } else {
                    (*instance)[className] = pointer;
                    //                    Q_ASSERT(nullptr != container);
                    /*if (qobject_cast<QAbstractModel *>(container) != nullptr) {
                        connect(pointer, SIGNAL(propertyChanged()), container ,
                                SIGNAL(propertyChanged()));
                    }

                    if (qobject_cast<QAbstractContainer *>(container) != nullptr) {
                        connect(pointer, SIGNAL(propertyChanged()), container, SLOT(onModelChanged()));
                        connect(pointer, SIGNAL(commandPropertyChanged()), container,
                                SLOT(onCommand()));
                    }*/
                }

                return pointer;
            }

            QModelFactory(QObject *parent = nullptr);
            ~QModelFactory();

            QPointer<QAbstractModel> &operator[](const QString &key);
            bool contains(const QString &key);

            void debugPrint();

        protected slots:
            void quit();
    };
}

#endif // QMODELFACTORY_H
