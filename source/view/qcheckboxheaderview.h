#ifndef QCHECKBOXHEADERVIEW_H_
#define QCHECKBOXHEADERVIEW_H_

#include <QHeaderView>
#include <QCheckBox>
#include <QMouseEvent>

namespace QRserver
{
    class QCheckboxHeaderView : public QHeaderView
    {
            Q_OBJECT

        public:
            QCheckboxHeaderView(Qt::Orientation orientation = Qt::Horizontal,
                                QWidget *parent = 0);
            ~QCheckboxHeaderView();

        public:
            static const qint32 CheckBoxColumnIndex = 0;

        signals:
            void stateChanged(int state);

        private:
            QCheckBox *m_pCheckBox;

        protected:
            virtual void paintSection(QPainter *painter, const QRect &rect,
                                      int logicalIndex) const;
            virtual void mousePressEvent(QMouseEvent *event);
            virtual void mouseReleaseEvent(QMouseEvent *event);
    };
};

#endif //QCHECKBOXHEADERVIEW_H_
