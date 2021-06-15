#ifndef QPAGEBAR_H
#define QPAGEBAR_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>

class QPageBar : public QWidget
{
        Q_OBJECT

    public:
        QPageBar(QWidget *parent = 0);
        ~QPageBar();
        void addPage();
        void removePage(int index);
        void setPageCount(int cnt);
        int count() const;
        int currentIndex() const;
        void clear();

    signals:
        void pageChanged(int index);

    public slots:
        void setCurrentIndex(int index);
        void update();

    private slots:
        void leftClicked();
        void rightClicked();

    protected:
        void paintEvent(QPaintEvent *event);

    private:
        void setText();

    private:
        int m_count;
        int m_index;
        QString m_text;
        QLabel *m_pLabelInfo;
        QPushButton *m_pBtnLeft;
        QPushButton *m_pBtnRight;
};

#endif // QPAGEBAR_H
