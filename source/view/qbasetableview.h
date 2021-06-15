#ifndef QBASETABLEVIEW_H_
#define QBASETABLEVIEW_H_

#include <QTableView>

namespace QRserver
{
    class QBaseTableView : public QTableView
    {
            Q_OBJECT

        public:
            QBaseTableView(QWidget *parent = 0);
            ~QBaseTableView();
    };
};

#endif //QBASETABLEVIEW_H_
