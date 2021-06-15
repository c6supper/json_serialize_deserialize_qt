
#ifndef QVCONNECTRESULTMODEL_H
#define QVCONNECTRESULTMODEL_H

#include <QObject>
#include <QVariant>
#include <QList>
#include <QMutexLocker>
#include "qprofile_debug.h"
#include "qprofile_global.h"
#include "qresultmodel.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QRserver
{
    class QVConnectResultModel: public QProfile::QAbstractModel
    {
            Q_OBJECT

            Q_PROPERTY(QVariantList resultModelFileList READ resultModelFileList WRITE
                       setResultModelFileList)

        public:
            static const QString CommandGetResultList;

        public:
            QVConnectResultModel(QObject *parent = nullptr);
            ~QVConnectResultModel();

            const QVariantList resultModelFileList() const;
            void setResultModelFileList(const QVariantList &resultModelFileList);
            void clear();

        private:
            void initAttributes();

        private:
            QList<QResultModel *> m_resultModelList;
            QMutex *m_mutex;
    };
}

Q_DECLARE_METATYPE(QRserver::QVConnectResultModel)

#endif
