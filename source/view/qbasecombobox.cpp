#include <QApplication>
#include <QPainter>
#include <QListView>
#include <math.h>
#include <QMouseEvent>
#include "qbasecombobox.h"

QBaseComboBox::QBaseComboBox(QWidget *parent)
    : QComboBox(parent),
      m_currentIndex(0)
{
    setObjectName("QBaseComboBox");
    this->installEventFilter(this);
    m_pItemListWidget = new QPageListWidget(this);
    m_pItemListWidget->hide();
    connect(this, SIGNAL(currentIndexChanged(const QString &)),
            SLOT(changeCurrentText(const QString &)));
    connect(this, SIGNAL(activated(const QString &)),
            SLOT(itemActivated(const QString &)));
    connect(m_pItemListWidget, SIGNAL(currentItemChanged(const QString &)),
            SLOT(changeCurrentItem(const QString &)));
    connect(m_pItemListWidget, SIGNAL(currentItemChanged(int)),
            SLOT(changeCurrentItem(int)));
}

QBaseComboBox::~QBaseComboBox()
{
}

void QBaseComboBox::addItems(const QStringList &texts)
{
    if (m_items == texts) {
        return;
    }

    setVisibleItems(texts, 0);
    m_items = texts;
    m_pItemListWidget->clear();
    m_pItemListWidget->addItems(texts);
}

void QBaseComboBox::setVisibleItem(const QString &text)
{
    setVisibleItems(QStringList() << text, text);
}

void QBaseComboBox::setVisibleItems(const QStringList &texts,
                                    const QString &current)
{
    int ci = m_items.indexOf(current);

    // if current is NULL, set current index as previous item index
    if (ci < 0) {
        ci = m_currentIndex;
    }

    if (ci > m_items.size()) {
        ci = m_items.size() - 1;
    }

    setVisibleItems(texts, ci);
}

void QBaseComboBox::setVisibleItems(const QStringList &texts, int current)
{
    // If items are not change, don't set combobox items
    if (!isChanged(texts, current)) {
        return;
    }

    blockSignals(true);
    QComboBox::clear();

    if (current <= 0) {
        blockSignals(false);
        QComboBox::addItems(texts);
    } else {
        QComboBox::addItems(texts);
        blockSignals(false);
        setCurrentIndex(current);
    }

    // Visible items index
    QList<int> vIndexList;

    for (int i = 0; i < m_items.size(); i++) {
        if (texts.contains(m_items.at(i))) {
            vIndexList << i;
        }
    }

    setVisibleItems(vIndexList, current);
}

void QBaseComboBox::setVisibleItems(const QList<int> &indexs, int current)
{
    // If items are not change, don't set combobox items
    if (!isChanged(indexs, current)) {
        return;
    }

    m_currentIndex = current;
    m_pItemListWidget->setVisibleItems(indexs);
    m_pItemListWidget->setCurrentIndex(current);
}

void QBaseComboBox::setVisibleItems(quint32 indcator, quint32 current)
{
    // If indcator equals 0, the combobox invisible
    setCurrentIndex(current);
    setVisible(indcator);

    if (indcator == 0) {
        return;
    }

    QList<int> indexs;

    for (size_t i = 0; i < sizeof(quint32) * 8; i++) {
        if (((quint32)1 << i) & indcator) {
            indexs << i;
        }
    }

    // set visible items and current item
    int ci = current;

    if (indexs.isEmpty()) {
        ci = 0;
    } else {
        if (!indexs.contains(ci)) {
            ci = indexs.first();
        }
    }

    setVisibleItems(indexs, ci);
}

void QBaseComboBox::setVisibleItems(quint64 indcator, quint32 current)
{
    // If indcator equals 0, the combobox invisible
    setCurrentIndex(current);
    setVisible(indcator);

    if (indcator == 0) {
        return;
    }

    QList<int> indexs;

    for (size_t i = 0; i < sizeof(quint64) * 8; i++) {
        if (((quint64)1 << i) & indcator) {
            indexs << i;
        }
    }

    // set visible items and current item
    int ci = current;

    if (indexs.isEmpty()) {
        ci = 0;
    } else {
        if (!indexs.contains(ci)) {
            ci = indexs.first();
        }
    }

    setVisibleItems(indexs, ci);
}

void QBaseComboBox::setInvisibleItems(const QList<int> &indexs, int current)
{
    QList<int> vIndexs;

    for (int i = 0; i < m_items.size(); i++) {
        if (!indexs.contains(i) && !vIndexs.contains(i)) {
            vIndexs << i;
        }
    }

    setVisibleItems(vIndexs, current);
}

