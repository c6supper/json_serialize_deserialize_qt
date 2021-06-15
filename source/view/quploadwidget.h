#ifndef QUPLOADWIDGET_H_
#define QUPLOADWIDGET_H_

#include "qrservercontainerwidget.h"
#include <QTabWidget>
#include "quploadprofilewidget.h"
#include "quploadresultwidget.h"

namespace QRserver
{
    class QUploadWidget : public QRserverContainerWidget
    {
            Q_OBJECT

        public:
            enum RserverTabUploadIndex {
                RserverTabUploadProfile = 0,
                RserverTabUploadResult
            };

        public:
            QUploadWidget(QWidget *parent = 0, const QString &tagName = "qupload");
            ~QUploadWidget();

            void translate(const QFont &font);
            const qint32 currentTabIndex();

        signals:
            void currentTabChanged(int);
            void onUploadBtnClicked();
            void onJobBtnClicked();
            void onUploadAndDelBtnClicked();
            void checkDeviceSucceed();
            void getProfileInfoList();

        public slots:
            void onSetTab(bool lock);

        private slots:
            void onRserverModelChanged();

        private:
            QTabWidget 	*m_ptab;
            QUploadProfileWidget *m_uploadProfile;
            QUploadResultWidget *m_uploadResult;

        protected:
            virtual void allocModel();
    };
};

#endif // QUPLOADWIDGET_H_
