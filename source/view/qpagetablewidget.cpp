#include "qpagetablewidget.h"
#include <QHeaderView>
#include "qprofile_global.h"
#include "qprofile_debug.h"

using namespace QRserver;

QPageTableWidget::QPageTableWidget(QWidget *parent, bool dynamicSort)
    : QWidget(parent), m_maxVisibleRows(8)
{
    m_standardItemModel = new QStandardItemModel(parent);
    connect(m_standardItemModel, SIGNAL(itemChanged(QStandardItem *)), this,
            SLOT(onItemChanged(QStandardItem *)));
    m_proxyModel = new QSortFilterPageProxyModel(this);
    m_proxyModel->setDynamicSortFilter(dynamicSort);
    m_proxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);
    m_proxyModel->setSourceModel(m_standardItemModel);
    m_proxyModel->setMaxVisibleRows(m_maxVisibleRows);
    m_tableView = new QBaseTableView(this);
    m_tableView->setModel(m_proxyModel);
    m_pageBar = new QPageBar(this);
    connect(m_pageBar, SIGNAL(pageChanged(int)), SLOT(changeCurrentPage(int)));
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addWidget(m_tableView);
    layout->addWidget(m_pageBar);
    layout->addStretch();
    connect(m_tableView, SIGNAL(clicked(const QModelIndex &)),
            SLOT(showToolTip(const QModelIndex &)));
}

#include <QToolTip>

const QString QPageTableWidget::getToolTip(const QModelIndex &index) const
{
    return "";
}

void QPageTableWidget::showToolTip(const QModelIndex &index)
{
    QString tooltip = getToolTip(index);

    if (!tooltip.isEmpty()) {
        QPoint pos(m_tableView->mapToGlobal(QPoint(0, 0)));
        pos.setY(pos.y() + index.row() * m_tableView->rowHeight(index.row()));
        QToolTip::showText(pos, tooltip);
    }
}

QPageTableWidget::~QPageTableWidget()
{
}

void QPageTableWidget::setFont(const QFont &font)
{
    m_tableView->setFont(font);
}

void QPageTableWidget::setSortingEnabled(bool enable)
{
    m_tableView->setSortingEnabled(enable);
}

void QPageTableWidget::setMaxVisibleRows(int maxRows)
{
    m_maxVisibleRows = maxRows;
    m_proxyModel->setMaxVisibleRows(maxRows);
}

void QPageTableWidget::onItemChanged(QStandardItem *item)
{
}

void QPageTableWidget::setShowGrid(bool show)
{
    m_tableView->setShowGrid(show);
}

QModelIndex QPageTableWidget::indexFromItem(const QStandardItem *item) const
{
    return m_standardItemModel->indexFromItem(item);
}

QList<QStandardItem *> QPageTableWidget::findItems(const QString &text,
        Qt::MatchFlags flags, int column) const
{
    return m_standardItemModel->findItems(text, flags, column);
}

int QPageTableWidget::rowCount(const QModelIndex &parent) const
{
    return m_standardItemModel->rowCount(parent);
}

int QPageTableWidget::columnCount(const QModelIndex &parent) const
{
    return m_standardItemModel->columnCount(parent);
}

void QPageTableWidget::setColumnCount(int columns)
{
    m_standardItemModel->setColumnCount(columns);
}

QModelIndex QPageTableWidget::index(int row, int column,
                                    const QModelIndex &parent) const
{
    return m_standardItemModel->index(row, column, parent);
}

bool QPageTableWidget::insertRow(int row, const QModelIndex &parent)
{
    return m_standardItemModel->insertRow(row, parent);
}

void QPageTableWidget::setItem(int row, int column, QStandardItem *item)
{
    m_standardItemModel->setItem(row, column, item);
}

bool QPageTableWidget::setData(const QModelIndex &index, const QVariant &value,
                               int role)
{
    return m_standardItemModel->setData(index, value, role);
}

void QPageTableWidget::setHorizontalHeaderLabels(const QStringList &labels)
{
    m_headerLabels = labels;
    m_standardItemModel->setHorizontalHeaderLabels(labels);
}

void	QPageTableWidget::setFilterKeyColumn(int column)
{
    m_proxyModel->setFilterKeyColumn(column);
}

