#include "qbasetableview.h"
#include <QHeaderView>

using namespace QRserver;

QBaseTableView::QBaseTableView(QWidget *parent)
    : QTableView(parent)
{
    this->verticalHeader()->hide();
    this->setSortingEnabled(true);
    this->setFocusPolicy(Qt::NoFocus);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setSelectionMode(QAbstractItemView::SingleSelection);
    this->setAutoScroll(false);
    this->verticalHeader()->setDefaultSectionSize(29);
    this->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
    this->horizontalHeader()->setHighlightSections(false);
    this->horizontalHeader()->setDefaultAlignment(Qt::AlignVCenter);
    this->horizontalHeader()->setClickable(false);
}

QBaseTableView::~QBaseTableView()
{
}

