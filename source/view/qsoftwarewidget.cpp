#include "qsoftwarewidget.h"
#include <QApplication>
#include "qrserverstrtable.h"
#include <QHeaderView>
#include "qsoftwaremodel.h"
#include "qmodelfactory.h"
#include "qupgradecommand.h"
#include "qgenericcommand.h"
#include "qrservermessagebox.h"
#include "qrserverdevicetype.h"
#if defined(BUILD_X86)
#include "qjson/qobjecthelper.h"
#else
#include <QJson/QObjectHelper>
#endif
#include "qrservercompatibility605.h"

#if !defined(BUILD_X86)
class rserverPlugin
{
    public:
        static bool isTestAppRunning();
        static bool tryReleaseTestApp();
};
#endif

using namespace QRserver;

QSoftwareWidget::QSoftwareWidget(QWidget *parent, const QString &tagName)
    : QRserverContainerWidget(parent)
{
    setTagName(tagName);
    setFixedSize(parent->width(), parent->height() - TAB_ROW_HEIGHT);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);
    QStringList headerList = QStringList() << "" <<
                             TRANSLATE_STR_RSERVER_SOFTWARE_TYPE <<
                             TRANSLATE_STR_RSERVER_SOFTWARE_CURRENT_VERSION <<
                             TRANSLATE_STR_RSERVER_SOFTWARE_NEW_VERSION;
    int addColumnCount = QDeviceInfo::PropertyList.count() - headerList.count() + 1;

    for (int i = 0; i < addColumnCount; i++) {
        headerList.append("");
    }

    m_pSoftwareTableWidget = new QSoftwareTableWidget(this);
    m_pSoftwareTableWidget->setHorizontalHeaderLabels(headerList);
    m_pSoftwareTableWidget->setShowGrid(false);
    m_pSoftwareTableWidget->setSortingEnabled(false);
    layout->addWidget(m_pSoftwareTableWidget);
    layout->addStretch();
}

void QSoftwareWidget::allocBackProgress()
{
}

void QSoftwareWidget::allocModel()
{
    setModel(m_softwareModel = QModelFactory::construct<QSoftwareModel>(this));
    disconnect(m_softwareModel, SIGNAL(propertyChanged()), this,
               SLOT(onModelChanged()));
}

const QByteArray QSoftwareWidget::get(const QGenericMessage &message)
{
    const QVariant &content = message.content();
    QJson::QObjectHelper::qvariant2qobject(content.toMap(), this);
    return QAbstractContainerWidget::get(message);
}

