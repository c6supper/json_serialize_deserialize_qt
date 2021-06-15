#ifndef QRSERVERUI_H_
#define QRSERVERUI_H_

#include <QPointer>
#include "qrserverwidget.h"

QT_BEGIN_NAMESPACE
namespace QProfile
{
    class QAbstractIpcClient;
}
QT_END_NAMESPACE

namespace QRserver
{
    class QRserverUi : public QWidget
    {
            Q_OBJECT

        public:
            QRserverUi(QWidget *parent = 0);
            ~QRserverUi();

            void translate(const QFont &font);

        protected:
            virtual void customEvent(QEvent *event);

        private:
            void loadStyleSheet();

        private:
            QHBoxLayout *m_pMainLayout;
            QPointer<QAbstractIpcClient> m_ipcClient;
            QRserverWidget *m_pRServer;

        signals:
            void networkError();

        private slots:
            void onIpcConnected();
            void onIpcDisconnected();
    };
}

#endif // QRSERVERUI_H_
