#ifndef QBASELINEEDIT_H
#define QBASELINEEDIT_H

#if defined(BUILD_X86)
#include "keypadfactory.h"
#else
#include <QEvent>
#endif
#include <QLineEdit>

class QBaseLineEdit : public QLineEdit
{
        Q_OBJECT

#if !defined(BUILD_X86)
        enum KeyPadType {GenKeyPad, HexKeyPad, BinKeyPad, NumKeyPad, IPv4KeyPad, FileNameKeyPad, EndKeyPad};
#endif

    public:
        QBaseLineEdit(QWidget *parent = 0);
        ~QBaseLineEdit();
        void setTitle(const QString &title);

    protected:
        void mousePressEvent(QMouseEvent *event);
        void changeEvent(QEvent *event);

    private:
        bool bIsSKPVisible;	// true--soft keyboard visible, false--soft keyboard invisible
        QString m_title;
};

#endif // QBASELINEEDIT_H
