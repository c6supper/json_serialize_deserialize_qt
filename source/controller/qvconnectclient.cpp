#include "qvconnectclient.h"
#include "qmodelfactory.h"
#include "qvconnectclientmodel.h"
#include "qabstracttopic.h"
#include "qvconnectmodel.h"
using namespace QProfile;
using namespace QRserver;

QVconnectClient::QVconnectClient(QObject *parent) :
    QRserverClient(parent)
{
}

void QVconnectClient::allocTopic()
{
    QRserverContainer::allocTopic();
    QVconnectClientModel::RserverVconnectCommandTopic = m_topic->getRestfulTopic(
                QAbstractTopic::command);
    QVconnectClientModel::RserverVconnectStatusTopic = m_topic->getRestfulTopic(
                QAbstractTopic::status);
}

bool QVconnectClient::initPropertiesMap(const QVariantMap &properties)
{
    if (properties.count() <= 0) {
        setProperty("tagName", "qvconnectclient");
    }

    return QRserverContainer::initPropertiesMap(properties);
}

void QVconnectClient::allocModel()
{
    setModel(m_vconnectClientModel = QModelFactory::construct<QVconnectClientModel>
                                     (this));
}

const QString QVconnectClient::getServerIP()
{
    const QVConnectModel *pVConnectModel =
        QModelFactory::constModel<QVConnectModel>(this);
    return pVConnectModel->vconnectServerIp();
}

QVconnectClient::~QVconnectClient()
{
}

