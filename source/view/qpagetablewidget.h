#ifndef QPAGETABLEVIEW_H_
#define QPAGETABLEVIEW_H_

#include <QWidget>
#include <QVBoxLayout>
#include <QStandardItemModel>
#include "qbasetableview.h"
#include "qsortfilterpageproxymodel.h"
#include "qpagebar.h"

namespace QRserver
{
    class QPageTableWidget : public QWidget
    {
            Q_OBJECT

        public:
            QPageTableWidget(QWidget *parent = 0, bool dynamicSort = true);
            ~QPageTableWidget();

            void setSortingEnabled(bool enable);
            void setShowGrid(bool show);
            void setHorizontalHeaderLabels(const QStringList &labels);
            void setFilterKeyColumn(int column);
            void setColumnHidden(int column, bool hide);
            void setSourceModel(QAbstractItemModel *sourceModel);
            void setMaxVisibleRows(int maxRows);
            void addItem(const QVariant &var);
            void addItems(const QVariantList &varList);
            void clear();
            QHeaderView *horizontalHeader() const;
            void setHorizontalHeader(QHeaderView *header);
            void setColumnWidth(int column, int width);
            void setSize(int width, int height);
            QModelIndex indexFromItem(const QStandardItem *item) const;
            QList<QStandardItem *> findItems(const QString &text,
                                             Qt::MatchFlags flags = Qt::MatchExactly, int column = 0) const;
            int rowCount(const QModelIndex &parent = QModelIndex()) const;
            int columnCount(const QModelIndex &parent = QModelIndex()) const;
            void setColumnCount(int columns);
            QModelIndex index(int row, int column,
                              const QModelIndex &parent = QModelIndex()) const;
            bool insertRow(int row, const QModelIndex &parent = QModelIndex());
            void setItem(int row, int column, QStandardItem *item);
            bool setData(const QModelIndex &index, const QVariant &value,
                         int role = Qt::EditRole);
            void adjustPageCount();
            void setAlternatingRowColors(bool enable);
            QStandardItemModel *model();
            void refresh();
            void setFont(const QFont &);
            void adjustHeight();

        private:
            void changeAllItemState(int state);
            virtual const QString getToolTip(const QModelIndex &index) const;

        public slots:
            void onFilterPatternChanged(const QString &);
            void onColumnFilterPatternChanged(const int column, const QString &str);

        private slots:
            void changeCurrentPage(int index);
            void showToolTip(const QModelIndex &index);

        protected:
            virtual const QString columnToProperty(int column) const = 0;

        protected slots:
            virtual void onItemChanged(QStandardItem *item);

        private:
            QStandardItemModel *m_standardItemModel;
            QSortFilterPageProxyModel *m_proxyModel;
            QBaseTableView *m_tableView;
            QStringList m_headerLabels;
            int m_maxVisibleRows;
            QPageBar *m_pageBar;
            QMap<int, bool> m_columnHiddenMap;
    };
};

#endif //QPAGETABLEVIEW_H_
