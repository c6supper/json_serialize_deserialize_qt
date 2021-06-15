
#ifndef QBACKPROGRESS_H_
#define QBACKPROGRESS_H_

#include <QDialog>
#include "qclockprogress.h"
#include <QLabel>
#include <QTimer>
#include <QPaintEvent>
#include <QPainter>
#include <QtCore/qglobal.h>
#include <QMutex>
#include "qprofile_global.h"
#include "qbaseprogress.h"

enum enBorderIndex {
    enBORDER_INDEX_LEFT = 0,
    enBORDER_INDEX_TOP,
    enBORDER_INDEX_RIGHT,
    enBORDER_INDEX_BOTTOM,
    enBORDER_INDEX_MAX,
    enBORDER_INDEX_INVALID
};

class QBackProgress : public QDialog, public QBaseProgress
{
        Q_OBJECT

    public:
        QBackProgress(QWidget *parent = 0);
        ~QBackProgress();
        virtual void setText(const QString &text);
        virtual const QString text();
        void setTextColor(const QColor &color);
        void setBackgroundColor(const QColor &color);
        void translate();
        virtual int run(quint32 interval, QObject *notifier = nullptr,
                        const QString method = "", quint32 checkPeriod = TimerCheckPeriod);
        virtual void restart(qint32 interval);
        virtual qint32 rest();
        virtual bool isActive() const;
        virtual void finishWithResult(int result);

        static qint32 Run(quint32 interval, QObject *notifier = nullptr,
                          const QString method = "", quint32 checkPeriod = TimerCheckPeriod);
        static QBaseProgress *Instance();
        static void Restart(qint32 interval);
        static qint32 Rest();
        static void SetText(const QString &text);
        static const QString Text();

    private slots:
        void timeout();
        void clear();

    private:
        void paintEvent(QPaintEvent *event);

    private:
        QLabel               	*m_pMsgLabel;
        QClockProgress       	*m_pWaitProc;
        qint32                 	*m_pBorderWidths;	// four border wdith array
        QColor                	m_bgColor;	    // background color
        QColor                	m_borderColor;	// border color
        QTimer               	m_timer;
        QObject					*m_notifier;
        QString					m_notifyMethod;
        qint32					m_interval;
        static QBaseProgress 	*m_instance;
        QMutex					m_mutex;
};

#endif /* QBACKPROGRESS_H_ */