bool QSoftwareWidget::checkUpgrade()
{
    QVariantList upgradeInfoList = m_softwareModel->upgradeInfoList();

    if (upgradeInfoList.count() <= 0) {
        qprofileDebug(QtWarningMsg) << "No upgrade information found in model.";
        return false;
    }

    QUpgradeInfo upgradeInfo;
    upgradeInfo.fromVariant(upgradeInfoList.first());

    switch (upgradeInfo.upgradeStatus()) {
    case QUpgradeInfo::VersionCantFind: {
        QRserverMessageBox::Information(this, TRANSLATE_STR_RSERVER_WARNING,
                                        TRANSLATE_STR_RSERVER_SOFTWARE_NO_AVAILABLE_VERSION, QMessageBox::Ok);
        return true;
    }
    break;

    case QUpgradeInfo::VersionIsNewest: {
        QRserverMessageBox::Information(this, TRANSLATE_STR_RSERVER_WARNING,
                                        TRANSLATE_STR_RSERVER_SOFTWARE_CURRENT_VERSION_LATEST, QMessageBox::Ok);
        return true;
    }
    break;

    case QUpgradeInfo::ModuleVersionEmpty: {
        QRserverMessageBox::Information(this, TRANSLATE_STR_RSERVER_WARNING,
                                        TRANSLATE_STR_RSERVER_SOFTWARE_MODULE_VERSIONI_EMPTY, QMessageBox::Ok);
        return true;
    }
    break;

    case QUpgradeInfo::ExceedCapOfPacSize: {
        QRserverMessageBox::Information(this, TRANSLATE_STR_RSERVER_WARNING,
                                        TRANSLATE_STR_RSERVER_SOFTWARE_EXCEED_CAP_OF_PAC_SIZE, QMessageBox::Ok);
        return true;
    }
    break;

    case QUpgradeInfo::CreatePathFailed:
    case QUpgradeInfo::SaveDirectoryNotExists:
    case QUpgradeInfo::Timeout:
    case QUpgradeInfo::MD5Mismatch:
    case QUpgradeInfo::SaveFileFailed:
    case QUpgradeInfo::StartFailed:
    case QUpgradeInfo::RemoteFileNotExists:
    case QUpgradeInfo::TooMuchConnection:
    case QUpgradeInfo::NotEnoughSpace:
    case QUpgradeInfo::UnknowError: {
        QRserverMessageBox::Information(this, TRANSLATE_STR_RSERVER_WARNING,
                                        TRANSLATE_STR_RSERVER_SOFTWARE_DOWNLOAD_FAILED_WITH_ERR_CODE.arg(
                                            upgradeInfo.upgradeStatus()), QMessageBox::Ok);
        return true;
    }
    break;

    case QUpgradeInfo::FineToUpgrade: {
        if (isCompatible(QRserverCompatibility605::FeatureUpgradeViaHttp)) {
            getBackProgress()->setText(QString(
                                           TRANSLATE_STR_RSERVER_SOFTWARE_DOWNLOAD_PROGRESS_WITHOUT_PERCENT_SIGN).arg(
                                           upgradeInfo.downloadedPercentage()));

            if (upgradeInfo.downloadedPercentage() != "100%") {
                getBackProgress()->restart(QSoftwareModel::PackageTimeout);
            }
        } else {
            static quint32 lastDownloadedSize = 0;
            quint32 imageSize = upgradeInfo.imageSize();
            quint32 downloadedSize = upgradeInfo.downloadedSize();
            float ratio = ((float)downloadedSize / imageSize) * 100;
            /*        qprofileDebug(QtDebugMsg) << "FineToUpgrade:" << downloadedSize << imageSize <<
                                              ratio;*/
            getBackProgress()->setText(QString(
                                           TRANSLATE_STR_RSERVER_SOFTWARE_DOWNLOAD_PROGRESS).arg(
                                           (quint32)ratio));

            if (lastDownloadedSize != downloadedSize) {
                getBackProgress()->restart(QSoftwareModel::PackageTimeout);
                lastDownloadedSize = downloadedSize;
            }
        }
    }
    break;

    case QUpgradeInfo::DownloadFileSuccess: {
        qprofileDebug(QtDebugMsg) << "DownloadFileSuccess.";
#if defined(BUILD_X86)
        QRserverMessageBox::Information(this, TRANSLATE_STR_RSERVER_INFORMATION,
                                        TRANSLATE_STR_RSERVER_SOFTWARE_DOWNLOAD_IMAGE_SUCCESS, QMessageBox::Ok);
#else
        QRserverMessageBox::Show(this, TRANSLATE_STR_RSERVER_INFORMATION,
                                 TRANSLATE_STR_RSERVER_SOFTWARE_DOWNLOAD_IMAGE_SUCCESS, QMessageBox::NoButton);
        QApplication::quit();
#endif
        return true;
    }
    break;

    default: {
        qprofileDebug(QtWarningMsg) << "Unknown upgradeStatus:" <<
                                    upgradeInfo.upgradeStatus();
    }
    break;
    }

    return false;
}

bool QSoftwareWidget::isTestAppRunning()
{
#if defined(BUILD_X86)
    return false;
#else
    return rserverPlugin::isTestAppRunning();
#endif
}

