/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qrserverengine.h"
#include "qmqttipcclient.h"
#include "qprofile_debug.h"
#include "qrserver.h"
#include <QCoreApplication>
#include "qrserverstatemachine.h"
#include "qrserverclient.h"
#include "qvconnectclient.h"
#include "qmodelfactory.h"
#ifdef BUILD_X86
#include <QCoreApplication>
#include <QProcess>
#include <QTimer>
#endif

using namespace QProfile;
using namespace QMQTT;
using namespace QRserver;

QRserverEngine::QRserverEngine(QObject *parent)
    : QAbstractProfile(parent), m_rootContainer(nullptr)
{
    //todo tagName should contain hostinfo
    setProperty("tagName", "rserver");
    m_modelFactory = QModelFactory::Instance();
    m_ipcClient = QMqttIpcClient::Create(this);
    connect(m_ipcClient, SIGNAL(connected()), this, SLOT(onIpcConnected()));
    m_ipcClient->connectToHost();
}

QRserverEngine::~QRserverEngine()
{
}

#ifdef BUILD_X86
void QRserverEngine::loadFakeData()
{
    /*    QString loadFakeDevice =
            QString("mosquitto_pub -t \"rserver\\qrserver\\put\" -f %1/../%2").arg(
                QCoreApplication::applicationDirPath()).arg("testcase/rserverengine/qrserver/devicelist.json");
    QString loadFakeRegister =
        QString("mosquitto_pub -t \"rserver\\qrserver\\qregister\\put\" -f %1/../%2").arg(
            QCoreApplication::applicationDirPath()).arg("testcase/rserverengine/qrserver/qregister/put.json");
    QProcess::execute(loadFakeDevice);
    QProcess::execute(loadFakeRegister);*/
}
#endif

void QRserverEngine::onIpcConnected()
{
    qprofileDebug(QtDebugMsg) << "MQTT server connected!";

    if (m_rserverClient.isNull()) {
        m_rserverClient = new QRserverClient(this);
    }

    if (m_vconnectClient.isNull()) {
        m_vconnectClient = new QVconnectClient(this);
    }

    if (m_rootContainer.isNull()) {
        m_rootContainer = new QRserver(this);
    }

#ifdef BUILD_X86
    QTimer::singleShot(1500, this, SLOT(loadFakeData()));
#endif
}
