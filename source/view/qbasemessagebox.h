#ifndef QBASEMESSAGEBOX_H_
#define QBASEMESSAGEBOX_H_

#include <QDialog>
#include <QPushButton>
#include <QWidget>
#include <QLabel>
#include <QMap>
#include <QMessageBox>
#include <QButtonGroup>

namespace QRserver
{

    class QBaseMessageBox : public QDialog
    {
            Q_OBJECT

        public:
            QBaseMessageBox(QWidget *parent = 0);
            ~QBaseMessageBox();
            QMessageBox::StandardButton information(QWidget *parent, const QString &title,
                                                    const QString &text, QMessageBox::StandardButtons buttons = QMessageBox::Ok,
                                                    Qt::Alignment alignment = Qt::AlignCenter);
            QMessageBox::StandardButton show(QWidget *parent, const QString &title,
                                             const QString &text, QMessageBox::StandardButtons buttons = QMessageBox::Ok,
                                             Qt::Alignment alignment = Qt::AlignCenter);
            void setTitle(const QString &title);
            void setText(const QString &text);
            void setInformativeText(const QString &text);
            void setSize(int width, int height);
            void setAlignment(Qt::Alignment alignment);
            void setButtonText(QMessageBox::StandardButton standBtn, const QString &text);
            void resetButtons(QMessageBox::StandardButtons buttons);
            void resetInformation(const QString &title, const QString &text,
                                  QMessageBox::StandardButtons buttons = QMessageBox::Ok);

        signals:
            void buttonClicked(int id);

        private slots:
            void onButtonClick(int id);

        private:
            bool initialize();
            void setButtonStyle(QPushButton *pBtn, const QString &text);

        protected:
            void resizeEvent(QResizeEvent *event);

        private:
            QLabel *m_pLabelTitle;
            QLabel *m_pLabelText;
            QLabel *m_pLabelInformative;
            QButtonGroup *m_pBtnGroup;
            QList<QPushButton *> m_btnList;
            QMessageBox::StandardButton m_button;
            QWidget *m_pBtnWidget;
            QColor m_titleBgColor;
            QMap<QMessageBox::StandardButton, QString> m_btnTextMap;
    };
}
#endif /* QBASEMESSAGEBOX_H_ */
