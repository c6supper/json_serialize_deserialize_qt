#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFlags>
#include <QResizeEvent>
#include <QApplication>
#include "qbasemessagebox.h"
#include "qprofile_debug.h"
#include "qprofile_global.h"

using namespace QRserver;

QBaseMessageBox::QBaseMessageBox(QWidget *parent)
    : QDialog(parent)
{
    setObjectName("QBaseMessageBox");
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setWindowModality(Qt::ApplicationModal);
    initialize();
}

QBaseMessageBox::~QBaseMessageBox()
{
}

bool QBaseMessageBox::initialize()
{
    // Title
    m_pLabelTitle = new QLabel(this);
    m_pLabelTitle->setObjectName("QBaseMessageBoxLabelTitle");
    // Text
    m_pLabelText = new QLabel(this);
    m_pLabelText->setObjectName("QBaseMessageBoxLabelText");
    // Informative
    m_pLabelInformative = new QLabel(this);
    m_pLabelInformative->setObjectName("QBaseMessageBoxLabelInformative");
    // Buttons widget
    m_pBtnWidget = new QWidget(this);
    m_pBtnWidget->setObjectName("QBaseMessageBoxBtnWidget");
    // Button group
    m_pBtnGroup = new QButtonGroup(this);
    connect(m_pBtnGroup, SIGNAL(buttonClicked(int)), SLOT(onButtonClick(int)));
    return true;
}

void QBaseMessageBox::resizeEvent(QResizeEvent *event)
{
    QDialog::resizeEvent(event);
    const int BORDER_WIDTH = 2;
    const int CONTENT_WIDTH = event->size().width() - BORDER_WIDTH * 2;
    const int CONTENT_HEIGHT = event->size().height() - BORDER_WIDTH * 2;
    // Adjust title label position
    m_pLabelTitle->setFixedWidth(CONTENT_WIDTH);
    m_pLabelTitle->move(BORDER_WIDTH, BORDER_WIDTH);

    // Adjust message label position
    if (m_pLabelInformative->text().trimmed().isEmpty()) {
        m_pLabelInformative->hide();
        m_pLabelText->setFixedSize(CONTENT_WIDTH,
                                   CONTENT_HEIGHT - m_pLabelTitle->height() - m_pBtnWidget->height());
        m_pLabelText->move(BORDER_WIDTH, BORDER_WIDTH + m_pLabelTitle->height());
    } else {
        m_pLabelInformative->show();
        m_pLabelText->setFixedSize(CONTENT_WIDTH,
                                   CONTENT_HEIGHT - m_pLabelTitle->height() - m_pLabelInformative->height() -
                                   m_pBtnWidget->height());
        m_pLabelText->move(BORDER_WIDTH, BORDER_WIDTH + m_pLabelTitle->height());
        m_pLabelInformative->setFixedWidth(CONTENT_WIDTH);
        m_pLabelInformative->move(BORDER_WIDTH,
                                  BORDER_WIDTH + m_pLabelTitle->height() + m_pLabelText->height());
    }

    // Adjust buttons position
    m_pBtnWidget->setFixedWidth(CONTENT_WIDTH);
    m_pBtnWidget->move(BORDER_WIDTH,
                       rect().bottomLeft().y() - BORDER_WIDTH - m_pBtnWidget->height() + 1);
}

void QBaseMessageBox::setTitle(const QString &title)
{
    m_pLabelTitle->setText(title);
}

void QBaseMessageBox::setText(const QString &text)
{
    m_pLabelText->setText(text);
    setFixedSize(width(), height());
}

void QBaseMessageBox::setInformativeText(const QString &text)
{
    m_pLabelInformative->setText(text);
    setFixedSize(width(), height());
}

void QBaseMessageBox::setAlignment(Qt::Alignment alignment)
{
    m_pLabelText->setAlignment(alignment);
}

void QBaseMessageBox::setButtonText(QMessageBox::StandardButton standBtn,
                                    const QString &text)
{
    m_btnTextMap.insert(standBtn, text);
}

void QBaseMessageBox::setSize(int width, int height)
{
    QDialog::setFixedSize(width, height);
}

void QBaseMessageBox::resetInformation(const QString &title,
                                       const QString &text, QMessageBox::StandardButtons buttons)
{
    m_pLabelTitle->setText(title);
    m_pLabelText->setText(text);
    QFont ft = qApp->font();
    m_pLabelTitle->setFont(ft);
    m_pLabelText->setFont(ft);
    resetButtons(buttons);
}

QMessageBox::StandardButton QBaseMessageBox::information(QWidget *parent,
        const QString &title, const QString &text, QMessageBox::StandardButtons buttons,
        Qt::Alignment alignment)
{
    if (parent == nullptr) {
        qprofileDebug(QtFatalMsg) << "parent = nullptr";
    }

    resetInformation(title, text, buttons);
    setAlignment(alignment);
    move(parent->window()->frameGeometry().topLeft() +
         parent->window()->rect().center() - rect().center());
    QDialog::exec();
    return m_button;
}