bool QBaseComboBox::isChanged(const QList<int> &indexs, int current) const
{
    return ((m_pItemListWidget->visibleItemsIndex() != indexs)
            || (m_currentIndex != current));
}

bool QBaseComboBox::isChanged(const QStringList &texts, int current) const
{
    return ((m_pItemListWidget->visibleItems() != texts)
            || (m_currentIndex != current));
}

void QBaseComboBox::setFont(const QFont &font)
{
    // Store style sheet
    QString ss = this->styleSheet();
    // Clear previous style sheet
    setStyleSheet("");
    QComboBox::setFont(font);
    m_pItemListWidget->setFont(font);
    // Restore style sheet
    setStyleSheet(ss);
}

int QBaseComboBox::currentIndex() const
{
    return m_currentIndex;
}

void QBaseComboBox::setTitle(const QString &title)
{
    m_pItemListWidget->setTitle(title);
}

void QBaseComboBox::setCurrentIndex(int index)
{
    int ci = index;

    if (index < 0 || index >= m_items.size()) {
        ci = 0;
    }

    m_currentIndex = ci;
    // from real index to combobox index
    ci = QComboBox::findText(m_items.at(ci),
                             Qt::MatchFixedString | Qt::MatchCaseSensitive);
    QComboBox::setCurrentIndex(ci > 0 ? ci : 0);
}

void QBaseComboBox::changeCurrentText(const QString &text)
{
    // correct index or -1, if there is not text
    changeCurrentItem(text);
}

void QBaseComboBox::changeCurrentItem(const QString &text)
{
    // Hide customize popup
    hidePopup();
    // emit current item text
    int idx = QComboBox::findText(text,
                                  Qt::MatchFixedString | Qt::MatchCaseSensitive);

    if (idx != m_currentIndex) {
        QComboBox::setCurrentIndex(idx);
        emit currentValueChanged(text);
    }
}

void QBaseComboBox::changeCurrentItem(int index)
{
    // Hide customize popup
    hidePopup();

    // from real index to combobox index
    if (index != m_currentIndex) {
        m_currentIndex = index;
        QComboBox::setCurrentIndex(QComboBox::findText(m_items.at(index),
                                   Qt::MatchFixedString | Qt::MatchCaseSensitive));
        emit currentValueChanged(index);
        emit activated(index);
        emit currentItemActivated(index);
    }
}

void QBaseComboBox::itemActivated(const QString &text)
{
    // correct index or -1, if there is not text
    int index = m_items.indexOf(text);

    if (index != m_currentIndex) {
        emit currentItemActivated(index <= 0 ? 0 : index);
        emit activated(index <= 0 ? 0 : index);
    }
}

void QBaseComboBox::setListItemWidth(int width)
{
    m_pItemListWidget->setFixedWidth(width);
}

void QBaseComboBox::setListItemAlignment(Qt::Alignment align)
{
    m_pItemListWidget->setItemAreaAlignment(align);
}

bool QBaseComboBox::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        if ((keyEvent->key() == Qt::Key_F1)
            || (keyEvent->key() == Qt::Key_F2)
            || (keyEvent->key() == Qt::Key_F3)
            || (keyEvent->key() == Qt::Key_F4)
            || (keyEvent->key() == Qt::Key_Escape)
            || (keyEvent->key() == Qt::Key_Return)
            || (keyEvent->key() == Qt::Key_Home)
            || (keyEvent->key() == Qt::Key_Left)
            || (keyEvent->key() == Qt::Key_Up)
            || (keyEvent->key() == Qt::Key_Right)
            || (keyEvent->key() == Qt::Key_Down)) {
            event->ignore();
            return true;
        }
    }

    return QComboBox::eventFilter(obj, event);
}

void QBaseComboBox::showPopup()
{
    m_pItemListWidget->show(m_currentIndex);
    QPoint cenPos = qApp->activeWindow()->geometry().center();
    m_pItemListWidget->move(cenPos.x() - m_pItemListWidget->width() / 2,
                            cenPos.y() - m_pItemListWidget->height() / 2);
}

void QBaseComboBox::hidePopup()
{
    QComboBox::hidePopup();
    m_pItemListWidget->hide();
}

const static int ICON_WIDTH = 20;
const static int ICON_HEIGHT = 20;

