#include "qprofile_global.h"
#include "qbackprogress.h"
#include <QHBoxLayout>
#include <QMetaObject>
#include <QDebug>
#include "qrserverstrtable.h"

QBaseProgress *QBackProgress::m_instance = nullptr;

QBaseProgress *QBackProgress::Instance()
{
    if (m_instance == nullptr) {
        m_instance = new QBackProgress(nullptr);
    }

    return m_instance;
}

qint32 QBackProgress::Run(quint32 interval, QObject *notifier,
                          const QString method, quint32 checkPeriod)
{
    return Instance()->run(interval, notifier, method, checkPeriod);
}

QBackProgress::QBackProgress(QWidget *parent)
    : QDialog(parent)
{
    if (parent != nullptr) {
        setFixedSize(300, 100);
    } else {
        setFixedSize(400, 200);
    }

    setWindowFlags(Qt::FramelessWindowHint | Qt::Widget);
    QVBoxLayout *pHLay = new QVBoxLayout(this);
    pHLay->addStretch();
    m_pMsgLabel = new QLabel(this);
    m_pMsgLabel->setText(TRANSLATE_STR_RSERVER_PROGRESS);

    if (parent != nullptr) {
        m_pMsgLabel->setFixedSize(270, 60);
    } else {
        m_pMsgLabel->setFixedWidth(370);
    }

    setTextColor(Qt::blue);

    if (parent != nullptr) {
        m_pMsgLabel->setAlignment(Qt::AlignHCenter);
    } else {
        m_pMsgLabel->setAlignment(Qt::AlignCenter);
    }

    pHLay->addWidget(m_pMsgLabel);
    m_pWaitProc = new QClockProgress(this);

    if (parent != nullptr) {
        m_pWaitProc->move(140, 60);
    } else {
        m_pWaitProc->move(170, 120);
    }

    pHLay->addStretch();
    m_pBorderWidths = new qint32[enBORDER_INDEX_MAX];
    m_pBorderWidths[enBORDER_INDEX_LEFT] = 2;
    m_pBorderWidths[enBORDER_INDEX_TOP] = 2;
    m_pBorderWidths[enBORDER_INDEX_RIGHT] = 2;
    m_pBorderWidths[enBORDER_INDEX_BOTTOM] = 2;
    m_borderColor = Qt::gray;
    setBackgroundColor(Qt::white);
    pHLay->setAlignment(Qt::AlignCenter);
    pHLay->setSpacing(5);
    m_timer.setSingleShot(true);
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(timeout()));
    connect(this, SIGNAL(finished(int)), this, SLOT(clear()));

    if (parent != nullptr) {
        move(200, 100);
    }
}

void QBackProgress::translate()
{
    m_pMsgLabel->setText(TRANSLATE_STR_RSERVER_PROGRESS);
}

QBackProgress::~QBackProgress()
{
    if (m_pWaitProc != nullptr) {
        delete m_pWaitProc;
    }

    if (m_pBorderWidths != nullptr) {
        delete m_pBorderWidths;
    }
}

void QBackProgress::setText(const QString &text)
{
    m_pMsgLabel->setText(text);
}

const QString QBackProgress::text()
{
    return m_pMsgLabel->text();
}

void QBackProgress::SetText(const QString &text)
{
    Instance()->setText(text);
}
const QString QBackProgress::Text()
{
    return Instance()->text();
}

void QBackProgress::setTextColor(const QColor &color)
{
    QPalette p = m_pMsgLabel->palette();
    p.setColor(QPalette::Foreground, color);
    m_pMsgLabel->setPalette(p);
}

void QBackProgress::setBackgroundColor(const QColor &color)
{
    m_bgColor = color;
    QPalette p = m_pMsgLabel->palette();
    p.setColor(QPalette::Background, color);
    m_pMsgLabel->setPalette(p);
}

bool QBackProgress::isActive() const
{
    return m_timer.isActive();
}