QMessageBox::StandardButton QBaseMessageBox::show(QWidget *parent,
        const QString &title, const QString &text, QMessageBox::StandardButtons buttons,
        Qt::Alignment alignment)
{
    if (parent == nullptr) {
        qprofileDebug(QtFatalMsg) << "parent = nullptr";
    }

    resetInformation(title, text, buttons);
    setAlignment(alignment);
    move(parent->window()->frameGeometry().topLeft() +
         parent->window()->rect().center() - rect().center());
    QDialog::show();
    return m_button;
}

void QBaseMessageBox::resetButtons(QMessageBox::StandardButtons buttons)
{
    delete m_pBtnWidget->layout();
    QHBoxLayout *pBtnLayout = new QHBoxLayout(m_pBtnWidget);
    pBtnLayout->setMargin(0);
    pBtnLayout->setSpacing(10);

    for (int i = 0; i < m_btnList.size(); i++) {
        disconnect(m_btnList.at(i), 0, 0, 0);
        m_pBtnGroup->removeButton(m_btnList.at(i));
    }

    qDeleteAll(m_btnList);
    m_btnList.clear();

    if (buttons.testFlag(QMessageBox::NoButton)) {
        return;
    }

    // OK
    QPushButton *pBtn = NULL;

    if (buttons.testFlag(QMessageBox::Ok)) {
        pBtn = new QPushButton(this);
        m_pBtnGroup->addButton(pBtn, QMessageBox::Ok);
        pBtnLayout->addWidget(pBtn);

        if (!m_btnTextMap.contains(QMessageBox::Ok))
            setButtonStyle(pBtn, tr("OK"));
        else
            setButtonStyle(pBtn, m_btnTextMap.value(QMessageBox::Ok));
    }

    // Yes
    if (buttons.testFlag(QMessageBox::Yes)) {
        pBtn = new QPushButton(this);
        m_pBtnGroup->addButton(pBtn, QMessageBox::Yes);
        pBtnLayout->addWidget(pBtn);

        if (!m_btnTextMap.contains(QMessageBox::Yes))
            setButtonStyle(pBtn, tr("YES"));
        else
            setButtonStyle(pBtn, m_btnTextMap.value(QMessageBox::Yes));
    }

    // Cancel
    if (buttons.testFlag(QMessageBox::Cancel)) {
        pBtn = new QPushButton(this);
        m_pBtnGroup->addButton(pBtn, QMessageBox::Cancel);
        pBtnLayout->addWidget(pBtn);

        if (!m_btnTextMap.contains(QMessageBox::Cancel))
            setButtonStyle(pBtn, tr("Cancel"));
        else
            setButtonStyle(pBtn, m_btnTextMap.value(QMessageBox::Cancel));
    }

    // No
    if (buttons.testFlag(QMessageBox::No)) {
        pBtn = new QPushButton(this);
        m_pBtnGroup->addButton(pBtn, QMessageBox::No);
        pBtnLayout->addWidget(pBtn);

        if (!m_btnTextMap.contains(QMessageBox::No))
            setButtonStyle(pBtn, tr("NO"));
        else
            setButtonStyle(pBtn, m_btnTextMap.value(QMessageBox::No));
    }

    // Close
    if (buttons.testFlag(QMessageBox::Close)) {
        pBtn = new QPushButton(this);
        m_pBtnGroup->addButton(pBtn, QMessageBox::Close);
        pBtnLayout->addWidget(pBtn);

        if (!m_btnTextMap.contains(QMessageBox::Close))
            setButtonStyle(pBtn, tr("Close"));
        else
            setButtonStyle(pBtn, m_btnTextMap.value(QMessageBox::Close));
    }
}

void QBaseMessageBox::setButtonStyle(QPushButton *pBtn, const QString &text)
{
    Q_ASSERT(pBtn != NULL);
    m_btnList.push_back(pBtn);
    pBtn->setText(text);
    QFont ft = qApp->font();
    pBtn->setFont(ft);
}

void QBaseMessageBox::onButtonClick(int id)
{
    m_button = (QMessageBox::StandardButton)id;

    switch (id) {
    case QMessageBox::Ok:
        QDialog::accept();
        break;

    case QMessageBox::Yes:
        QDialog::accept();
        break;

    case QMessageBox::Cancel:
        QDialog::reject();
        break;

    case QMessageBox::No:
        QDialog::reject();
        break;

    case QMessageBox::Close:
        QDialog::reject();
        break;

    default:
        QDialog::accept();
        break;
    }

    emit buttonClicked(id);
}
