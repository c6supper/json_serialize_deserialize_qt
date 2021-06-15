#ifndef QRESULTFILTERDIALOG_H_
#define QRESULTFILTERDIALOG_H_

#include <QDialog>
#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include "qbasecombobox.h"
#include "qprofile_global.h"

namespace QRserver
{
    class QResultFilterDialog : public QDialog
    {
            Q_OBJECT

        public:
            QResultFilterDialog(QWidget *parent = nullptr);
            ~QResultFilterDialog();

            void setSize(int width, int height);
            static QVariant Exec(QWidget *parent, const QStringList &jobIdList,
                                 const QStringList &locationList);

            static const QString FilterKeyAll;

            static void Initialize(QWidget *parent);

        private:
            bool initialize();
            QVariant exec(const QStringList &jobIdList, const QStringList &locationList);
            static QResultFilterDialog *instance(QWidget *parent = nullptr);

        protected:
            void resizeEvent(QResizeEvent *event);

        private:
            QLabel *m_labelTitle;
            QWidget *m_centerWidget;
            QBaseComboBox *m_comboBoxJobId;
            QBaseComboBox *m_comboBoxLocation;
            QWidget *m_pBtnWidget;
            static QResultFilterDialog *m_instance;
    };
}

#endif /* QRESULTFILTERDIALOG_H_ */
