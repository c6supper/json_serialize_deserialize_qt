#ifndef QABSTRACTCONTAINER_WIDGET_H
#define QABSTRACTCONTAINER_WIDGET_H

#include <QPointer>
#include <QWidget>
#include "qabstractipcclient.h"
#include "qgenericmessage.h"
#include "qabstracttopic.h"
#include "qabstractmodel.h"
#include "qrserverpropertybinder.h"
#include "qbackprogress.h"
#include "qeventprogress.h"
#include "qrservernetworkchecker.h"

using namespace QProfile;

namespace QRserver
{
    class QAbstractContainerWidget : public QWidget
    {
            Q_OBJECT

        public:
            QAbstractContainerWidget(QWidget *parent = 0, bool isRetainMessage = false);
            ~QAbstractContainerWidget();

            QString serialize();
            void deSerialize(const QString &data);

            const QVariant toVariant() const;

            virtual void setTagName(const QString &tagName) = 0;
            const QString tagName();

            void setModel(QAbstractModel *model, bool needBinder = false);
            QAbstractModel *model();

            virtual bool isTopWidget();
            bool isTopWidgetVisible();

        protected:
            virtual void customEvent(QEvent *event);
            bool isCompatible(const QString feature);

        protected slots:
            virtual	void allocTopic() = 0;
            virtual	void allocModel() = 0;
            virtual const QByteArray get(const QGenericMessage &message);
            virtual void onModelChanged();
            virtual void postponeInit();
            virtual void onIpcConnected();
            virtual void onIpcDisconnected();
            virtual void onNetworkError();

        protected:
            virtual void allocBackProgress();
            QBaseProgress	*getBackProgress();
            QRserverNetworkChecker *getNetworkChecker();

        private:
            static const QStringList ignoredPropertyList;

        private:
            static const QStringList IgnoredPropertyList();

        protected:
            QPointer<QAbstractIpcClient> 	m_ipcClient;
            QPointer<QAbstractTopic> 		m_topic;
            QRserverNetworkChecker        *m_networkChecker;

        private:
            QPointer<QAbstractModel> 		m_model;
            QRserverPropertyBinder			*m_binder;
            bool							m_isRetainMessage;
            QBaseProgress					*m_backProgress;

        signals:
            void propertyChanged();
    };
};

#endif // QABSTRACTCONTAINER_WIDGET_H
