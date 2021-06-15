#include "qcheckboxheaderview.h"
#include <QDebug>

using namespace QRserver;

QCheckboxHeaderView::QCheckboxHeaderView(Qt::Orientation orientation,
        QWidget *parent)
    : QHeaderView(orientation, parent)
{
#if 0 // TODO:
    m_pCheckBox = new QCheckBox(this);
    m_pCheckBox->setFixedSize(25, 25);
    m_pCheckBox->setFocusPolicy(Qt::NoFocus);
    connect(m_pCheckBox, SIGNAL(stateChanged(int)), this,
            SIGNAL(stateChanged(int)));
#endif
    setResizeMode(QHeaderView::Fixed);
    setHighlightSections(false);
    setDefaultAlignment(Qt::AlignVCenter);
    setClickable(true);
}

void QCheckboxHeaderView::paintSection(QPainter *painter, const QRect &rect,
                                       int logicalIndex) const
{
#if 0 // TODO:

    if (CheckBoxColumnIndex == logicalIndex) {
        int iYOffset = m_pCheckBox->height() < rect.height() ? (rect.height() -
                       m_pCheckBox->height()) / 2 : 0;
        QRect rectCheckBox(rect.x() + 3, rect.y() + iYOffset,
                           m_pCheckBox->width(), m_pCheckBox->height());
        m_pCheckBox->setGeometry(rectCheckBox);
    }

#endif
    QHeaderView::paintSection(painter, rect, logicalIndex);
}

void QCheckboxHeaderView::mousePressEvent(QMouseEvent *event)
{
    const int index = logicalIndexAt(event->pos());

    if (CheckBoxColumnIndex == index) {
        setClickable(false);
    }

    QHeaderView::mousePressEvent(event);

    if (!isClickable()) {
        setClickable(true);
    }
}

void QCheckboxHeaderView::mouseReleaseEvent(QMouseEvent *event)
{
    const int index = logicalIndexAt(event->pos());

    if (CheckBoxColumnIndex == index) {
        setClickable(false);
    }

    QHeaderView::mouseReleaseEvent(event);

    if (!isClickable()) {
        setClickable(true);
    }
}

QCheckboxHeaderView::~QCheckboxHeaderView()
{
}

