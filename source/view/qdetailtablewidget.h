#ifndef QDETAILTABLEWIDGET_H_
#define QDETAILTABLEWIDGET_H_

#include "quploadfiletablewidget.h"

namespace QRserver
{
    class QDetailTableWidget : public QUploadFileTableWidget
    {
            Q_OBJECT

        public:
            QDetailTableWidget(QWidget *parent = 0);
            ~QDetailTableWidget();

            void addItems(const QStringList &pathList);
    };
};

#endif /* QDETAILTABLEWIDGET_H_ */
