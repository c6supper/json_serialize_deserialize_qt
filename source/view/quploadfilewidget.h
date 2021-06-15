#ifndef QUPLOADFILEWIDGET_H_
#define QUPLOADFILEWIDGET_H_

#include "qrservercontainerwidget.h"
#include "qabstractstatus.h"
#include "quploadfiletablewidget.h"
#include "qbaselineedit.h"
#include <QPushButton>

namespace QRserver
{
    class QUploadFileWidget : public QRserverContainerWidget
    {
            Q_OBJECT

            Q_PROPERTY(QVariantList fileModelList READ fileModelList WRITE setFileModelList)

        public:
            QUploadFileWidget(QWidget *parent = 0,
                              const QString &tagName = "");
            ~QUploadFileWidget();
            void translate(const QFont &font);

            const QVariantList fileModelList() const;
            void setFileModelList(const QVariantList &fileModelList);
            void setColumnFilterPattern(const int column, const QString &pattern);
            QStringList getColumnValueList(const int column) const;

        protected slots:
            void onGetFileList();

        protected:
            virtual void allocBackProgress();

        private slots:
            void onInvalidRowChecked();

        private:
            QStringList m_headerLabels;
            QUploadFileTableWidget *m_tableWidget;
            QWidget *m_bottomWidget;
            QBaseLineEdit *m_filterPattern;
            QPushButton *m_pBtnRefresh;

        protected:
            virtual void resizeEvent(QResizeEvent *event);
            virtual void showEvent(QShowEvent *event);
    };
};

#endif // QUPLOADFILEWIDGET_H_
