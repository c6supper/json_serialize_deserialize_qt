#ifndef QSOFTWARETABLEWIDGET_H_
#define QSOFTWARETABLEWIDGET_H_

#include "qpagetablewidget.h"

namespace QRserver
{
    class QSoftwareTableWidget : public QPageTableWidget
    {
            Q_OBJECT

        public:
            QSoftwareTableWidget(QWidget *parent = 0);
            ~QSoftwareTableWidget();

            enum SelectionMode {SingleSelection, MultiSelection};
            void setCheckboxSelectionMode(enum SelectionMode mode);
            void addItems(const QVariantList &varList);
            QVariantList getCheckedItems() const;

        protected:
            virtual const QString columnToProperty(int column) const;

        protected slots:
            virtual void onItemChanged(QStandardItem *item);

        private:
            enum SelectionMode m_selectionMode;

        private:
            virtual const QString getToolTip(const QModelIndex &index) const;
    };
};

#endif /* QSOFTWARETABLEWIDGET_H_ */
