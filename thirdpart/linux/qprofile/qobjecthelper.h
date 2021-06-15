/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QOBJECTHELPER_H
#define QOBJECTHELPER_H

#include <QtCore/QLatin1String>
#include <QtCore/QStringList>
#include <QtCore/QVariantMap>
#include "qprofile_global.h"

QT_BEGIN_NAMESPACE
class QObject;
QT_END_NAMESPACE

namespace QProfile
{
    /**
    * @brief Class used to convert QObject into QVariant and vivce-versa.
    * During these operations only the class attributes defined as properties will
    * be considered.
    * Properties marked as 'non-stored' will be ignored.
    *
      \endcode

      \sa Parser
      \sa Serializer
    */

    class QObjectHelper
    {
        public:
            QObjectHelper();
            ~QObjectHelper();

            /**
            * This method converts a QObject instance into a QVariantMap.
            *
            * @param object The QObject instance to be converted.
            * @param ignoredProperties Properties that won't be converted.
            */
            static QVariantMap qobject2qvariant(const QObject *object,
                                                const QStringList &ignoredProperties = QStringList(QString(
                                                        QLatin1String("objectName"))));

            /**
            * This method converts a QVariantMap instance into a QObject
            *
            * @param variant Attributes to assign to the object.
            * @param object The QObject instance to update.
            */
            static void qvariant2qobject(const QVariantMap &variantMap,
                                         QObject *object = nullptr);

            static QString removeClassName(QString className);

        private:
            Q_DISABLE_COPY(QObjectHelper)
            class QObjectHelperPrivate;
            QObjectHelperPrivate *const d;
    };
}

#endif // QOBJECTHELPER_H
