#ifndef QDOWNLOADWIDGET_H_
#define QDOWNLOADWIDGET_H_

#include "qrservercontainerwidget.h"
#include "qdownloadprofiletablewidget.h"
#include "qdownloadfilemodel.h"
#include "qabstractstatus.h"

namespace QRserver
{
    class QDownloadWidget : public QRserverContainerWidget
    {
            Q_OBJECT

            Q_PROPERTY(QVariantList fileModelList READ fileModelList WRITE setFileModelList)

        public:
            QDownloadWidget(QWidget *parent = 0, const QString &tagName = "qdownload");
            ~QDownloadWidget();

            const QVariantList downloadFileModelList() const;
            const QVariantList fileModelList() const;
            void setFileModelList(const QVariantList &fileModelList);
            void translate(const QFont &font);

        protected:
            virtual void allocBackProgress();

        private:
            QDownloadProfileTableWidget *m_profileTableWidget;
            QDownloadFileModel		 *m_downloadFileModel;
            QLabel *m_pLabLockDown;

        public slots:
            void onGetChangeList();
            void onDownloadAll();
            void onLockDown(bool bLock);
            void onShowLockedTestType();
            void onCheckDeviceSucceed();

        protected:
            virtual void allocModel();
            virtual void resizeEvent(QResizeEvent *event);

    };
};

#endif /* QDOWNLOADWIDGET_H_ */
