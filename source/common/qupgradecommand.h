#ifndef QUPGRADECOMMAND_H
#define QUPGRADECOMMAND_H

#include "qabstractcommand.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

using namespace QProfile;

namespace QRserver
{
    class QUpgradeCommand : public QAbstractCommand
    {
            Q_OBJECT

            Q_PROPERTY(QVariantList deviceInfoList READ deviceInfoList WRITE
                       setDeviceInfoList)

        public:
            QUpgradeCommand(QObject *parent = nullptr);
            QUpgradeCommand(QObject *parent, const QString &method,
                            const QVariant &deviceInfoList, const QString &topic = "");
            ~QUpgradeCommand();

            virtual bool isValid();
            virtual void pureVirtualMock() {};

            const QVariantList deviceInfoList() const;
            void setDeviceInfoList(const QVariantList &deviceInfoList);
    };
}

Q_DECLARE_METATYPE(QRserver::QUpgradeCommand)

#endif // QUPGRADECOMMAND_H
