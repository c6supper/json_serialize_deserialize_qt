#include "qclockprogress.h"
#include <QPainter>

static int COUNT = 0;
const static int TIME_OUT = 100;

/******************************
 * Function: QClockProgress(QWidget *parent = 0)
 * Description: Construct clock progress widget.
 * Input: parent	QWidget *	parent widget
 * Output: parent	QWidget *	parent widget
 * Return: void
 ******************************/
QClockProgress::QClockProgress(QWidget *parent)
    : QWidget(parent),
      bIsTextVisible(false),
      m_format(UserType),
      m_maxValue(0),
      m_minValue(0),
      m_curValue(0)
{
    m_pTimer = new QTimer(this);
    connect(m_pTimer, SIGNAL(timeout()), SLOT(roll()));
    setFixedSize(30, 30);
    setVisible(false);
}

/******************************
 * Function: ~QClockProgress()
 * Description: Destroy clock progress widget.
 * Input: void
 * Output: void
 * Return: void
 ******************************/
QClockProgress::~QClockProgress()
{
}

/******************************
 * Function: start()
 * Description: Start clock progress,
 * 				you must start it, otherwise is invisible.
 * Input: void
 * Output: void
 * Return: void
 ******************************/
void QClockProgress::start()
{
    m_pTimer->start(TIME_OUT);
    setVisible(true);
}

/******************************
 * Function: stop()
 * Description: Stop clock progress and it's invisible.
 * Input: void
 * Output: void
 * Return: void
 ******************************/
void QClockProgress::stop()
{
    m_pTimer->stop();
    setVisible(false);
}

/******************************
 * Function: setFormat(Format format)
 * Description: Set clock progress text format.
 * Input: format		Format		text format
 * Output: void
 * Return: void
 ******************************/
void QClockProgress::setFormat(Format format)
{
    m_format = format;

    switch (m_format) {
    case Percentage:
        bIsTextVisible = true;
        break;

    case Fraction:
        bIsTextVisible = true;
        break;

    case UserType:
    default:
        bIsTextVisible = false;
        break;
    }

    update();
}

/******************************
 * Function: setRange(int min, int max)
 * Description: Set range.
 * Input: min	int		min value
 * 		max		int		max value
 * Output: void
 * Return: void
 ******************************/
void QClockProgress::setRange(int min, int max)
{
    if (min > max) {
        m_minValue = max;
        m_maxValue = min;
    } else {
        m_minValue = min;
        m_maxValue = max;
    }

    m_minValue = m_minValue > 0 ? m_minValue : 0;
    m_maxValue = m_maxValue > m_minValue ? m_maxValue : m_minValue;
    update();
}

/******************************
 * Function: setValue(int value)
 * Description: Set range.
 * Input: value		int		current value
 * Output: void
 * Return: void
 ******************************/
void QClockProgress::setValue(int value)
{
    m_curValue = value;

    if (m_curValue < m_minValue) {
        m_curValue = m_minValue;
    } else if (m_curValue > m_maxValue) {
        m_curValue = m_maxValue;
    }

    update();
}

/******************************
 * Function: roll()
 * Description: Draw runing clock.
 * Input: void
 * Output: void
 * Return: void
 ******************************/
void QClockProgress::roll()
{
    update();
    COUNT++;

    if (COUNT > 10) {
        COUNT = 0;
    }
}

void QClockProgress::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);
    // Draw animate clock
    QPoint cenPos = rect().center();
    QRect bgRect(cenPos.x() - 15, cenPos.y() - 15, 30, 30);
    painter.drawPixmap(bgRect,
                       QPixmap(":/images/MISC_PROGCLK_11states.png").copy(COUNT * 30, 0, 30, 30));

    // If true, draw progress text
    if (bIsTextVisible) {
        QString text;

        switch (m_format) {
        case Percentage:
            text = QString("%1 %").arg(m_curValue / m_maxValue);
            break;

        case Fraction:
            text = QString("%1/%2").arg(m_curValue).arg(m_maxValue);
            break;

        case UserType:
        default:
            text = QString();
            break;
        }

        // Draw progress text
        if (!text.isEmpty()) {
            QFont font("Arial", 8);
            font.setBold(true);
            painter.setFont(font);
            painter.setPen(Qt::white);
            painter.drawText(QRect(rect().x() - 1, rect().y() - 1, rect().width(),
                                   rect().height()), Qt::AlignCenter, text);
        }
    }
}