QPageListWidgetItem::QPageListWidgetItem(const QString &text, QWidget *parent)
    : QWidget(parent),
      m_index(-1),
      m_text(text),
      bIsSelected(false),
      bIsHovered(false),
      m_selectColor(Qt::blue),
      m_hoverColor(QColor(232, 242, 254)),
      m_textAlign(Qt::AlignTrailing)
{
    initialize();
}

QPageListWidgetItem::QPageListWidgetItem(const QIcon &icon, const QString &text,
        QWidget *parent)
    : QWidget(parent),
      m_text(text),
      m_icon(icon),
      bIsSelected(false),
      bIsHovered(false),
      bIsActive(true),
      m_textAlign(Qt::AlignTrailing)
{
    initialize();
}

QPageListWidgetItem::~QPageListWidgetItem()
{
}

bool QPageListWidgetItem::initialize()
{
    setObjectName("QPageListWidgetItem");
    setMouseTracking(true);
    return true;
}

void QPageListWidgetItem::setIndex(int index)
{
    m_index = index;
}

int QPageListWidgetItem::index() const
{
    return m_index;
}

void QPageListWidgetItem::setText(const QString &text)
{
    m_text = text;
    update();
}

QString QPageListWidgetItem::text() const
{
    return m_text;
}

void QPageListWidgetItem::setSelect(bool b)
{
    bIsSelected = b;
    update();
}

void QPageListWidgetItem::setHoverIn()
{
    bIsHovered = true;
    update();
}

void QPageListWidgetItem::setHoverOut()
{
    bIsHovered = false;
    update();
}

void QPageListWidgetItem::setActive(bool b)
{
    bIsActive = b;

    if (!bIsActive) {
        setVisible(false);
    }
}

bool QPageListWidgetItem::isActive() const
{
    return bIsActive;
}

void QPageListWidgetItem::clear()
{
    bIsSelected = false;
    bIsHovered = false;
    m_icon = QIcon();
    m_text = "";
    update();
}

void QPageListWidgetItem::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);

    // High light background color
    if (bIsSelected) {
        if (bIsHovered) {
            painter.setPen(m_selectColor);
            painter.fillRect(rect(), QBrush(m_selectColor));
        } else {
            painter.setPen(m_selectColor);
            painter.fillRect(rect(), QBrush(m_selectColor));
        }
    } else {
        if (bIsHovered) {
            painter.setPen(m_hoverColor);
            painter.fillRect(rect(), QBrush(m_hoverColor));
        }
    }

    // Draw icon
    int DIST = 10;

    if (!m_icon.isNull()) {
        DIST += ICON_WIDTH;
        QPoint cenPos = QRect(0, 0, ICON_WIDTH, height()).center();
        painter.drawPixmap(QRect(cenPos.x() - ICON_WIDTH / 2,
                                 cenPos.y() - ICON_HEIGHT / 2, ICON_WIDTH, ICON_HEIGHT),
                           m_icon.pixmap(QSize(ICON_WIDTH, ICON_HEIGHT)));
    }

    // Draw text
    if (!m_text.isEmpty()) {
        painter.setPen(bIsSelected ? Qt::white : Qt::black);
        painter.setFont(font());
        painter.drawText(QRect(rect().x(), rect().y(), rect().width() - 1,
                               rect().height()), m_textAlign | Qt::AlignVCenter, m_text);
    }
}

void QPageListWidgetItem::setTextAlignment(Qt::Alignment align)
{
    m_textAlign = align;
}

QPageListWidgetHeader::QPageListWidgetHeader(QWidget *parent)
    : QWidget(parent)
{
    initialize();
}

QPageListWidgetHeader::QPageListWidgetHeader(const QString &title,
        const QString &text, QWidget *parent)
    : QWidget(parent),
      m_title(title),
      m_text(text)
{
    initialize();
}

QPageListWidgetHeader::~QPageListWidgetHeader()
{
}

bool QPageListWidgetHeader::initialize()
{
    setObjectName("QPageListWidgetHeader");
    return true;
}

void QPageListWidgetHeader::setTitle(const QString &title)
{
    m_title = title;
    update();
}

void QPageListWidgetHeader::setText(const QString &text)
{
    m_text = text;
    update();
}

QString QPageListWidgetHeader::text() const
{
    return m_text;
}

