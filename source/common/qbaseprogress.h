#ifndef QBASEPROGRESS_H_
#define QBASEPROGRESS_H_

#include <QString>
#include <QObject>
#include <QDialog>
#include "qprofile_global.h"

class QBaseProgress
{
    public:
        enum ProgressCode { Rejected = QDialog::Rejected, Accepted = QDialog::Accepted, ForceClosed, Timeout };
        static const quint32 TimerCheckPeriod;

    public:
        virtual ~QBaseProgress() {};
        virtual int run(quint32 interval, QObject *notifier = nullptr,
                        const QString method = "", quint32 checkPeriod = TimerCheckPeriod) = 0;
        virtual void finishWithResult(int result) = 0;
        virtual void setText(const QString &text) {};
        virtual const QString text()
        {
            return QString();
        };
        virtual qint32 rest()
        {
            return 0;
        };
        virtual void restart(qint32 interval) = 0;
        virtual bool isActive() const = 0;
};

#endif  /* QBASEPROGRESS_H_ */
