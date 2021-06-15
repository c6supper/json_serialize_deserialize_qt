#ifndef QDETAILINFOBOX_H_
#define QDETAILINFOBOX_H_

#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QHeaderView>
#include "qdetailtablewidget.h"
#include "qprofile_debug.h"
#include "qprofile_global.h"

namespace QRserver
{

    class QDetailInfoBox : public QDialog
    {
            Q_OBJECT

        public:
            QDetailInfoBox(QWidget *parent = 0);
            ~QDetailInfoBox();
            void setSize(int width, int height);

            static void Exec(QWidget *parent, const QStringList &pathList);
            static void Initialize(QWidget *parent);

        protected:
            void resizeEvent(QResizeEvent *event);

        private:
            bool initialize();
            void exec(const QStringList &pathList);

            static QDetailInfoBox *instance(QWidget *parent = nullptr);

        private:
            QLabel *m_pLabelTitle;
            QDetailTableWidget *m_pPageTable;
            QWidget *m_pBtnWidget;
            QPushButton *m_pBtnOk;
            static QDetailInfoBox *m_instance;
    };
};

#endif //QDETAILINFOBOX_H_
