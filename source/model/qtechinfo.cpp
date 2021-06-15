#include "qtechinfo.h"
//#include "qobjectfactory.h"
//#include <QMetaProperty>

using namespace QProfile;
using namespace QRserver;

//QObjectSelfRegisteration<QRserver::QTechInfo> registerQDeviceInfo;

const QString QTechInfo::TechInfoFilePath = "/tmp/var/run/rserver/techinfo";
#if 0
const QStringList QTechInfo::propertyList()
{
    QStringList list;

    for (int propertyIdx = 0;
         propertyIdx < QTechInfo::staticMetaObject.propertyCount(); propertyIdx++) {
        QMetaProperty property = QTechInfo::staticMetaObject.property(propertyIdx);
        list.append(property.name());
    }

    list.removeOne("objectName");
    list.removeOne("command");
    return list;
}
#endif
QTechInfo::QTechInfo(QObject *parent) : QAbstractModel(parent)
{
    Q_ASSERT(parent != nullptr);
}

void QTechInfo::initAttributes()
{
}

QString QTechInfo::techId() const
{
    return m_techID;
}

void QTechInfo::setTechId(const QString &techId)
{
    m_techID = techId;
}

QString QTechInfo::techName() const
{
    return m_techName;
}

void QTechInfo::setTechName(const QString &techName)
{
    m_techName = techName;
}

QString QTechInfo::company() const
{
    return m_company;
}

void QTechInfo::setCompany(const QString &company)
{
    m_company = company;
}

QString QTechInfo::supervisorId() const
{
    return m_supervisorId;
}

void QTechInfo::setSupervisorId(const QString &supervisorId)
{
    m_supervisorId = supervisorId;
}

