#include "quploadfiletablewidget.h"
#include "qfilemodel.h"
#include "qcheckboxheaderview.h"
#include "qrservercompatibility405.h"

using namespace QRserver;

const qint32 QUploadFileTableWidget::ColumnIndexDisplayName = 1;
const qint32 QUploadFileTableWidget::ColumnIndexTypeName = 2;
const qint32 QUploadFileTableWidget::ColumnIndexDateTime = 3;
const qint32 QUploadFileTableWidget::ColumnIndexFileType = 4;
const qint32 QUploadFileTableWidget::ColumnIndexPathName = 5;
const qint32 QUploadFileTableWidget::ColumnIndexJobId = 6;
const qint32 QUploadFileTableWidget::ColumnIndexLocation = 7;
qint32 QUploadFileTableWidget::FileNameLengthLimitation =
    QRserverCompatibility405::MaxFileName;

QUploadFileTableWidget::QUploadFileTableWidget(QWidget *parent)
    : QPageTableWidget(parent)
{
    setObjectName("QUploadFileTableWidget");
    QCheckboxHeaderView *header = new QCheckboxHeaderView();
    this->setHorizontalHeader(header);
}

QUploadFileTableWidget::~QUploadFileTableWidget()
{
}

void QUploadFileTableWidget::onItemChanged(QStandardItem *item)
{
    if (item->isCheckable()) {
        QModelIndex itemIndex = indexFromItem(item);

        if (!itemIndex.isValid())
            return;

        QModelIndex fileNameIndex = index(itemIndex.row(), ColumnIndexDisplayName);

        if (fileNameIndex.data().toString().length() > FileNameLengthLimitation) {
            model()->blockSignals(true);
            item->setCheckState(Qt::Unchecked);
            model()->blockSignals(false);
            emit invalidRowChecked();
        }
    }
}

const QString QUploadFileTableWidget::getToolTip(const QModelIndex &index) const
{
    if (index.isValid()) {
        const QAbstractItemModel *model = index.model();
        return model->data(model->index(index.row(), ColumnIndexDisplayName),
                           Qt::DisplayRole).toString();
    }

    return "";
}

void QUploadFileTableWidget::addItems(const QVariantList &varList)
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
                if (columnToProperty(i + 1) == QFileModel::PropertyDateTime) {
                    setData(index(row, i + 1), itemMap[columnToProperty(i + 1)].toDateTime(),
                            Qt::DisplayRole);
                    setData(index(row, i + 1), itemMap[columnToProperty(i + 1)].toDateTime(),
                            Qt::StatusTipRole);
                } else {
                    setData(index(row, i + 1), itemMap[columnToProperty(i + 1)], Qt::DisplayRole);
                    setData(index(row, i + 1), columnToProperty(i + 1), Qt::StatusTipRole);
                    setData(index(row, i + 1), itemMap[columnToProperty(i + 1)], Qt::ToolTipRole);
                }

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

QVariantList QUploadFileTableWidget::getCheckedItems() const
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

QStringList QUploadFileTableWidget::getColumnValueList(const int column) const
{
    QStringList list;
    QList<QStandardItem *> itemList = findItems("*",
                                      Qt::MatchWildcard | Qt::MatchRecursive);

    for (int row = 0; row < itemList.length(); row++) {
        list.append(index(row, column).data(Qt::DisplayRole).toString());
    }

    list.removeAll("");
    list.removeDuplicates();
    return list;
}

const QString QUploadFileTableWidget::columnToProperty(int column) const
{
    QString strProperty;

    switch (column) {
    case ColumnIndexDisplayName:
        strProperty = QFileModel::PropertyDisplayName;
        break;

    case ColumnIndexTypeName:
        strProperty = QFileModel::PropertyTypeName;
        break;

    case ColumnIndexDateTime:
        strProperty = QFileModel::PropertyDateTime;
        break;

    case ColumnIndexFileType:
        strProperty = QFileModel::PropertyFileType;
        break;

    case ColumnIndexPathName:
        strProperty = QFileModel::PropertyPathName;
        break;

    case ColumnIndexJobId:
        strProperty = QFileModel::PropertyJobId;
        break;

    case ColumnIndexLocation:
        strProperty = QFileModel::PropertyLocation;
        break;

    default:
        strProperty = "";
        break;
    }

    return strProperty;
}

