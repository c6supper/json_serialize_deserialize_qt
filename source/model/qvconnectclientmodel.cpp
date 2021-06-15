/*
 * qvconnectclientmodel.cpp
 *
 *  Created on: Apr 29, 2020
 *      Author: duyi
 */
#include "qvconnectclientmodel.h"
#include "qobjectfactory.h"

using namespace QProfile;
using namespace QRserver;

QObjectSelfRegisteration<QVconnectClientModel> registerQVconnectClientModel;

QString QVconnectClientModel::RserverVconnectCommandTopic = "";
QString QVconnectClientModel::RserverVconnectStatusTopic = "";

QVconnectClientModel::QVconnectClientModel(QObject *parent): QAbstractModel(
        parent)
{
    Q_ASSERT(parent != nullptr);
}

void QVconnectClientModel::initAttributes()
{
}

QVconnectClientModel::~QVconnectClientModel()
{
}
