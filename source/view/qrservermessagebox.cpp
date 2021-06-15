#include "qrservermessagebox.h"
#include "qrserverstrtable.h"

using namespace QRserver;

QRserverMessageBox *QRserverMessageBox::m_instance = nullptr;

QRserverMessageBox::QRserverMessageBox(QWidget *parent)
    : QBaseMessageBox(parent)
{
    setButtonText(QMessageBox::Ok, TRANSLATE_STR_RSERVER_BTN_OK);
}

QRserverMessageBox *QRserverMessageBox::instance(QWidget *parent)
{
    if (m_instance == nullptr) {
        m_instance = new QRserverMessageBox(parent);
    }

    return m_instance;
}

void QRserverMessageBox::Initialize(QWidget *parent)
{
    if (m_instance == nullptr) {
        m_instance = new QRserverMessageBox(parent);
    }
}

void QRserverMessageBox::Accept()
{
    instance()->accept();
}

QMessageBox::StandardButton QRserverMessageBox::Information(QWidget *parent,
        const QString &title, const QString &text, QMessageBox::StandardButtons buttons,
        Qt::Alignment alignment)
{
    if (parent != nullptr) {
        if (!parent->isVisible())
            return QMessageBox::NoButton;
    }

    return instance(parent)->information(parent, title, text, buttons, alignment);
}

QMessageBox::StandardButton QRserverMessageBox::Show(QWidget *parent,
        const QString &title, const QString &text, QMessageBox::StandardButtons buttons,
        Qt::Alignment alignment)
{
    if (parent != nullptr) {
        if (!parent->isVisible())
            return QMessageBox::NoButton;
    }

    if (instance(parent)->isVisible()) {
        instance(parent)->accept();
    }

    return instance(parent)->show(parent, title, text, buttons, alignment);
}

QRserverMessageBox::~QRserverMessageBox()
{
}

