
#ifndef QCMDPARSER_H
#define QCMDPARSER_H

#include "qprofile_global.h"
#include "qprofile_debug.h"
#include <QObject>
#include <QList>
#include <QVariant>
#include <QMutexLocker>

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QRserver
{
    class QCMDParser : public QObject
    {
            Q_OBJECT

            Q_PROPERTY(QVariantList cmdList READ cmdList WRITE setCmdList)

        public:
            QCMDParser(QObject *parent = nullptr);
            ~QCMDParser();

            QVariantList cmdList();
            void setCmdList(QVariantList &cmdList);

            static const QVariant GetCMDInfo(const QString cmd);

        private:
            static QCMDParser *instance();
            const QVariant getCMDInfo(const QString cmd);

        private:
            static QCMDParser *m_instance;
            QMutex m_mutex;
            QVariantList m_cmdList;
    };
}

#endif /* QCMDPARSER_H */
