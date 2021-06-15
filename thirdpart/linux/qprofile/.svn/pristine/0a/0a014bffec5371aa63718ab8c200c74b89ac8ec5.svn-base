#include "qabstractresponse.h"
#include "qobjectfactory.h"

using namespace QProfile;

QObjectSelfRegisteration<QAbstractResponse> registerQAbstractResponse;

const QString QAbstractResponse::PropertyId = "id";
const QString QAbstractResponse::PropertyType = "type";
const QString QAbstractResponse::PropertyTriggerResponse = "triggerResponse";
const QString QAbstractResponse::StepResponseMap = "stepResponseMap";

QAbstractResponse::QAbstractResponse(QObject *parent):
    QAbstractStatus(parent)
{
    //response not use status, set default.
    setCommandStatus(QProfile::QAbstractStatus::Succeed);
}

QAbstractResponse::QAbstractResponse(const QVariant &responseVar)
    : QAbstractStatus(responseVar)
{
    //response not use status, set default.
    setCommandStatus(QProfile::QAbstractStatus::Succeed);
}

QAbstractResponse::~QAbstractResponse()
{
}

const QString QAbstractResponse::serialNumber() const
{
    return m_serialNumber;
}
void QAbstractResponse::setSerialNumber(const QString serialNumber)
{
    m_serialNumber = serialNumber;
}

const QAbstractResponse::TriggerResponse QAbstractResponse::triggerResponse()
const
{
    return (QAbstractResponse::TriggerResponse)m_triggerResponse;
}

void QAbstractResponse::setTriggerResponse(const
        QAbstractResponse::TriggerResponse triggerResponse)
{
    m_triggerResponse = (quint32)triggerResponse;
}

