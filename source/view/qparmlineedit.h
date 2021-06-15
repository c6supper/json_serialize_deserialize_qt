#ifndef QPARMLINEEDIT_H
#define QPARMLINEEDIT_H

#include <QWidget>
#include <QLabel>
#include "qbaselineedit.h"

#if defined(BUILD_X86)
#else
#include <QResizeEvent>
// TODO:
#endif

class QParmLineEdit : public QWidget
{
        Q_OBJECT

    public:
        QParmLineEdit(const QString &name, const QString &value, QWidget *parent = 0);
        ~QParmLineEdit();

        void setName(const QString &name);
        void setWidth(int width);
        void setEnable(bool b);
        void setReadOnly(bool b);
        QString text() const;
        void setFont(const QFont &);
        //        void setTextColor(const QColor &color, FieldFlag flag = FieldName);
        void setMaxLength(int length);

    signals:
        void textChanged(const QString &text);
        void textEdited(const QString &text);
        void editingFinished();

    public slots:
        void setText(const QString &value);

    protected slots:
        /******************************
         * Function: notifyChanged()
         * Description: to notify parent widget to deal with changes.
         * Input: void
         * Output: void
         * Return: void
         ******************************/
        void notifyChanged();

    protected:
        void resizeEvent(QResizeEvent *event);

    private:
        QLabel *m_pName;
        QBaseLineEdit *m_pValue;
};

#endif // QPARMLINEEDIT_H
