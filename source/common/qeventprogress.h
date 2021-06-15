
#ifndef QEVENTPROGRESS_H_
#define QEVENTPROGRESS_H_

#include <QEventLoop>
#include <QTimer>
#include <QtCore/qglobal.h>
#include <QMutex>
#include "qprofile_global.h"
#include "qbaseprogress.h"

class QEventProgress : public QEventLoop, public QBaseProgress
{
        Q_OBJECT

    public:
        QEventProgress(QObject *parent = 0);
        ~QEventProgress();

        virtual int run(quint32 interval, QObject *notifier = nullptr,
                        const QString method = "", quint32 checkPeriod = TimerCheckPeriod);
        virtual void finishWithResult(int result);
        virtual void restart(qint32 interval);
        virtual bool isActive() const;

        static QBaseProgress *Instance();

    private slots:
        void timeout();
        void clear();

    private:
        QTimer               	m_timer;
        QObject					*m_notifier;
        QString					m_notifyMethod;
        qint32					m_interval;
        static QBaseProgress 	*m_instance;
        QMutex					m_mutex;
};

#endif /* QEVENTPROGRESS_H_ */