void QSoftwareWidget::onUpgrade()
{
    if (deviceList().count() <= 0) {
        QRserverMessageBox::Information(this, TRANSLATE_STR_RSERVER_WARNING,
                                        TRANSLATE_STR_RSERVER_SOFTWARE_UPGRADE_TO, QMessageBox::Ok);
        return;
    }

#if !defined(BUILD_X86)

    if (isTestAppRunning()) {
        const QRserverModel &rserverModel = m_softwareModel->rserverModel();

        if (!rserverModel.canReleaseAppManually()) {
            if (rserverPlugin::tryReleaseTestApp() != true) {
                QRserverMessageBox::Information(this, TRANSLATE_STR_RSERVER_WARNING,
                                                TRANSLATE_STR_RSERVER_SOFTWARE_RELEASE_APP_FAILED, QMessageBox::Ok);
                return;
            }
        } else {
            QRserverMessageBox::Information(this, TRANSLATE_STR_RSERVER_WARNING,
                                            TRANSLATE_STR_RSERVER_SOFTWARE_MEASUREMENT_RUNNING, QMessageBox::Ok);
            return;
        }
    }

#endif
    QUpgradeCommand upgradeCommand(this, QSoftwareModel::CommandUpgrade,
                                   deviceList());
    m_softwareModel->setUpgradeInfoList(QVariantList());
    getBackProgress()->setText(TRANSLATE_STR_RSERVER_PROGRESS);

    if (QBaseProgress::Timeout == issueCommand(upgradeCommand.toVariant(),
            "checkUpgrade", 10 * QSoftwareModel::PackageTimeout)) {
    }

    getBackProgress()->setText(TRANSLATE_STR_RSERVER_PROGRESS);
}

const QVariantList QSoftwareWidget::deviceList() const
{
    return m_pSoftwareTableWidget->getCheckedItems();
}

void QSoftwareWidget::setDeviceList(const QVariantList &deviceList)
{
    QList<QVariant>::const_iterator i;
    QVariantList filteredDeviceList;
    int lastDeviceType = -1;

    for (i = deviceList.constBegin(); i != deviceList.constEnd(); ++i) {
        QDeviceInfo device;
        device.fromVariant((*i).toMap());
        qprofileDebug(QtDebugMsg) << "serialNumber = " << device.serialNumber();

        if (lastDeviceType != static_cast<int>(device.deviceType())) {
            lastDeviceType = static_cast<int>(device.deviceType());
            filteredDeviceList.append((*i));
        }
    }

    static QVariantList sDeviceList;

    if (sDeviceList == filteredDeviceList)
        return;

    m_pSoftwareTableWidget->clear();
    m_pSoftwareTableWidget->addItems(filteredDeviceList);
    sDeviceList = filteredDeviceList;
}
void QSoftwareWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    m_pSoftwareTableWidget->setSize(rect().width(), rect().height());
    m_pSoftwareTableWidget->setColumnWidth(0, (int)(width() * 0.08));
    m_pSoftwareTableWidget->setColumnWidth(1, (int)(width() * 0.28));
    m_pSoftwareTableWidget->setColumnWidth(2, (int)(width() * 0.32));
    m_pSoftwareTableWidget->setColumnWidth(3, (int)(width() * 0.32 + 1));
}
void QSoftwareWidget::translate(const QFont &font)
{
    QStringList headerList = QStringList() << "" <<
                             TRANSLATE_STR_RSERVER_SOFTWARE_TYPE <<
                             TRANSLATE_STR_RSERVER_SOFTWARE_CURRENT_VERSION <<
                             TRANSLATE_STR_RSERVER_SOFTWARE_NEW_VERSION;
    int addColumnCount = QDeviceInfo::PropertyList.count() - headerList.count() + 1;

    for (int i = 0; i < addColumnCount; i++) {
        headerList.append("");
    }

    m_pSoftwareTableWidget->setHorizontalHeaderLabels(headerList);
    m_pSoftwareTableWidget->setFont(font);
}
QSoftwareWidget::~QSoftwareWidget()
{
}
