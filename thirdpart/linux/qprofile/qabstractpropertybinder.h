/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */


#ifndef QABSTRACTPROPERTYBINDER_H
#define QABSTRACTPROPERTYBINDER_H

#include "qprofile_export.h"
#include "qprofile_global.h"
#include <QObject>
#include <QMutexLocker>

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QProfile
{
    class QAbstractPropertyBinder : public QObject
    {
            Q_OBJECT

        public:
            QAbstractPropertyBinder(QObject *view, QObject *model);
            ~QAbstractPropertyBinder();

        public:
            static const QString PropertyChangeNotifySignalName;

        private slots:
            void propertyChanged();
            void destroyBinder();

        private:
            QObject					*m_model;
            QObject					*m_view;
            QMutex					*m_mutex;

        private:
            void copyProperty(QObject *src, QObject *dest);
            void bindPropertyChanged(QObject *notifier);
            bool isIgnoredProperty(const QString property);
            static const QStringList IgnoredPropertyList();

        private:
            static const QStringList ignoredPropertyList;
    };
}
#endif // QABSTRACTPROPERTYBINDER_H
