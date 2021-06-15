#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMap>
#include <QPushButton>
#include <QFlags>
#include <QResizeEvent>
#include <QApplication>
#include "qresultfilterdialog.h"
#include "qfilemodel.h"
#include "qrserverstrtable.h"

using namespace QRserver;

const QString QResultFilterDialog::FilterKeyAll = "All";

QResultFilterDialog *QResultFilterDialog::m_instance = nullptr;

QResultFilterDialog *QResultFilterDialog::instance(QWidget *parent)
{
    if (m_instance == nullptr) {
        m_instance = new QResultFilterDialog(parent);
    }

    return m_instance;
}

void QResultFilterDialog::Initialize(QWidget *parent)
{
    if (m_instance == nullptr) {
        m_instance = new QResultFilterDialog(parent);
    }
}

QVariant QResultFilterDialog::Exec(QWidget *parent,
                                   const QStringList &jobIdList,
                                   const QStringList &locationList)
{
    return instance(parent)->exec(jobIdList, locationList);
}

QResultFilterDialog::QResultFilterDialog(QWidget *parent)
    : QDialog(parent)
{
    setObjectName("QResultFilterDialog");
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setWindowModality(Qt::ApplicationModal);
    initialize();
}

QResultFilterDialog::~QResultFilterDialog()
{
}

bool QResultFilterDialog::initialize()
{
    // Title
    m_labelTitle = new QLabel(TRANSLATE_STR_RSERVER_UPLOAD_FILTER, this);
    m_labelTitle->setObjectName("QResultFilterDialogLabelTitle");
    //center widget
    m_centerWidget = new QWidget(this);
    m_centerWidget->setObjectName("QResultFilterDialogCenterWidget");
    QVBoxLayout *l_centerLayout = new QVBoxLayout(m_centerWidget);
    l_centerLayout->setMargin(10);
    l_centerLayout->setSpacing(10);
    QLabel *l_jobId = new QLabel(TRANSLATE_STR_RSERVER_UPLOAD_FILTER_JOBID, this);
    l_jobId->setObjectName("QResultFilterDialogLabelJobId");
    m_comboBoxJobId = new QBaseComboBox(this);
    m_comboBoxJobId->setTitle(TRANSLATE_STR_RSERVER_UPLOAD_FILTER_JOBID);
    QLabel *l_location = new QLabel(TRANSLATE_STR_RSERVER_UPLOAD_FILTER_LOCATION,
                                    this);
    l_location->setObjectName("QResultFilterDialogLabelLocation");
    m_comboBoxLocation = new QBaseComboBox(this);
    m_comboBoxLocation->setTitle(TRANSLATE_STR_RSERVER_UPLOAD_FILTER_LOCATION);
    l_centerLayout->addWidget(l_jobId);
    l_centerLayout->addWidget(m_comboBoxJobId);
    l_centerLayout->addWidget(l_location);
    l_centerLayout->addWidget(m_comboBoxLocation);
    // Buttons widget
    m_pBtnWidget = new QWidget(this);
    m_pBtnWidget->setObjectName("QResultFilterDialogBtnWidget");
    QHBoxLayout *pBtnLayout = new QHBoxLayout(m_pBtnWidget);
    pBtnLayout->setMargin(0);
    pBtnLayout->setSpacing(10);
    QPushButton *l_btnOK = new QPushButton(TRANSLATE_STR_RSERVER_BTN_OK, this);
    connect(l_btnOK, SIGNAL(clicked(bool)), this, SLOT(accept()));
    pBtnLayout->addWidget(l_btnOK);
    return true;
}

void QResultFilterDialog::resizeEvent(QResizeEvent *event)
{
    QDialog::resizeEvent(event);
    const int BORDER_WIDTH = 2;
    const int CONTENT_WIDTH = event->size().width() - BORDER_WIDTH * 2;
    const int CONTENT_HEIGHT = event->size().height() - BORDER_WIDTH * 2;
    // Adjust title label position
    m_labelTitle->setFixedWidth(CONTENT_WIDTH);
    m_labelTitle->move(BORDER_WIDTH, BORDER_WIDTH);
    //Adjust content position
    m_centerWidget->setFixedSize(CONTENT_WIDTH,
                                 CONTENT_HEIGHT - m_labelTitle->height() - m_pBtnWidget->height());
    m_centerWidget->move(BORDER_WIDTH, BORDER_WIDTH + m_labelTitle->height());
    // Adjust buttons position
    m_pBtnWidget->setFixedWidth(CONTENT_WIDTH);
    m_pBtnWidget->move(BORDER_WIDTH,
                       rect().bottomLeft().y() - BORDER_WIDTH - m_pBtnWidget->height() + 1);
}

void QResultFilterDialog::setSize(int width, int height)
{
    QDialog::setFixedSize(width, height);
}

QVariant QResultFilterDialog::exec(const QStringList &jobIdList,
                                   const QStringList &locationList)
{
    //jobId
    QStringList l_jobIdList = jobIdList;
    l_jobIdList.prepend(FilterKeyAll);
    QString l_curJobId = m_comboBoxJobId->currentText();
    //combobox: addItems will clear old data. clear() has a bug.
    m_comboBoxJobId->addItems(l_jobIdList);
    int l_jobIdIdx = m_comboBoxJobId->findText(l_curJobId,
                     Qt::MatchFixedString | Qt::MatchCaseSensitive);
    m_comboBoxJobId->setCurrentIndex(l_jobIdIdx > 0 ? l_jobIdIdx : 0);
    //location
    QStringList l_locationList = locationList;
    l_locationList.prepend(FilterKeyAll);
    QString l_curLocation = m_comboBoxLocation->currentText();
    m_comboBoxLocation->addItems(l_locationList);
    int l_locationIdx = m_comboBoxLocation->findText(l_curLocation,
                        Qt::MatchFixedString | Qt::MatchCaseSensitive);
    m_comboBoxLocation->setCurrentIndex(l_locationIdx > 0 ? l_locationIdx : 0);
    //
    QPoint cenPos = QRect(0, 60, 799, 400).center();
    this->move(cenPos.x() - this->width() / 2 + 4,
               cenPos.y() - this->height() / 2);
    QDialog::exec();
    //get selection
    QVariantMap map;
    l_curJobId = m_comboBoxJobId->currentText();
    l_curJobId.replace(FilterKeyAll, "");
    map.insert(QFileModel::PropertyJobId, QVariant(l_curJobId));
    l_curLocation = m_comboBoxLocation->currentText();
    l_curLocation.replace(FilterKeyAll, "");
    map.insert(QFileModel::PropertyLocation, QVariant(l_curLocation));
    return QVariant(map);
}

