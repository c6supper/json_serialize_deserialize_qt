#include "qparmlineedit.h"
#include <QHBoxLayout>
#include <QMetaMethod>

QParmLineEdit::QParmLineEdit(const QString &name, const QString &value,
                             QWidget *parent)
    : QWidget(parent)
{
    setObjectName("QParmLineEdit");
    QHBoxLayout *l_pLayout = new QHBoxLayout(this);
    l_pLayout->setContentsMargins(0, 0, 0, 0);
    l_pLayout->setSpacing(0);
    m_pName = new QLabel(this);
    m_pName->setObjectName("QParmLineEditName");
    m_pName->setText(name);
    l_pLayout->addWidget(m_pName);
    m_pValue = new QBaseLineEdit(this);
    m_pValue->setTitle(name);
    m_pValue->setText(value);
    l_pLayout->addWidget(m_pValue);
    connect(m_pValue, SIGNAL(textChanged(const QString &)),
            SIGNAL(textChanged(const QString &)));
    connect(m_pValue, SIGNAL(textEdited(const QString &)),
            SIGNAL(textEdited(const QString &)));
    connect(m_pValue, SIGNAL(editingFinished()), SIGNAL(editingFinished()));
    connect(m_pValue, SIGNAL(textEdited(const QString &)), SLOT(notifyChanged()));
}

QParmLineEdit::~QParmLineEdit()
{
}

void QParmLineEdit::setFont(const QFont &font)
{
    m_pName->setFont(font);
    m_pValue->setFont(font);
}

void QParmLineEdit::setMaxLength(int length)
{
    m_pValue->setMaxLength(length);
}

void QParmLineEdit::notifyChanged()
{
    if (parent() != NULL) {
        const QMetaObject *parentMeta = parent()->metaObject();
        int index = parentMeta->indexOfSignal("propertyChanged()");

        if (index >= 0) {
            QMetaMethod propertyChangedMethod = parentMeta->method(index);
            propertyChangedMethod.invoke(parent());
        }
    }
}

void QParmLineEdit::setName(const QString &name)
{
    m_pName->setText(name);
    m_pValue->setTitle(name);
}

QString QParmLineEdit::text() const
{
    return m_pValue->text();
}

void QParmLineEdit::setText(const QString &text)
{
    m_pValue->setText(text);
}

void QParmLineEdit::setWidth(int width)
{
    QWidget::setFixedWidth(width);
    m_pName->setFixedWidth(width / 2 + 1);
    m_pValue->setFixedWidth(width - m_pName->width());
}

void QParmLineEdit::setEnable(bool b)
{
    m_pValue->setEnabled(b);
}

void QParmLineEdit::setReadOnly(bool b)
{
    m_pValue->setReadOnly(b);
}

#if 0
void QParmLineEdit::setTextColor(const QColor &color, FieldFlag flag)
{
    switch (flag) {
    case FieldValue:
        m_pValue->setTextColor(color);
        break;

    case FieldAll:
        m_pName->setTextColor(color);
        m_pValue->setTextColor(color);
        break;

    case FieldName:
    default:
        m_pName->setTextColor(color);
        break;
    }
}
#endif

void QParmLineEdit::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    setWidth(event->size().width());
}