qint32 QBackProgress::run(quint32 interval, QObject *notifier ,
                          const QString method, quint32 checkPeriod)
{
    m_mutex.lock();
    m_pWaitProc->start();
    m_notifier = notifier;
    m_notifyMethod = method;

    if (interval < 2 * TimerCheckPeriod) {
        interval = 2 * TimerCheckPeriod;
    }

    if (checkPeriod < TimerCheckPeriod) {
        checkPeriod = TimerCheckPeriod;
    }

    m_interval = interval;
    m_timer.start(checkPeriod);
    m_mutex.unlock();
    QWidget *parent = qobject_cast<QWidget *>(this->parent());

    if (parent != nullptr) {
        parent->setDisabled(true);
    }

    return exec();
}

void QBackProgress::clear()
{
    m_mutex.lock();
    m_notifier = nullptr;
    m_notifyMethod = "";
    m_pWaitProc->stop();
    m_interval = 0;
    m_timer.stop();
    m_mutex.unlock();
    QWidget *parent = qobject_cast<QWidget *>(this->parent());

    if (parent != nullptr) {
        parent->setDisabled(false);
    }
}

void QBackProgress::finishWithResult(int result)
{
    done(result);
}

qint32 QBackProgress::Rest()
{
    return Instance()->rest();
}
qint32 QBackProgress::rest()
{
    return m_interval;
}

void QBackProgress::restart(qint32 interval)
{
    if (m_interval <= 0) {
        return;
    }

    m_interval = interval;
}

void QBackProgress::Restart(qint32 interval)
{
    Instance()->restart(interval);
}

void QBackProgress::timeout()
{
    m_mutex.lock();

    if (m_notifier != nullptr && !m_notifyMethod.isEmpty()) {
        bool retVal = false;
        QMetaObject::invokeMethod(m_notifier, m_notifyMethod.toLatin1().data(),
                                  Qt::DirectConnection,
                                  Q_RETURN_ARG(bool, retVal));

        if (retVal) {
            m_mutex.unlock();
            finishWithResult(QBaseProgress::Accepted);
            return;
        }
    }

    m_interval -= m_timer.interval();
    m_mutex.unlock();

    if (m_interval <= 0) {
        finishWithResult(Timeout);
    } else {
        m_timer.start();
    }
}

void QBackProgress::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    // Draw background color
    if (m_bgColor.isValid()) {
        painter.setPen(m_bgColor);
        painter.setBrush(QBrush(m_bgColor));
        painter.drawRect(0, 0, QWidget::width(), QWidget::height());
    }

    // Draw border, if border width greater than 0
    QPen pen;
    pen.setJoinStyle(Qt::MiterJoin);
    pen.setColor(m_borderColor.isValid() ? m_borderColor : m_bgColor);
    float offset = 0;
    QPointF xOffPosF;
    QPointF yOffPosF;

    for (qint32 i = 0; i < enBORDER_INDEX_MAX; i++) {
        if (m_pBorderWidths[i] <= 0) {
            continue;
        }

        offset = (float)((float)m_pBorderWidths[i] / (float)2) - 0.2;
        xOffPosF = QPointF(offset, 0);
        yOffPosF = QPointF(0, offset);
        pen.setWidth(m_pBorderWidths[i]);
        painter.setPen(pen);

        switch (i) {
        case enBORDER_INDEX_LEFT:
            painter.drawLine(rect().topLeft() + xOffPosF, rect().bottomLeft() + xOffPosF);
            break;

        case enBORDER_INDEX_TOP:
            painter.drawLine(rect().topLeft() + yOffPosF, rect().topRight() + yOffPosF);
            break;

        case enBORDER_INDEX_RIGHT:
            painter.drawLine(rect().topRight() - xOffPosF, rect().bottomRight() - xOffPosF);
            break;

        case enBORDER_INDEX_BOTTOM:
            painter.drawLine(rect().bottomLeft() - yOffPosF,
                             rect().bottomRight() - yOffPosF);
            break;

        default:
            break;
        }
    }

    return QDialog::paintEvent(event);
}
