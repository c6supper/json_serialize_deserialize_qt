#ifndef QUPLOADFILETABLEWIDGET_H_
#define QUPLOADFILETABLEWIDGET_H_

#include "qpagetablewidget.h"

namespace QRserver
{
    class QUploadFileTableWidget : public QPageTableWidget
    {
            Q_OBJECT

        public:
            QUploadFileTableWidget(QWidget *parent = 0);
            ~QUploadFileTableWidget();

            void addItems(const QVariantList &varList);
            QVariantList getCheckedItems() const;
            QStringList getColumnValueList(const int column) const;

        public:
            static const qint32 ColumnIndexDisplayName;
            static const qint32 ColumnIndexTypeName;
            static const qint32 ColumnIndexDateTime;
            static const qint32 ColumnIndexFileType;
            static const qint32 ColumnIndexPathName;
            static const qint32 ColumnIndexJobId;
            static const qint32 ColumnIndexLocation;
            static qint32 FileNameLengthLimitation;

        protected:
            virtual const QString columnToProperty(int column) const;

        signals:
            void invalidRowChecked();

        protected slots:
            virtual void onItemChanged(QStandardItem *item);

        private:
            virtual const QString getToolTip(const QModelIndex &index) const;
    };
};

#endif //QUPLOADFILETABLEWIDGET_H_
