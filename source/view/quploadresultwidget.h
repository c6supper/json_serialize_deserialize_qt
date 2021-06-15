#ifndef QUPLOADRESULTWIDGET_H_
#define QUPLOADRESULTWIDGET_H_

#include "quploadfilewidget.h"
#include "qabstractstatus.h"

namespace QRserver
{
    class QUploadResultWidget : public QUploadFileWidget
    {
            Q_OBJECT

        public:
            QUploadResultWidget(QWidget *parent = 0,
                                const QString &tagName = "quploadresult");
            ~QUploadResultWidget();

        public slots:
            void onUpload();
            void onJob();
            void onUploadAndDel();

        protected:
            virtual void allocModel();

        private:
            void upload(const QVariant &commandVar,
                        const QVariantList &uploadList);
    };
};

#endif // QUPLOADRESULTWIDGET_H_
