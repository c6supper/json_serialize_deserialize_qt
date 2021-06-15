#include <QHBoxLayout>
#include <QPainter>
#include <QStyleOption>
#include "qpagebar.h"

QPageBar::QPageBar(QWidget *parent)
    : QWidget(parent),
      m_count(0),
      m_index(-1),
      m_text("Page 0 of 0")
{
    setObjectName("QPageBar");
    QHBoxLayout *pMainLayout = new QHBoxLayout(this);
    pMainLayout->setAlignment(Qt::AlignCenter);
    pMainLayout->setMargin(0);
    pMainLayout->setSpacing(100);
    m_pBtnLeft = new QPushButton(this);
    m_pBtnLeft->setObjectName("QPageBarBtnLeft");
    pMainLayout->addWidget(m_pBtnLeft);
    m_pBtnRight = new QPushButton(this);
    m_pBtnRight->setObjectName("QPageBarBtnRight");
    pMainLayout->addWidget(m_pBtnRight);
    connect(m_pBtnLeft, SIGNAL(clicked(bool)), this, SLOT(leftClicked()));
    connect(m_pBtnRight, SIGNAL(clicked(bool)), this, SLOT(rightClicked()));
}

QPageBar::~QPageBar()
{
}

void QPageBar::setPageCount(int cnt)
{
    if ((m_count == cnt) || (cnt < 0))
        return;

    if (m_index < 0) {
        m_index = 0;
    }

    m_count = cnt;
    setText();
}

int QPageBar::count() const
{
    return m_count;
}

int QPageBar::currentIndex() const
{
    return m_index;
}

void QPageBar::update()
{
    QWidget::update();
    m_pBtnLeft->update();
    m_pBtnRight->update();
}

void QPageBar::clear()
{
    m_index = -1;
    m_count = 0;
    setText();
}

void QPageBar::addPage()
{
    if (m_index < 0) {
        m_index = 0;
    }

    m_count++;
    setText();
}

void QPageBar::removePage(int index)
{
    if (m_count <= 0 || index < 0) {
        return;
    }

    if ((m_index > index) || (m_index == index && index == (m_count - 1))) {
        m_index--;
    }

    m_count--;
    setText();
}

void QPageBar::setCurrentIndex(int index)
{
    if (0 == m_count) {
        return;
    }

    m_index = index;

    if (m_index < 0) {
        m_index = m_count - 1;
    }

    if (m_index >= m_count) {
        m_index = 0;
    }

    setText();
}

void QPageBar::leftClicked()
{
    if (0 == m_count || m_index < 0) {
        return;
    }

    m_index--;

    if (m_index < 0) {
        m_index = m_count - 1;
    }

    setText();
    emit pageChanged(m_index);
}

void QPageBar::rightClicked()
{
    if (0 == m_count || m_index < 0) {
        return;
    }

    m_index++;

    if (m_index >= m_count) {
        m_index = 0;
    }

    setText();
    emit pageChanged(m_index);
}

void QPageBar::paintEvent(QPaintEvent *event)
{
    // subclass from QWidget need to provide a paintEvent for custom QWidget as below
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    QWidget::paintEvent(event);
    QPainter painter(this);
    painter.drawText(rect(), Qt::AlignCenter, m_text);
}

void QPageBar::setText()
{
    m_text = QString("Page %1 of %2").arg(m_index + 1).arg(m_count);
    update();
}

