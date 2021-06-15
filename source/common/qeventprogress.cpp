#include "qprofile_global.h"
#include "qeventprogress.h"
#include <QMetaObject>
#include <QDebug>

QBaseProgress *QEventProgress::m_instance = nullptr;

QBaseProgress *QEventProgress::Instance()
{
    if (m_instance == nullptr) {
        m_instance = new QEventProgress(nullptr);
    }

    return m_instance;
}

QEventProgress::QEventProgress(QObject *parent)
    : QEventLoop(parent)
{
    m_timer.setSingleShot(true);
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(timeout()));
}

QEventProgress::~QEventProgress()
{
}

bool QEventProgress::isActive() const
{
    return m_timer.isActive();
}

qint32 QEventProgress::run(quint32 interval, QObject *notifier ,
                           const QString method, quint32 checkPeriod)
{
    m_mutex.lock();
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

void QEventProgress::clear()
{
    m_mutex.lock();
    m_notifier = nullptr;
    m_notifyMethod = "";
    m_interval = 0;
    m_timer.stop();
    m_mutex.unlock();
    QWidget *parent = qobject_cast<QWidget *>(this->parent());

    if (parent != nullptr) {
        parent->setDisabled(false);
    }
}

void QEventProgress::restart(qint32 interval)
{
    if (m_interval <= 0) {
        return;
    }

    m_interval = interval;
}

void QEventProgress::timeout()
{
    m_mutex.lock();

    if (m_notifier != nullptr && !m_notifyMethod.isEmpty()) {
        bool retVal = false;
        QMetaObject::invokeMethod(m_notifier, m_notifyMethod.toLatin1().data(),
                                  Qt::DirectConnection,
                                  Q_RETURN_ARG(bool, retVal));

        if (retVal) {
            m_mutex.unlock();
            finishWithResult(Accepted);
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

void QEventProgress::finishWithResult(int result)
{
    clear();
    exit(result);
}

