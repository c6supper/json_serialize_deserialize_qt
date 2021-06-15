#ifndef QRSERVERCONTAINER_WIDGET_H
#define QRSERVERCONTAINER_WIDGET_H

#include "qabstractcontainerwidget.h"
#include "qbackprogress.h"

#define TAB_ROW_HEIGHT (30)

namespace QRserver
{
    class QRserverContainerWidget : public QAbstractContainerWidget
    {
            Q_OBJECT

        public:
            QRserverContainerWidget(QWidget *parent = 0, bool isRetainMessage = false);
            ~QRserverContainerWidget();

        protected:
            virtual void setTagName(const QString &tagName);
            virtual qint32 issueCommand(const QVariant &commandVar,
                                        const QString &method = "", quint32 interval = 10000,
                                        quint32 checkPeriod = QBaseProgress::TimerCheckPeriod);
            virtual const QVariant issueCommand(const QVariant &commandVar,
                                                quint32 interval,
                                                const QString &method, const QVariant &exepcted = QVariant());

        private slots:
            void onStatusArrived();

        protected slots:
            virtual void allocTopic();
            virtual void postponeInit();
            virtual void onIpcDisconnected();
            virtual void onNetworkError();
    };
};

#endif // QRSERVERCONTAINER_WIDGET_H
