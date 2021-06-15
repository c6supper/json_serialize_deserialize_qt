#ifndef QVCONNECTCLIENTMODEL_H
#define QVCONNECTCLIENTMODEL_H
#include <QObject>
#include "qabstractmodel.h"

namespace QRserver
{
    class QVconnectClientModel: public QProfile::QAbstractModel
    {
            Q_OBJECT

        public:
            static QString RserverVconnectCommandTopic;
            static QString RserverVconnectStatusTopic;

        public:
            QVconnectClientModel(QObject *parent = nullptr);
            ~QVconnectClientModel();

        public slots:
            virtual void initAttributes();
    };
}

Q_DECLARE_METATYPE(QRserver::QVconnectClientModel)

#endif /* QVCONNECTCLIENTMODEL_H */
