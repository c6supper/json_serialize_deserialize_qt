#ifndef QRSERVERMESSAGEBOX_H_
#define QRSERVERMESSAGEBOX_H_

#include "qbasemessagebox.h"
#include "qprofile_global.h"

namespace QRserver
{
    class QRserverMessageBox : public QBaseMessageBox
    {
            Q_OBJECT

        public:
            QRserverMessageBox(QWidget *parent = nullptr);
            ~QRserverMessageBox();
            static QMessageBox::StandardButton Information(QWidget *parent,
                    const QString &title, const QString &text,
                    QMessageBox::StandardButtons buttons = QMessageBox::Ok,
                    Qt::Alignment alignment = Qt::AlignCenter);
            static QMessageBox::StandardButton Show(QWidget *parent, const QString &title,
                                                    const QString &text, QMessageBox::StandardButtons buttons = QMessageBox::Ok,
                                                    Qt::Alignment alignment = Qt::AlignCenter);
            static void Accept();
            static void Initialize(QWidget *parent);

        private:
            static QRserverMessageBox *instance(QWidget *parent = nullptr);

        private:
            static QRserverMessageBox		*m_instance;
    };
};

#endif // QRSERVERMESSAGEBOX_H_
