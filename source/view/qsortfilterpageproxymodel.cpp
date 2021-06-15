#include <QSize>
#include <QBrush>
#include <QDateTime>
#include "qsortfilterpageproxymodel.h"

QSortFilterPageProxyModel::QSortFilterPageProxyModel(
    QObject *parent)
    : QSortFilterProxyModel(parent), m_maxVisibleRows(0), m_currentPage(0)
{
}

void QSortFilterPageProxyModel::sort(int column, Qt::SortOrder order)
{
    if (column == CheckBoxColumnIndex) {
        return;
    }

    QSortFilterProxyModel::sort(column, order);
}

QSortFilterPageProxyModel::~QSortFilterPageProxyModel()
{
}

void QSortFilterPageProxyModel::setMaxVisibleRows(int maxRows)
{
    m_maxVisibleRows = maxRows;
}

void QSortFilterPageProxyModel::setCurrentPage(int index)
{
    m_currentPage = index;
    refresh();
}

void QSortFilterPageProxyModel::refresh()
{
    emit layoutChanged();
}

int QSortFilterPageProxyModel::actualRowCount() const
{
    /* total rows */
    return QSortFilterProxyModel::rowCount();
}

int QSortFilterPageProxyModel::rowCount(const QModelIndex &parent) const
{
    /* current page rows */
    /* overwirte for Page, total rows use actualRowCount(). */
    int restRow = QSortFilterProxyModel::rowCount() - m_maxVisibleRows *
                  m_currentPage;
    return ((restRow > m_maxVisibleRows)
            && (m_maxVisibleRows != 0)) ? m_maxVisibleRows : restRow;
}

void QSortFilterPageProxyModel::setColumnFilterRegExp(const int column,
        const QRegExp &regExp)
{
    m_columnFilterRegExpMap.insert(column, regExp);
    invalidateFilter();
}

bool QSortFilterPageProxyModel::filterAcceptsRow(int sourceRow,
        const QModelIndex &sourceParent) const
{
    bool bAccept = true;
    int columnCount = QSortFilterProxyModel::columnCount();

    for (int column = 0; column < columnCount; column++) {
        if (m_columnFilterRegExpMap.contains(column)) {
            QModelIndex index = sourceModel()->index(sourceRow, column, sourceParent);
            bAccept = bAccept
                      && sourceModel()->data(index).toString().contains(m_columnFilterRegExpMap.value(
                                  column));

            if (!bAccept) {
                break;
            }
        }
    }

    return (bAccept
            && QSortFilterProxyModel::filterAcceptsRow(sourceRow, sourceParent));
}

bool QSortFilterPageProxyModel::lessThan(const QModelIndex &left,
        const QModelIndex &right) const
{
    bool bLessThan;
    QVariant leftData = sourceModel()->data(left);
    QVariant rightData = sourceModel()->data(right);

    if (leftData.type() == QVariant::DateTime) {
        bLessThan = leftData.toDateTime() < rightData.toDateTime();
    } else if (leftData.type() == QVariant::Int
               || leftData.type() == QVariant::UInt
               || leftData.type() == QVariant::LongLong
               || leftData.type() == QVariant::ULongLong) {
        bLessThan = leftData.toLongLong() < rightData.toLongLong();
    } else {
        bLessThan = QSortFilterProxyModel::lessThan(left, right);
    }

#ifdef BUILD_X86
    return !bLessThan;
#else
    return bLessThan;
#endif
}

QVariant QSortFilterPageProxyModel::data(const QModelIndex &index,
        int role) const
{
    if (!index.isValid())
        return false;

    int row = index.row() + m_maxVisibleRows * m_currentPage;
    QModelIndex localIndex = createIndex(row, index.column(),
                                         index.internalPointer());

    if (!localIndex.isValid())
        return false;

    switch (role) {
    case Qt::BackgroundColorRole:
        if (localIndex.row() % 2) {
            return QBrush(QColor(231, 233, 235));
        } else {
            return QBrush(QColor(204, 207, 213));
        }

        break;

    case Qt::CheckStateRole:
        break;

    case Qt::DisplayRole:
        QVariant localData = QSortFilterProxyModel::data(localIndex, role);

        if (localData.type() == QVariant::DateTime)
            return localData.toDateTime().toString("yyyy-MM-dd hh:mm:ss");

        break;
    }

    return QSortFilterProxyModel::data(localIndex, role);
}

bool QSortFilterPageProxyModel::setData(const QModelIndex &index,
                                        const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    int row = index.row() + m_maxVisibleRows * m_currentPage;
    QModelIndex localIndex = createIndex(row, index.column(),
                                         index.internalPointer());

    if (!localIndex.isValid())
        return false;

    return QSortFilterProxyModel::setData(localIndex, value, role);
}
