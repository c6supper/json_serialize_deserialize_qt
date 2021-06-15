#include "qdetailinfobox.h"
#include "qrserverstrtable.h"
#include "quploadfiletablewidget.h"
#include <QResizeEvent>

using namespace QRserver;

QDetailInfoBox *QDetailInfoBox::m_instance = nullptr;

QDetailInfoBox *QDetailInfoBox::instance(QWidget *parent)
{
    if (m_instance == nullptr) {
        m_instance = new QDetailInfoBox(parent);
    }

    return m_instance;
}

void QDetailInfoBox::Initialize(QWidget *parent)
{
    if (m_instance == nullptr) {
        m_instance = new QDetailInfoBox(parent);
    }
}

QDetailInfoBox::QDetailInfoBox(QWidget *parent)
    : QDialog(parent)
{
    setObjectName("QDetailInfoBox");
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setWindowModality(Qt::ApplicationModal);
    initialize();
}

QDetailInfoBox::~QDetailInfoBox()
{
}

void QDetailInfoBox::setSize(int width, int height)
{
    QDialog::setFixedSize(width, height);
}

void QDetailInfoBox::Exec(QWidget *parent, const QStringList &pathList)
{
    instance(parent)->exec(pathList);
}

void QDetailInfoBox::resizeEvent(QResizeEvent *event)
{
    qprofileDebug(QtWarningMsg) << event;
    QDialog::resizeEvent(event);
    int columnWidth = width() / 2;
    int remainder = width() % 2;
    m_pPageTable->setColumnWidth(0, 0);
    m_pPageTable->setColumnWidth(QUploadFileTableWidget::ColumnIndexDisplayName,
                                 columnWidth);
    m_pPageTable->setColumnWidth(QUploadFileTableWidget::ColumnIndexTypeName,
                                 columnWidth + remainder);
}

bool QDetailInfoBox::initialize()
{
    QVBoxLayout *pDetaliLayout = new QVBoxLayout(this);
    pDetaliLayout->setMargin(2);
    pDetaliLayout->setSpacing(0);
    // Title
    m_pLabelTitle = new QLabel(TRANSLATE_STR_RSERVER_DOWNLOAD_LOCKEDLIST, this);
    m_pLabelTitle->setObjectName("QDetailInfoBoxLabelTitle");
    // Table widget
    m_pPageTable = new QDetailTableWidget(this);
    m_pPageTable->setObjectName("QDetailInfoBoxPageTable");
    m_pPageTable->setHorizontalHeaderLabels(QStringList() << ""
                                            << TRANSLATE_STR_RSERVER_UPLOAD_NAME
                                            << TRANSLATE_STR_RSERVER_UPLOAD_TYPE);
    m_pPageTable->setFilterKeyColumn(
        QUploadFileTableWidget::ColumnIndexDisplayName);
    m_pPageTable->setShowGrid(true);
    m_pPageTable->setMaxVisibleRows(5);
    // Buttons widget
    m_pBtnWidget = new QWidget(this);
    m_pBtnWidget->setObjectName("QDetailInfoBoxBtnWidget");
    QHBoxLayout *pBtnLayout = new QHBoxLayout(m_pBtnWidget);
    pBtnLayout->setMargin(0);
    pBtnLayout->setSpacing(10);
    m_pBtnOk = new QPushButton(TRANSLATE_STR_RSERVER_BTN_OK, this);
    connect(m_pBtnOk, SIGNAL(clicked(bool)), this, SLOT(accept()));
    pBtnLayout->addWidget(m_pBtnOk);
    pDetaliLayout->addWidget(m_pLabelTitle);
    pDetaliLayout->addWidget(m_pPageTable);
    pDetaliLayout->addWidget(m_pBtnWidget);
    return true;
}

void QDetailInfoBox::exec(const QStringList &pathList)
{
    m_pPageTable->clear();
    m_pPageTable->addItems(pathList);
    QDialog::exec();
}
