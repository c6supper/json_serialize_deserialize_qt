#ifndef QUPLOADPROFILEWIDGET_H_
#define QUPLOADPROFILEWIDGET_H_

#include "quploadfilewidget.h"
#include "qabstractstatus.h"

namespace QRserver
{
    class QUploadProfileWidget : public QUploadFileWidget
    {
            Q_OBJECT

        public:
            QUploadProfileWidget(QWidget *parent = 0,
                                 const QString &tagName = "quploadprofile");
            ~QUploadProfileWidget();

        public slots:
            void onUpload();

        protected:
            virtual void allocModel();
            void uploadProfileInfoList();

        protected slots:
            void onGetProfileInfoList();
    };
};

#endif // QUPLOADPROFILEWIDGET_H_
