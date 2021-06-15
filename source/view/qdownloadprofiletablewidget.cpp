#include "qdownloadprofiletablewidget.h"
#include <QHeaderView>
#include "qdownloadfilemodel.h"

using namespace QRserver;

QDownloadProfileTableWidget::QDownloadProfileTableWidget(QWidget *parent)
    : QPageTableWidget(parent, false)
{
    setObjectName("QDownloadProfileTableWidget");
}

void QDownloadProfileTableWidget::addItems(const QVariantList &varList)
{
    foreach (QVariant var, varList) {
        QVariantMap itemMap = var.toMap();
        int row = rowCount();
        insertRow(row);

        for (int i = 0; i < itemMap.size(); i++) {
            setData(index(row, i), QVariant(QBrush(getColumnForegroundColor(i,
                                                   itemMap[columnToProperty(i)].toString()))), Qt::ForegroundRole);
            setData(index(row, i), itemMap[columnToProperty(i)], Qt::DisplayRole);
        }
    }

    adjustPageCount();
}

const QString QDownloadProfileTableWidget::columnToProperty(int column) const
{
    QString strProperty;

    switch (column) {
    case 0:
        strProperty = QFileModel::PropertyMeasureType;
        break;

    case 1:
        strProperty = QDownloadFileModel::FileStatus;
        break;

    default:
        strProperty = "";
        break;
    }

    return strProperty;
}

const QColor QDownloadProfileTableWidget::getColumnForegroundColor(
    int column, QString status) const
{
    QColor color;

    switch (column) {
    case 0:
        color = Qt::black;
        break;

    case 1:
        if (QDownloadFileModel::FileStatusNew == status)
            color = Qt::red;
        else
            color = Qt::green;

        break;

    default:
        color = Qt::black;
        break;
    }

    return color;
}

QDownloadProfileTableWidget::~QDownloadProfileTableWidget()
{
}