void	QPageTableWidget::setColumnHidden(int column, bool hide)
{
    if (column > 0 && column < m_headerLabels.count()) {
        if (!m_columnHiddenMap.contains(column))
            m_columnHiddenMap.insert(column, hide);

        m_tableView->setColumnHidden(column, hide);
    }
}

void QPageTableWidget::onFilterPatternChanged(const QString &str)
{
    QRegExp regExp(str, Qt::CaseInsensitive, QRegExp::RegExp);
    m_proxyModel->setFilterRegExp(regExp);
    adjustPageCount();
}

void QPageTableWidget::onColumnFilterPatternChanged(const int column,
        const QString &str)
{
    QRegExp regExp(str, Qt::CaseInsensitive, QRegExp::RegExp);
    m_proxyModel->setColumnFilterRegExp(column, regExp);
    adjustPageCount();
}

void QPageTableWidget::addItem(const QVariant &var)
{
    QVariantMap itemMap = var.toMap();
    int row = m_standardItemModel->rowCount();
    m_standardItemModel->insertRow(row);

    for (int i = 0; i < itemMap.size(); i++) {
        m_standardItemModel->setData(m_standardItemModel->index(row, i),
                                     itemMap[columnToProperty(i)]);
    }

    adjustPageCount();
}

void QPageTableWidget::clear()
{
    m_standardItemModel->removeRows(0, rowCount());
    m_pageBar->clear();
    refresh();
}

void QPageTableWidget::changeCurrentPage(int index)
{
    m_proxyModel->setCurrentPage(index);
}

void QPageTableWidget::adjustPageCount()
{
    m_pageBar->clear();

    for (int i = 0; i < m_proxyModel->actualRowCount(); i++) {
        if ((i + 1) % m_maxVisibleRows == 0) {
            m_pageBar->addPage();
        }
    }

    if (m_proxyModel->actualRowCount() % m_maxVisibleRows != 0) {
        m_pageBar->addPage();
    }

    m_pageBar->setVisible(m_proxyModel->actualRowCount() > m_maxVisibleRows);
    m_proxyModel->setCurrentPage(0);
}

void QPageTableWidget::adjustHeight()
{
    int tableHeight = qMin(rowCount(),
                           m_maxVisibleRows) * m_tableView->rowHeight(0);
    setFixedHeight(tableHeight + m_pageBar->height());
    m_tableView->setFixedHeight(tableHeight);
}

void QPageTableWidget::addItems(const QVariantList &varList)
{
    foreach (QVariant var, varList) {
        QVariantMap itemMap = var.toMap();
        int row = m_standardItemModel->rowCount();
        m_standardItemModel->insertRow(row);

        for (int i = 0; i < itemMap.size(); i++) {
            m_standardItemModel->setData(m_standardItemModel->index(row, i),
                                         itemMap[columnToProperty(i)]);
        }
    }

    adjustPageCount();
}

QHeaderView *QPageTableWidget::horizontalHeader() const
{
    return m_tableView->horizontalHeader();
}

void QPageTableWidget::setHorizontalHeader(QHeaderView *header)
{
    m_tableView->setHorizontalHeader(header);
}

void QPageTableWidget::setColumnWidth(int column, int width)
{
    m_tableView->setColumnWidth(column, width);
}

void QPageTableWidget::setSize(int width, int height)
{
    QWidget::setFixedSize(width, height);
    m_pageBar->setFixedSize(width, 30);
    m_tableView->setFixedSize(width, qMax(height - m_pageBar->height(), 0));
    adjustPageCount();
}

void QPageTableWidget::setSourceModel(QAbstractItemModel *
                                      sourceModel)
{
    m_standardItemModel = qobject_cast<QStandardItemModel *>(sourceModel);
    m_standardItemModel->setHorizontalHeaderLabels(m_headerLabels);
    m_proxyModel->setSourceModel(sourceModel);
}

void QPageTableWidget::setAlternatingRowColors(bool enable)
{
    m_tableView->setAlternatingRowColors(enable);
}

QStandardItemModel *QPageTableWidget::model()
{
    return m_standardItemModel;
}

void QPageTableWidget::refresh()
{
    m_proxyModel->refresh();
}

