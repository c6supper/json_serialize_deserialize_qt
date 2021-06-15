#ifndef QSOFTWAREWIDGET_H_
#define QSOFTWAREWIDGET_H_

#include "qrservercontainerwidget.h"
#include "qsoftwaretablewidget.h"
#include <QVBoxLayout>
#include "qsoftwaremodel.h"

namespace QRserver
{
    class QSoftwareWidget : public QRserverContainerWidget
    {
            Q_OBJECT

            Q_PROPERTY(QVariantList deviceList READ deviceList WRITE setDeviceList)

        public:
            QSoftwareWidget(QWidget *parent = 0, const QString &tagName = "qsoftware");
            ~QSoftwareWidget();

            const QVariantList deviceList() const;
            void setDeviceList(const QVariantList &deviceList);

            Q_INVOKABLE bool checkUpgrade();
            bool isTestAppRunning();
            void translate(const QFont &font);

        private:
            QSoftwareTableWidget 		*m_pSoftwareTableWidget;
            QSoftwareModel				*m_softwareModel;

        public slots:
            void onUpgrade();

        protected slots:
            virtual const QByteArray get(const QGenericMessage &message);

        protected:
            virtual void allocModel();
            virtual void resizeEvent(QResizeEvent *event);
            virtual void allocBackProgress();
    };
};

#endif /* QSOFTWAREWIDGET_H_ */
