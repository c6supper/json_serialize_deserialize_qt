#include "quploadwidget.h"
#include "qrserverstrtable.h"
#include "qrserverdevicetype.h"
#include "qrservermodel.h"
#include "qmodelfactory.h"
#include "qvconnectmodel.h"

using namespace QRserver;

QUploadWidget::QUploadWidget(QWidget *parent, const QString &tagName)
    : QRserverContainerWidget(parent)
{
    setTagName(tagName);
    setFixedSize(parent->width(), parent->height() - TAB_ROW_HEIGHT);
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);
    m_ptab = new QTabWidget(this);
    m_ptab->setObjectName("QUploadQTab");
    m_ptab->setFixedSize(width(), height());
    m_uploadProfile = new QUploadProfileWidget(m_ptab, "quploadprofile");
    m_uploadResult = new QUploadResultWidget(m_ptab, "quploadresult");
    m_ptab->addTab(m_uploadProfile, TRANSLATE_STR_RSERVER_TAB_PROFILE);
    m_ptab->addTab(m_uploadResult, TRANSLATE_STR_RSERVER_TAB_RESULT);
    layout->addWidget(m_ptab);
    layout->addStretch();
    connect(m_ptab, SIGNAL(currentChanged(int)), this,
            SIGNAL(currentTabChanged(int)));
    connect(this, SIGNAL(onUploadBtnClicked()), m_uploadProfile, SLOT(onUpload()));
    connect(this, SIGNAL(onUploadBtnClicked()), m_uploadResult, SLOT(onUpload()));
    connect(this, SIGNAL(onUploadAndDelBtnClicked()), m_uploadResult,
            SLOT(onUploadAndDel()));
    connect(this, SIGNAL(onJobBtnClicked()), m_uploadResult,
            SLOT(onJob()));
    connect(this, SIGNAL(checkDeviceSucceed()), m_uploadResult,
            SLOT(onGetFileList()));
    connect(this, SIGNAL(getProfileInfoList()), m_uploadProfile,
            SLOT(onGetProfileInfoList()));
}

void QUploadWidget::allocModel()
{
    const QRserverModel *rserverModel = QModelFactory::constModel<QRserverModel>
                                        (this);
    connect(rserverModel, SIGNAL(propertyChanged()), this,
            SLOT(onRserverModelChanged()));
}

const qint32 QUploadWidget::currentTabIndex()
{
    return m_ptab->currentIndex();
}

void QUploadWidget::translate(const QFont &font)
{
    m_ptab->setTabText(m_ptab->indexOf(m_uploadResult),
                       TRANSLATE_STR_RSERVER_TAB_RESULT);
    m_uploadResult->translate(font);

    if (m_uploadProfile != nullptr) {
        m_ptab->setTabText(m_ptab->indexOf(m_uploadProfile),
                           TRANSLATE_STR_RSERVER_TAB_PROFILE);
        m_uploadProfile->translate(font);
    }

    //
    m_ptab->setFont(font);
}

void QUploadWidget::onRserverModelChanged()
{
    const QRserverModel *rserverModel = QModelFactory::constModel<QRserverModel>
                                        (this);

    if (rserverModel->getChassis().isValid()) {
        if (!rserverModel->supportProfile()) {
            m_ptab->removeTab(m_ptab->indexOf(m_uploadProfile));
            delete m_uploadProfile;
            m_uploadProfile = nullptr;
        }

        m_ptab->setStyleSheet(QString("QTabBar::tab{width:%1px;}").arg(
                                  m_ptab->width() / m_ptab->count()));
        disconnect(rserverModel, SIGNAL(propertyChanged()), this,
                   SLOT(onRserverModelChanged()));
    }
}

void QUploadWidget::onSetTab(bool lock)
{
    if (m_uploadProfile != nullptr) {
        m_ptab->blockSignals(
            true); //in case emitting signal currentChanged which invokes onUploadWidgetTabChanged in rserverwidget during initialization
        m_ptab->setTabEnabled(m_ptab->indexOf(m_uploadProfile), !lock);
        m_ptab->blockSignals(false);
    }
}

QUploadWidget::~QUploadWidget()
{
}

