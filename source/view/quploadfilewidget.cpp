#include "quploadfilewidget.h"
#include "qgenericcommand.h"
#include "quploadprofilemodel.h"
#include "qrservermessagebox.h"
#include "qrserverstrtable.h"
#include "quploadstatus.h"

using namespace QRserver;

QUploadFileWidget::QUploadFileWidget(QWidget *parent,
                                     const QString &tagName)
    : QRserverContainerWidget(parent)
{
    setObjectName("QUploadFileWidget");
    setTagName(tagName);
    setFixedSize(parent->width(), parent->height() - TAB_ROW_HEIGHT);
    m_tableWidget = new QUploadFileTableWidget(this);
    m_tableWidget->setHorizontalHeaderLabels(QStringList() << "" <<
            TRANSLATE_STR_RSERVER_UPLOAD_NAME << TRANSLATE_STR_RSERVER_UPLOAD_TYPE <<
            TRANSLATE_STR_RSERVER_UPLOAD_DATE << TRANSLATE_STR_RSERVER_UPLOAD_TYPEVALUE <<
            TRANSLATE_STR_RSERVER_UPLOAD_FULLPATH << TRANSLATE_STR_RSERVER_UPLOAD_JOBID <<
            TRANSLATE_STR_RSERVER_UPLOAD_LOCATION);
    m_tableWidget->setShowGrid(false);
    m_tableWidget->setFilterKeyColumn(
        QUploadFileTableWidget::ColumnIndexDisplayName);
    m_tableWidget->setColumnHidden(QUploadFileTableWidget::ColumnIndexFileType,
                                   true);
    m_tableWidget->setColumnHidden(QUploadFileTableWidget::ColumnIndexPathName,
                                   true);
    m_tableWidget->setColumnHidden(QUploadFileTableWidget::ColumnIndexJobId, true);
    m_tableWidget->setColumnHidden(QUploadFileTableWidget::ColumnIndexLocation,
                                   true);
    connect(m_tableWidget, SIGNAL(invalidRowChecked()),
            this, SLOT(onInvalidRowChecked()));
    m_filterPattern = new QBaseLineEdit(this);
    m_filterPattern->setObjectName("UploadFileFilterPattern");
    m_filterPattern->setPlaceholderText(":Name");
    m_filterPattern->setTitle("Filter");
    connect(m_filterPattern, SIGNAL(textChanged(const QString &)),
            m_tableWidget, SLOT(onFilterPatternChanged(const QString &)));
    m_pBtnRefresh = new QPushButton(this);
    m_pBtnRefresh->setObjectName("UploadFileBtnRefresh");
    m_pBtnRefresh->setText(TRANSLATE_STR_RSERVER_BTN_REFRESH);
    connect(m_pBtnRefresh, SIGNAL(clicked(bool)), SLOT(onGetFileList()));
    QHBoxLayout *bottomLayout = new QHBoxLayout(this);
    bottomLayout->setContentsMargins(8, 2, 5, 8);
    bottomLayout->setSpacing(8);
    bottomLayout->addWidget(m_filterPattern);
    bottomLayout->addWidget(m_pBtnRefresh);
    bottomLayout->addStretch();
    m_bottomWidget  = new QWidget(this);
    m_bottomWidget->setLayout(bottomLayout);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addWidget(m_tableWidget);
    layout->addWidget(m_bottomWidget);
    layout->addStretch();
}

QUploadFileWidget::~QUploadFileWidget()
{
}

void QUploadFileWidget::setColumnFilterPattern(const int column,
        const QString &pattern)
{
    m_tableWidget->onColumnFilterPatternChanged(column, pattern);
}

void QUploadFileWidget::allocBackProgress()
{
}

void QUploadFileWidget::onInvalidRowChecked()
{
    QRserverMessageBox::Show(this, TRANSLATE_STR_RSERVER_WARNING,
                             TRANSLATE_STR_RSERVER_UPLOAD_FILENAME_LENGTH_TOOLONG.arg(
                                 QUploadFileTableWidget::FileNameLengthLimitation), QMessageBox::Ok);
}

void QUploadFileWidget::onGetFileList()
{
    m_tableWidget->clear();
    QGenericCommand genericCommand(this, QUploadFileModel::CommandGetFileList);
    QVariant statusVar = issueCommand(genericCommand.toVariant(),
                                      QUploadFileModel::GetFileListTimeout, "");

    if (!statusVar.isNull()) {
        QUploadStatus status(statusVar);

        switch (status.commandStatus()) {
        case QAbstractStatus::Succeed:
            break;

        default:
            QString message = QString("%1").arg(
                                  TRANSLATE_STR_RSERVER_UPLOAD_LOADFILES_FAILED.arg(status.commandStatus()));
            QRserverMessageBox::Show(this, TRANSLATE_STR_RSERVER_WARNING, message,
                                     QMessageBox::Ok);
            break;
        }
    }
}

const QVariantList QUploadFileWidget::fileModelList() const
{
    return m_tableWidget->getCheckedItems();
}

void QUploadFileWidget::setFileModelList(const QVariantList &fileModelList)
{
    m_tableWidget->clear();
    m_tableWidget->addItems(fileModelList);
}

QStringList QUploadFileWidget::getColumnValueList(const int column) const
{
    return m_tableWidget->getColumnValueList(column);
}

void QUploadFileWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    m_tableWidget->setSize(rect().width(),
                           rect().height() - m_bottomWidget->height() - 5);
    m_tableWidget->setColumnWidth(0, (int)(width() * 0.05));
    m_tableWidget->setColumnWidth(1, (int)(width() * 0.35));
    m_tableWidget->setColumnWidth(2, (int)(width() * 0.28));
    m_tableWidget->setColumnWidth(3, (int)(width() * 0.32 + 1));
    m_tableWidget->setMaxVisibleRows(m_tableWidget->height() / 30 - 1);
}

void QUploadFileWidget::translate(const QFont &font)
{
    m_tableWidget->setHorizontalHeaderLabels(QStringList() << "" <<
            TRANSLATE_STR_RSERVER_UPLOAD_NAME << TRANSLATE_STR_RSERVER_UPLOAD_TYPE <<
            TRANSLATE_STR_RSERVER_UPLOAD_DATE << TRANSLATE_STR_RSERVER_UPLOAD_TYPEVALUE <<
            TRANSLATE_STR_RSERVER_UPLOAD_FULLPATH << TRANSLATE_STR_RSERVER_UPLOAD_JOBID <<
            TRANSLATE_STR_RSERVER_UPLOAD_LOCATION);
    m_pBtnRefresh->setText(TRANSLATE_STR_RSERVER_BTN_REFRESH);
    //
    m_tableWidget->setFont(font);
    m_filterPattern->setFont(font);
    m_pBtnRefresh->setFont(font);
}

void QUploadFileWidget::showEvent(QShowEvent *event)
{
    QUploadFileModel *fileModel = qobject_cast<QUploadFileModel *>(model());

    if (!getBackProgress()->isActive() && fileModel->fileModelList().size() == 0) {
        QTimer::singleShot(0, this, SLOT(onGetFileList()));
    }

    QWidget::showEvent(event);
}

