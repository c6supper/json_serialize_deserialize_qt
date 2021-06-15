#ifndef QDOWNLOADPROFILETABLEWIDGET_H_
#define QDOWNLOADPROFILETABLEWIDGET_H_

#include "qpagetablewidget.h"
#include <QColor>

namespace QRserver
{
    class QDownloadProfileTableWidget : public QPageTableWidget
    {
            Q_OBJECT

        public:
            QDownloadProfileTableWidget(QWidget *parent = 0);
            ~QDownloadProfileTableWidget();

            void addItems(const QVariantList &varList);

        private:
            const QColor getColumnForegroundColor(int column, QString status) const;

        protected:
            virtual const QString columnToProperty(int column) const;
    };
};

#endif /* QPROFILETABLEWIDGET_H_ */
