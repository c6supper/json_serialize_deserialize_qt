#include "qsoftwaretablewidget.h"
#include "qsoftwaremodel.h"

using namespace QRserver;

QSoftwareTableWidget::QSoftwareTableWidget(QWidget *parent)
    : QPageTableWidget(parent), m_selectionMode(SingleSelection)
{
    setObjectName("QSoftwareTableWidget");
}

void QSoftwareTableWidget::setCheckboxSelectionMode(
    enum SelectionMode mode)
{
    m_selectionMode = mode;
}

const QString QSoftwareTableWidget::getToolTip(const QModelIndex &index) const
{
    if (index.isValid()) {
        return index.data(Qt::DisplayRole).toString();
    }

    return "";
}

void QSoftwareTableWidget::onItemChanged(QStandardItem *item)
{
    if (m_selectionMode == MultiSelection)
        return;

    QModelIndex index = indexFromItem(item);
    QList<QStandardItem *> itemList = findItems("*",
                                      Qt::MatchWildcard | Qt::MatchRecursive);

    for (int row = 0; row < itemList.length(); row++) {
        QModelIndex idx = indexFromItem(itemList.at(row));

        if (index != idx) {
            model()->blockSignals(true);
            itemList.at(row)->setCheckState(Qt::Unchecked);
            model()->blockSignals(false);
        }
    }

    refresh();
}

QVariantList QSoftwareTableWidget::getCheckedItems() const
{
    QVariantList varList;
    QList<QStandardItem *> itemList = findItems("*",
                                      Qt::MatchWildcard | Qt::MatchRecursive);

    for (int row = 0; row < itemList.length(); row++) {
        if (itemList.at(row)->checkState() == Qt::Checked) {
            QVariantMap itemMap;

            for (int col = 1; col < columnCount(); col++) {
                itemMap.insert(index(row, col).data(Qt::StatusTipRole).toString(), index(row,
                               col).data(Qt::DisplayRole));
            }

            varList.append(itemMap);
        }
    }

    return varList;
}

void QSoftwareTableWidget::addItems(const QVariantList &varList)
{
    foreach (QVariant var, varList) {
        QVariantMap itemMap = var.toMap();
        int row = rowCount();
        insertRow(row);
        QStandardItem *itemCheckBox = new QStandardItem("");
        itemCheckBox->setCheckState(Qt::Unchecked);
        itemCheckBox->setCheckable(true);
        setItem(row, 0, itemCheckBox);
        QVariantMap hiddenMap = itemMap;
        int column = 0;

        for (int i = 0; i < itemMap.size(); i++) {
            if (!columnToProperty(i + 1).isEmpty()) {
                setData(index(row, i + 1), itemMap[columnToProperty(i + 1)], Qt::DisplayRole);
                setData(index(row, i + 1), columnToProperty(i + 1), Qt::StatusTipRole);
                hiddenMap.remove(columnToProperty(i + 1));
                column = i + 1;
            }
        }

        QVariantMap::iterator i;

        for (i = hiddenMap.begin(); i != hiddenMap.end(); ++i) {
            setData(index(row, ++column), i.key(), Qt::StatusTipRole);
            setData(index(row, column), i.value(), Qt::DisplayRole);
        }
    }

    adjustPageCount();
}

const QString QSoftwareTableWidget::columnToProperty(int column) const
{
    QString strProperty;

    switch (column) {
    case 1:
        strProperty = QDeviceInfo::PropertyDeviceName;
        break;

    case 2:
        strProperty = QDeviceInfo::PropertyVersion;
        break;

    case 3:
        strProperty = QDeviceInfo::PropertyRemoteVersion;
        break;

    default:
        strProperty = "";
        break;
    }

    return strProperty;
}

QSoftwareTableWidget::~QSoftwareTableWidget()
{
}

