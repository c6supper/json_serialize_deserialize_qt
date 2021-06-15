#include "qbaselineedit.h"

#if !defined(BUILD_X86)
class rserverPlugin
{
    public:
        static QString accessKeyPad(const QString &title, const QString &text, int type,
                                    QWidget *parent = 0);
        static void closeKeyPad();
};
#endif

QBaseLineEdit::QBaseLineEdit(QWidget *parent)
    : QLineEdit(parent),
      bIsSKPVisible(false)
{
    setObjectName("QBaseLineEdit");
    setFocusPolicy(Qt::NoFocus);
}

QBaseLineEdit::~QBaseLineEdit()
{
}

void QBaseLineEdit::setTitle(const QString &title)
{
    m_title = title;
}

void QBaseLineEdit::mousePressEvent(QMouseEvent *event)
{
    QLineEdit::mousePressEvent(event);
    bIsSKPVisible = true;
    QString orgStr = text();
    QString retStr;
#if defined(BUILD_X86)
    retStr = CKeyPadFactory::accessKeyPad(m_title, text(),
                                          CKeyPadFactory::GenKeyPad, NULL);
#else
    retStr = rserverPlugin::accessKeyPad(m_title, text(), GenKeyPad, this);
#endif

    if (isEnabled())
        setText(retStr);

    bIsSKPVisible = false;

    if (orgStr != retStr) {
        emit textEdited(text());
    }
}

void QBaseLineEdit::changeEvent(QEvent *event)
{
    QLineEdit::changeEvent(event);

    if (event->type() == QEvent::EnabledChange) {
        if (!isEnabled() && bIsSKPVisible) {
#if defined(BUILD_X86)
            CKeyPadFactory::closeKeyPad();
#else
            rserverPlugin::closeKeyPad();
#endif
            bIsSKPVisible = false;
        }
    } else if (event->type() == QEvent::Hide) {
        if (bIsSKPVisible) {
#if defined(BUILD_X86)
            CKeyPadFactory::closeKeyPad();
#else
            rserverPlugin::closeKeyPad();
#endif
            bIsSKPVisible = false;
        }
    }
}

