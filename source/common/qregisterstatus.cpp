#include "qregisterstatus.h"
#include "qobjectfactory.h"

using namespace QRserver;
using namespace QProfile;

QObjectSelfRegisteration<QRegisterStatus> registerQRegisterStatus;

QRegisterStatus::QRegisterStatus(QObject *parent): QAbstractStatus(
        parent)
{
}

QRegisterStatus::QRegisterStatus(const QVariant &statusVar) :
    QAbstractStatus(statusVar)
{
    fromVariant(statusVar);
}

QRegisterStatus::QRegisterStatus(QObject *parent, const QString &id,
                                 const quint32 commandStatus)
    : QAbstractStatus(parent, id, "", commandStatus)
{
}

const QString QRegisterStatus::compatibilityClassName() const
{
    return m_compatibilityClassName;
}
void QRegisterStatus::setCompatibilityClassName(const QString
        compatibilityClassName)
{
    m_compatibilityClassName = compatibilityClassName;
}

const QVariant QRegisterStatus::rserverModelVar()
{
    return m_rserverModelVar;
}

void QRegisterStatus::setRserverModelVar(const QVariant rserverModelVar)
{
    m_rserverModelVar = rserverModelVar;
}

const QVariant QRegisterStatus::registerModelVar()
{
    return m_registerModelVar;
}

void QRegisterStatus::setRegisterModelVar(const QVariant registerModelVar)
{
    m_registerModelVar = registerModelVar;
}

QRegisterStatus::~QRegisterStatus()
{
}

