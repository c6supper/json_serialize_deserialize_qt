#ifndef QBASECOMBOBOX_H
#define QBASECOMBOBOX_H

#include "qpagebar.h"
#include <QComboBox>
#include <QStringList>
#include <QVBoxLayout>

// TODO: move class to separate file
class QPageListWidget;

class QBaseComboBox : public QComboBox
{
        Q_OBJECT

    public:
        QBaseComboBox(QWidget *parent = 0);
        ~QBaseComboBox();
        void addItems(const QStringList &texts);
        void setVisibleItem(const QString &text);
        void setVisibleItems(const QStringList &texts, const QString &current = 0);
        void setVisibleItems(const QStringList &texts, int current);
        void setVisibleItems(const QList<int> &indexs, int current);
        void setVisibleItems(quint32 indcator, quint32 current);
        void setVisibleItems(quint64 indcator, quint32 current);
        void setInvisibleItems(const QList<int> &indexs, int current);
        void setFont(const QFont &font);
        int currentIndex() const;
        void setTitle(const QString &title);
        void setAutoStyleEnabled(bool b);
        bool isAutoStyleEnabled() const;
        void setListItemWidth(int width);
        void setListItemAlignment(Qt::Alignment align);

    signals:
        void currentValueChanged(const QString &text);
        void currentValueChanged(int index);
        void currentItemActivated(int index);

    public slots:
        void setCurrentIndex(int index);

    private slots:
        void changeCurrentText(const QString &text);
        void changeCurrentItem(const QString &text);
        void changeCurrentItem(int index);
        void itemActivated(const QString &text);

    protected:
        bool eventFilter(QObject *obj, QEvent *event);

        // Reimplement QComboBox showPopup()/hidePopup() to show a custom pop-up
        void showPopup();
        void hidePopup();

    private:
        bool isChanged(const QList<int> &indexs, int current) const;
        bool isChanged(const QStringList &texts, int current) const;

    private:
        int m_currentIndex;
        QStringList m_items;
        QPageListWidget *m_pItemListWidget;
};

class QPageListWidgetItem : public QWidget
{
        Q_OBJECT

    public:
        QPageListWidgetItem(const QString &text, QWidget *parent = 0);
        QPageListWidgetItem(const QIcon &icon, const QString &text,
                            QWidget *parent = 0);
        ~QPageListWidgetItem();
        void setIndex(int index);
        int index() const;
        void setText(const QString &text);
        QString text() const;
        void setSelect(bool b);
        void setHoverIn();
        void setHoverOut();
        void setActive(bool b);
        bool isActive() const;
        void clear();
        void setTextAlignment(Qt::Alignment align);

    protected:
        void paintEvent(QPaintEvent *event);

    private:
        bool initialize();

    private:
        int m_index;
        QString m_text;
        QIcon m_icon;
        bool bIsSelected;
        bool bIsHovered;
        bool bIsActive;
        QColor m_selectColor;
        QColor m_hoverColor;
        Qt::Alignment m_textAlign;
};

class QPageListWidgetHeader : public QWidget
{
        Q_OBJECT

    public:
        QPageListWidgetHeader(QWidget *parent = 0);
        QPageListWidgetHeader(const QString &title, const QString &text,
                              QWidget *parent = 0);
        ~QPageListWidgetHeader();
        void setTitle(const QString &title);
        void setText(const QString &text);
        QString text() const;

    protected:
        void paintEvent(QPaintEvent *event);

    private:
        bool initialize();

    private:
        QString m_title;
        QString m_text;
};

class QPageListWidgetItemArea : public QWidget
{
        Q_OBJECT

    public:
        QPageListWidgetItemArea(QWidget *parent = 0);
        ~QPageListWidgetItemArea();
        void setSize(int width, int height);
        void setMaxVisibleRows(int rows);
        void addItems(const QStringList &texts);
        void clear();
        void setCurrentIndex(int index);
        int currentIndex() const;
        QString currentText() const;
        void setVisibleItems(const QList<int> &indexs);
        void setItemAlignment(Qt::Alignment align);

    signals:
        void currentItemChanged(const QString &text);
        void currentItemChanged(int index);

    public slots:
        void setCurrentPage(int index);

    private:
        void updateStatus();
        void adjustItemSize(QPageListWidgetItem *item);

    protected:
        void mouseMoveEvent(QMouseEvent *event);
        void mousePressEvent(QMouseEvent *event);
        void mouseReleaseEvent(QMouseEvent *event);

    private:
        int m_pageIndex;
        int m_currentIndex;
        int m_maxVisibleRows;
        QString m_selectValue;
        QVBoxLayout *m_pMainLayout;
        QPageListWidgetItem *m_pPreItem;
        QPageListWidgetItem *m_pCurItem;
        QPageListWidgetItem *m_pHoverItem;
        QList<QPageListWidgetItem *> m_itemList;
        QList<QPageListWidgetItem *> m_visibleItemList;
        Qt::Alignment m_itemAlign;
};

class QPageListWidget : public QWidget
{
        Q_OBJECT

    public:
        QPageListWidget(QWidget *parent = 0);
        ~QPageListWidget();
        void setSize(int width, int height);
        void addItem(const QString &text);
        void addItems(const QStringList &texts);
        void setVisibleItems(const QList<int> &indexs);
        void setTitle(const QString &title);
        void setText(const QString &text);
        QString selectItem() const;
        void show(const QString &text);
        void show(int index);
        void clear();
        QStringList visibleItems() const;
        QList<int> visibleItemsIndex() const;
        void setItemAreaAlignment(Qt::Alignment align);

    signals:
        void currentItemChanged(const QString &text);
        void currentItemChanged(int index);

    public slots:
        void setCurrentIndex(int index);

    private:
        void adjustPageCount();
        void adjustHeight();
        QStringList getCurrentItems() const;

    protected:
        void resizeEvent(QResizeEvent *event);
        void paintEvent(QPaintEvent *event);

    private:
        int m_maxVisibleRows;
        QString m_title;
        QString m_curValue;
        QStringList m_itemList;
        QList<int> m_indexList;
        QPageListWidgetHeader *m_pTitleArea;
        QPageListWidgetItemArea *m_pItemArea;
        QPageBar *m_pPageBar;
};

#endif /* QBASECOMBOBOX_H */