void QPageListWidgetHeader::paintEvent(QPaintEvent *event)
{
    // subclass from QWidget need to provide a paintEvent for custom QWidget as below
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    QWidget::paintEvent(event);
    QPainter painter(this);

    // Draw title
    if (!m_title.isEmpty()) {
        painter.setFont(font());
        painter.drawText(QRect(rect().x() + 10, rect().y() + 3, rect().width(),
                               rect().height()), Qt::AlignLeft | Qt::AlignTop, m_title);
    }

    // Draw text
    if (!m_text.isEmpty()) {
        painter.setFont(font());
        painter.drawText(QRect(rect().x(), rect().y(), rect().width() - 3,
                               rect().height() - 4), Qt::AlignRight | Qt::AlignBottom, m_text);
    }
}

QPageListWidgetItemArea::QPageListWidgetItemArea(QWidget *parent)
    : QWidget(parent),
      m_pageIndex(0),
      m_currentIndex(0),
      m_maxVisibleRows(8),
      m_itemAlign(Qt::AlignTrailing)
{
    setObjectName("QPageListWidgetItemArea");
    m_pMainLayout = new QVBoxLayout(this);
    m_pMainLayout->setContentsMargins(0, 2, 0, 0);
    m_pMainLayout->setSpacing(1);
    m_pMainLayout->setAlignment(Qt::AlignCenter);
    m_pMainLayout->addStretch();
    // Enable mouse move event
    setMouseTracking(true);
    setMaxVisibleRows(m_maxVisibleRows);
    // Init current item/previous item
    m_pPreItem = NULL;
    m_pCurItem = NULL;
    m_pHoverItem = NULL;
}

QPageListWidgetItemArea::~QPageListWidgetItemArea()
{
}

void QPageListWidgetItemArea::setSize(int width, int height)
{
    QWidget::setFixedSize(width, height);
    QPageListWidgetItem *pItem = NULL;

    for (int i = 0; i < m_itemList.size(); i++) {
        pItem = m_itemList.at(i);
        adjustItemSize(pItem);
    }
}

void QPageListWidgetItemArea::setMaxVisibleRows(int rows)
{
    m_maxVisibleRows = rows;
    updateStatus();
}

void QPageListWidgetItemArea::addItems(const QStringList &texts)
{
    QPageListWidgetItem *pItem = NULL;

    for (int i = 0; i < texts.size(); i++) {
        pItem = new QPageListWidgetItem(texts.at(i), this);
        pItem->setTextAlignment(m_itemAlign);
        pItem->setIndex(m_itemList.size());
        m_itemList << pItem;
        adjustItemSize(pItem);
        pItem->setVisible(false);
        pItem->setActive(false);
        m_pMainLayout->insertWidget(m_pMainLayout->count() - 1, pItem);
        pItem->installEventFilter(this);
    }
}

void QPageListWidgetItemArea::adjustItemSize(QPageListWidgetItem *pItem)
{
    if (pItem == NULL)
        return;

    // border_width * 2 + 2
    pItem->setFixedWidth(width() - 6);
}

void QPageListWidgetItemArea::clear()
{
    QPageListWidgetItem *pItem = NULL;

    for (int i = 0; i < m_itemList.size(); i++) {
        pItem = m_itemList.at(i);
        pItem->removeEventFilter(this);
    }

    qDeleteAll(m_itemList);
    m_itemList.clear();
    m_visibleItemList.clear();
    // Reset
    m_pPreItem = NULL;
    m_pCurItem = NULL;
    m_pHoverItem = NULL;
    m_currentIndex = 0;
    m_pageIndex = 0;
}

void QPageListWidgetItemArea::updateStatus()
{
    const int START_INDEX = m_pageIndex * m_maxVisibleRows;

    if (START_INDEX < 0 || START_INDEX >= m_visibleItemList.size()) {
        return;
    }

    const int END_INDEX = (m_visibleItemList.size() > (START_INDEX +
                           m_maxVisibleRows)) ? (START_INDEX + m_maxVisibleRows) :
                          m_visibleItemList.size();
    QPageListWidgetItem *pItem = NULL;

    for (int i = 0; i < m_visibleItemList.size(); i++) {
        pItem = m_visibleItemList.at(i);

        if (pItem->isActive()) {
            pItem->setVisible(i >= START_INDEX && i < END_INDEX);
            pItem->setSelect(pItem->index() == m_currentIndex && pItem->isVisible());
        }
    }
}

void QPageListWidgetItemArea::setCurrentPage(int index)
{
    m_pageIndex = index;
    updateStatus();
}

void QPageListWidgetItemArea::setCurrentIndex(int index)
{
    m_currentIndex = index;
    updateStatus();
}

