#include "qrserverui.h"
#include <QDebug>
#include "qmqttipcclient.h"
#include <QCoreApplication>
#include <QFile>
#include <QLatin1String>
#include "qrserverenvironment.h"
#include "qmsghandle.h"
#include "qautocheckevent.h"
#include "qrservercompatibility.h"
#include "qrservermessagebox.h"
#include "qresultfilterdialog.h"

using namespace QRserver;

#define RSERVER_UI_WIDTH  (660)
#define RSERVER_UI_HEIGHT (400)
#define BORDER_WIDTH (1)

QRserverUi::QRserverUi(QWidget *parent)
    : QWidget(parent), m_pRServer(nullptr)
{
    setProperty("tagName", "rserver");
#ifdef DEBUG_PRINT
    qInstallMsgHandler(qMessageOutput);
#endif
    QAbstractEnvironment::SetInstance(new QRserverEnvironment());
    QAbstractEnvironment::PrintEnvironmentVarList();
    QAbstractEnvironment::CheckEnvironment();
    QAbstractCompatibility::SetInstance(new QRserverCompatibility());
#ifdef BUILD_X86
    setFixedSize(RSERVER_UI_WIDTH, RSERVER_UI_HEIGHT);
#else
    setFixedSize(parent->width() - BORDER_WIDTH * 2,
                 parent->height() - BORDER_WIDTH * 2);
#endif
    loadStyleSheet();
    m_pMainLayout = new QHBoxLayout(this);
    m_pMainLayout->setMargin(0);
    m_pMainLayout->setSpacing(0);
    m_ipcClient = QMqttIpcClient::Create(this);
    connect(m_ipcClient, SIGNAL(connected()), this, SLOT(onIpcConnected()));
    connect(m_ipcClient, SIGNAL(disconnected()), this, SLOT(onIpcDisconnected()));
    m_ipcClient->connectToHost();
    connect(QRserverNetworkChecker::Instance(), SIGNAL(networkError()), this,
            SIGNAL(networkError()));
    QRserverMessageBox::Initialize(this);
    QResultFilterDialog::Initialize(this);
}

void QRserverUi::customEvent(QEvent *event)
{
    if (QAutoCheckEvent::EventType() == event->type()) {
        if (m_pRServer == nullptr) {
            return QCoreApplication::postEvent(this, new QProfile::QAutoCheckEvent(),
                                               Qt::LowEventPriority);
        } else {
            return QCoreApplication::postEvent(m_pRServer, new QProfile::QAutoCheckEvent());
        }
    }

    return QWidget::customEvent(event);
}

void QRserverUi::onIpcDisconnected()
{
    qprofileDebug(QtDebugMsg) << "MQTT server disconnected! m_ipcClient:" <<
                              m_ipcClient->isConnectedToHost();
    setDisabled(true);
}

void QRserverUi::onIpcConnected()
{
    qprofileDebug(QtDebugMsg) << "MQTT server connected! m_ipcClient:" <<
                              m_ipcClient->isConnectedToHost();

    if (m_pRServer == nullptr) {
        m_pRServer = new QRserverWidget(this);
        m_pMainLayout->addWidget(m_pRServer);
    }

    setDisabled(false);
}

void QRserverUi::translate(const QFont &font)
{
    m_pRServer->translate(font);
}

void QRserverUi::loadStyleSheet()
{
    QFile qssFile(":/qss/stylesheet.qss");

    if (qssFile.open(QFile::ReadOnly)) {
        qprofileDebug(QtDebugMsg) << "Open stylesheet.qss success.";
        QString styleSheet = QLatin1String(qssFile.readAll());
        setStyleSheet(styleSheet);
        qssFile.close();
    } else {
        qprofileDebug(QtDebugMsg) << "Open stylesheet.qss failed.";
    }
}

QRserverUi::~QRserverUi()
{
}

