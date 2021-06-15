
#include "qgetresultstatus.h"
#include "qobjectfactory.h"

using namespace QRserver;
using namespace QProfile;

QObjectSelfRegisteration<QGetResultStatus> registerQGetResultStatus;

QGetResultStatus::QGetResultStatus(QObject *parent): QUploadStatus(
        parent)
{
}

QGetResultStatus::QGetResultStatus(const QVariant &statusVar) :
    QUploadStatus(statusVar)
{
    fromVariant(statusVar);
}

QGetResultStatus::QGetResultStatus(QObject *parent, const QString &id,
                                   const quint32 commandStatus)
    : QUploadStatus(parent, id, commandStatus)
{
}

const QVariant QGetResultStatus::vConnectResultModel() const
{
    return m_vConnectResultModel;
}
void QGetResultStatus::setVConnectResultModel(const QVariant
        &vConnectResultModel)
{
    m_vConnectResultModel = vConnectResultModel;
}

QGetResultStatus::~QGetResultStatus()
{
}