int QPageListWidgetItemArea::currentIndex() const
{
    return m_currentIndex;
}

QString QPageListWidgetItemArea::currentText() const
{
    return m_selectValue;
}

void QPageListWidgetItemArea::setVisibleItems(const QList<int> &indexs)
{
    m_visibleItemList.clear();
    QPageListWidgetItem *pItem = NULL;

    for (int i = 0; i < m_itemList.size(); i++) {
        pItem = m_itemList.at(i);
        pItem->setVisible(false);
        pItem->setActive(indexs.contains(pItem->index()));

        if (indexs.contains(pItem->index())) {
            pItem->setVisible(true);
            m_visibleItemList << pItem;
        }
    }

    updateStatus();
}

void QPageListWidgetItemArea::mouseMoveEvent(QMouseEvent *event)
{
    QWidget::mouseMoveEvent(event);

    if (m_pHoverItem != NULL) {
        m_pHoverItem->setHoverOut();
    }

    m_pHoverItem = qobject_cast<QPageListWidgetItem *>(childAt(event->pos()));

    if (m_pHoverItem != NULL) {
        m_pHoverItem->setHoverIn();
    }
}

void QPageListWidgetItemArea::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    // Reset
    m_pPreItem = m_pCurItem = NULL;
    // handle item at mouse press position
    m_pPreItem = qobject_cast<QPageListWidgetItem *>(childAt(event->pos()));
}

void QPageListWidgetItemArea::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);
    // handle item at mouse release position
    m_pCurItem = qobject_cast<QPageListWidgetItem *>(childAt(event->pos()));

    // if press item equals to release item, the release item is selected item
    if (m_pCurItem == m_pPreItem && m_pCurItem != NULL) {
        m_pCurItem->setSelect(true);
        m_selectValue = m_pCurItem->text();
        m_currentIndex = m_pCurItem->index();
        emit currentItemChanged(m_selectValue);
        emit currentItemChanged(m_currentIndex);
    }

    m_pPreItem = m_pCurItem = NULL;
}

void QPageListWidgetItemArea::setItemAlignment(Qt::Alignment align)
{
    m_itemAlign = align;
    QPageListWidgetItem *pItem = NULL;

    for (int i = 0; i < m_itemList.size(); i++) {
        pItem = m_itemList.at(i);
        pItem->setTextAlignment(m_itemAlign);
    }
}

QPageListWidget::QPageListWidget(QWidget *parent)
    : QWidget(parent), m_maxVisibleRows(8)
{
    setObjectName("QPageListWidget");
    setWindowFlags(Qt::Popup);
    // Title area
    m_pTitleArea = new QPageListWidgetHeader(this);
    // Item area
    m_pItemArea = new QPageListWidgetItemArea(this);
    m_pItemArea->setMaxVisibleRows(m_maxVisibleRows);
    // Page bar
    m_pPageBar = new QPageBar(this);
    m_pPageBar->setObjectName("QPageListWidgetQPageBar");
    m_pPageBar->setVisible(false);
    connect(m_pItemArea, SIGNAL(currentItemChanged(const QString &)),
            SIGNAL(currentItemChanged(const QString &)));
    connect(m_pItemArea, SIGNAL(currentItemChanged(int)),
            SIGNAL(currentItemChanged(int)));
    connect(m_pPageBar, SIGNAL(pageChanged(int)), m_pItemArea,
            SLOT(setCurrentPage(int)));
}

QPageListWidget::~QPageListWidget()
{
}

void QPageListWidget::setSize(int width, int height)
{
    QWidget::setFixedSize(width, height);
    adjustHeight();
}

void QPageListWidget::addItem(const QString &text)
{
    m_itemList.push_back(text);
    addItems(m_itemList);
}

void QPageListWidget::addItems(const QStringList &texts)
{
    m_itemList = texts;
    m_pItemArea->addItems(m_itemList);
    // Init all items visible
    QList<int> indexList;

    for (int i = 0; i < m_itemList.size(); i++) {
        if (!indexList.contains(i)) {
            indexList << i;
        }
    }

    setVisibleItems(indexList);
}

