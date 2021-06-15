/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */


#ifndef QABSTRACTMODEL_H
#define QABSTRACTMODEL_H

#include "qprofile_export.h"
#include "qprofile_debug.h"
#include "qprofile_global.h"
#include "qabstractattribute.h"
#include <QTimer>
#include <QList>
#include <QStringList>
#include "qabstractjsonelement.h"

QT_BEGIN_NAMESPACE
class QVariant;
class QByteArray;
QT_END_NAMESPACE

namespace QProfile
{
    class QAbstractModel : public QAbstractJsonElement
    {
            Q_OBJECT

            //            Q_PROPERTY(quint32 lastError READ error WRITE setError)
            Q_PROPERTY(QVariant command READ command WRITE appendCommand)
            Q_PROPERTY(QVariant trigger READ trigger WRITE appendTrigger)

        private:
            QList<QAbstractAttribute *> 	m_attributeList;
            QTimer							m_changeNotifyTimer;

        public:
            static const qint32 MaxNumberErrorStored;
            static const qint32 PropertyChangedNotifyInterval;

        public:
            QAbstractModel(QObject *parent = nullptr);
            QAbstractModel(QObject *parent, const QByteArray &content);
            QAbstractModel(const QAbstractModel &model);
            ~QAbstractModel();
            virtual void pureVirtualMock() {};
            Q_INVOKABLE void registerAttribute(QAbstractAttribute *attribute);

            Q_INVOKABLE void setEnabled(bool enabled)
            {
                m_enabled = enabled;
            }
            Q_INVOKABLE bool isEnabled()
            {
                return m_enabled;
            }
            bool blockNotify(bool notify);
            bool isNotify();
            void asynPropertyChangeNotify();
            void emitModelChangedByOthers();

            /*void setError(const quint32 &error)
            {
                if (m_errorList.count() > MaxNumberErrorStored) {
                    m_errorList.removeFirst();
                }

                if (m_errorList.last() != error) {
                    if (!m_errorList.isEmpty())
                        m_errorList.append(error);
                }
            }
            const quint32 &error()
            {
                return m_errorList.last();
            }*/
            const QVariant command();
            const QVariant takeCommand();
            void appendCommand(const QVariant command);
            virtual bool isValid();

            //Trigger
            const QVariant trigger();
            const QVariant takeTrigger();
            void appendTrigger(const QVariant trigger);

        protected:
            virtual const QStringList ignoredProperties() const;
        public slots:
            virtual void initAttributes() = 0;

        protected slots:
            void propertyChangeNotify();
            void propertyChangeNotifyLater();

        signals:
            void propertyChanged();
            void commandPropertyChanged();
            void triggerPropertyChanged();
            void modelChangedByOthers();

        private:
            bool											m_enabled;
            bool											m_isNotify;
            QVariantList									m_commandList;
            QMutex											*m_commandMutex;
            QVariantList									m_triggerList;
            QMutex											*m_triggerMutex;

        protected:
            bool m_isValid;
    };
}

Q_DECLARE_METATYPE(QProfile::QAbstractModel *)

#endif // QABSTRACTMODEL_H
