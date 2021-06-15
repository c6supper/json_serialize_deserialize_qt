/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */


#include "qclientmodel.h"
#include "qobjectfactory.h"
#include "qabstractcontainer.h"
#include "qmodelfactory.h"
#if defined(BUILD_X86)
#include "qjson/parser.h"
#include "qjson/serializer.h"
#include "qjson/qobjecthelper.h"
#else
#include <QJson/Parser>
#include <QJson/Serializer>
#include <QJson/QObjectHelper>
#endif

using namespace QProfile;
using namespace QRserver;

QObjectSelfRegisteration<QClientModel> registerQClientModel;

const QString QClientModel::CommandProcessMessage = "processMessage";
QString QClientModel::RserverClientCommandTopic = "";
QString QClientModel::RserverClientStatusTopic = "";

QClientModel::QClientModel(QObject *parent) : QAbstractModel(parent)
{
    Q_ASSERT(parent != nullptr);
    m_registerModel = QSharedPointer<const QRegisterModel>
                      (QModelFactory::constModel<QRegisterModel>(this));
}

QClientModel::QClientModel(const QClientModel &model) : QAbstractModel(model)
{
    m_registerModel = QSharedPointer<const QRegisterModel>
                      (QModelFactory::constModel<QRegisterModel>(this));
}

QClientModel::~QClientModel()
{
    m_registerModel.clear();
}

/*QVariantList QClientModel::messageSeries()
{
    QMutexLocker locker(&m_mutex);
    QVariantList messageSerieList;

    for (int i = 0; i < m_messageSeriesList.size(); ++i) {
        QMessageSeries *model = m_messageSeriesList.at(i);
        QVariant modelVar = QJson::QObjectHelper::qobject2qvariant(model);
        messageSerieList.append(modelVar);
    }

    return messageSerieList;
}

QVariant QClientModel::takeFirstMessageSeries()
{
    QMutexLocker locker(&m_mutex);
    QVariant messageSerie;

    if (m_messageSeriesList.size() > 0) {
        QMessageSeries *model = m_messageSeriesList.takeFirst();
        messageSerie = QVariant(QJson::QObjectHelper::qobject2qvariant(model));
    }

    return messageSerie;
}

void QClientModel::appendMessageSeries(QVariantList &messageSeriesList)
{
    QMutexLocker locker(&m_mutex);

        while (!m_messageSeriesList.isEmpty())
            delete m_messageSeriesList.takeFirst();

    for (int i = 0; i < messageSeriesList.size(); ++i) {
        QVariant modelVar = messageSeriesList.at(i);
        QMessageSeries *model = new QMessageSeries(this);
        QJson::QObjectHelper::qvariant2qobject(modelVar.toMap(), model);
        m_messageSeriesList.append(model);
    }
}*/

void QClientModel::initAttributes()
{
}