void QPageListWidget::setVisibleItems(const QList<int> &indexs)
{
    m_indexList = indexs;
    m_pItemArea->setVisibleItems(indexs);
    // Store previous index
    int ci = m_pPageBar->currentIndex();
    // If summary item height greater than widget's height, page bar visible
    m_pPageBar->setVisible(m_indexList.size() > m_maxVisibleRows);
    adjustHeight();
    adjustPageCount();

    // Restore previous index
    if (ci < 0) {
        ci = 0;
    } else if (ci > m_pPageBar->count() - 1) {
        ci = m_pPageBar->count() - 1;
    }

    m_pPageBar->setCurrentIndex(ci);
}

void QPageListWidget::setCurrentIndex(int index)
{
    m_pItemArea->setCurrentIndex(index);
}

void QPageListWidget::setTitle(const QString &title)
{
    m_title = title;
    m_pTitleArea->setTitle(title);
}

void QPageListWidget::setText(const QString &text)
{
    m_curValue = text;
    m_pTitleArea->setText(text);
}

void QPageListWidget::show(const QString &text)
{
    QWidget::show();
    m_pTitleArea->setText(text);
    m_pItemArea->setCurrentIndex(m_itemList.indexOf(text));
    adjustHeight();
}

void QPageListWidget::show(int index)
{
    QWidget::show();
    m_pPageBar->setVisible(m_indexList.size() > m_maxVisibleRows);
    adjustHeight();
    adjustPageCount();
    int pageIndex = ceil((float)(m_indexList.indexOf(index) + 1) /
                         (float)m_maxVisibleRows) - 1;
    m_pPageBar->setCurrentIndex(pageIndex);
    m_pTitleArea->setText((index >= 0
                           && index < m_itemList.size()) ? m_itemList.at(index) : "");
    m_pItemArea->setCurrentPage(pageIndex);
    m_pItemArea->setCurrentIndex(index);
}

void QPageListWidget::clear()
{
    m_pItemArea->clear();
}

QStringList QPageListWidget::visibleItems() const
{
    QStringList vItems;

    for (int i = 0; i < m_indexList.size(); i++) {
        vItems << m_itemList.at(m_indexList.at(i));
    }

    return vItems;
}

QList<int> QPageListWidget::visibleItemsIndex() const
{
    return m_indexList;
}

void QPageListWidget::adjustPageCount()
{
    // Clear previous page count
    m_pPageBar->clear();

    // Calculate current page count
    for (int i = 0; i < m_indexList.size(); i++) {
        if ((i + 1) % m_maxVisibleRows == 0) {
            m_pPageBar->addPage();
        }
    }

    // Add extra one page
    if (m_indexList.size() % m_maxVisibleRows != 0) {
        m_pPageBar->addPage();
    }
}

void QPageListWidget::adjustHeight()
{
    const int ADJUST = 2;
    QRect contentRect = rect();

    if (m_pPageBar->isVisible()) {
        m_pItemArea->setSize(contentRect.width(),
                             contentRect.height() - m_pTitleArea->height() - m_pPageBar->height() + ADJUST *
                             2);
    } else {
        m_pItemArea->setSize(contentRect.width(),
                             contentRect.height() - m_pTitleArea->height() + ADJUST);
    }
}

QStringList QPageListWidget::getCurrentItems() const
{
    const int START_INDEX = m_pPageBar->currentIndex() * m_maxVisibleRows;

    if (START_INDEX < 0 || START_INDEX >= m_itemList.size()) {
        return QStringList();
    }

    const int END_INDEX = (m_itemList.size() > (START_INDEX + m_maxVisibleRows)) ?
                          (START_INDEX + m_maxVisibleRows) : m_itemList.size();
    QStringList retList;

    for (int i = START_INDEX; i < END_INDEX; i++) {
        retList.push_back(m_itemList.at(i));
    }

    return retList;
}

void QPageListWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    const int ADJUST = 2;
    m_pTitleArea->setFixedWidth(rect().width());
    m_pTitleArea->move(rect().x(), rect().y());
    m_pItemArea->setFixedWidth(rect().width() + ADJUST * 2);
    m_pItemArea->move(rect().x(), rect().y() + m_pTitleArea->height() - ADJUST);
    m_pPageBar->setFixedWidth(rect().width());
    m_pPageBar->move(rect().x(),
                     rect().bottomLeft().y() - m_pPageBar->height() + 1);
    adjustHeight();
}

void QPageListWidget::paintEvent(QPaintEvent *e)
{
    // subclass from QWidget need to provide a paintEvent for custom QWidget as below
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    QWidget::paintEvent(e);
}

void QPageListWidget::setItemAreaAlignment(Qt::Alignment align)
{
    m_pItemArea->setItemAlignment(align);
}

