#ifndef QTECHINFO_H
#define QTECHINFO_H

#include <QObject>
#include <QSharedPointer>
#include "qprofile_debug.h"
#include "qprofile_global.h"
#include "qstringattribute.h"
#include "qabstractmodel.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QRserver
{
    class QTechInfo: public QProfile::QAbstractModel
    {
            Q_OBJECT

            Q_PROPERTY(QString techId READ techId WRITE setTechId)
            Q_PROPERTY(QString techName READ techName WRITE setTechName)
            Q_PROPERTY(QString company READ company WRITE setCompany)
            Q_PROPERTY(QString supervisorId READ supervisorId WRITE setSupervisorId)

        public:
            static const QString TechInfoFilePath;

        public:
            QTechInfo(QObject *parent = nullptr);
            ~QTechInfo() {}

            QString techId() const;
            void setTechId(const QString &techId);
            QString techName() const;
            void setTechName(const QString &techName);
            QString company() const;
            void setCompany(const QString &company);
            QString supervisorId() const;
            void setSupervisorId(const QString &supervisorId);

        private:
            void initAttributes();

        private:
            QString m_techID;
            QString m_techName;
            QString m_company;
            QString m_supervisorId;
    };
}
Q_DECLARE_METATYPE(QRserver::QTechInfo)
#endif
