#ifndef QSORTFILTERPAGEPROXYMODEL_H
#define QSORTFILTERPAGEPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QMap>
#include <QRegExp>

class QSortFilterPageProxyModel : public QSortFilterProxyModel
{
        Q_OBJECT

    public:
        QSortFilterPageProxyModel(QObject *parent = 0);
        ~QSortFilterPageProxyModel();

        void setMaxVisibleRows(int maxRows);
        void setCurrentPage(int index);
        void refresh();
        int actualRowCount() const;
        void setColumnFilterRegExp(const int column, const QRegExp &regExp);
        virtual QVariant data(const QModelIndex &index,
                              int role = Qt::DisplayRole) const;
        virtual bool setData(const QModelIndex &index, const QVariant &value,
                             int role = Qt::EditRole);
        virtual void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);

    public:
        static const qint32 CheckBoxColumnIndex = 0;

    protected:
        bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;
        bool lessThan(const QModelIndex &left, const QModelIndex &right) const;

    private:
        int rowCount(const QModelIndex &parent = QModelIndex()) const ;

    private:
        int m_maxVisibleRows;
        int m_currentPage;
        QMap<int, QRegExp> m_columnFilterRegExpMap;
};

#endif //QSORTFILTERPAGEPROXYMODEL_H
