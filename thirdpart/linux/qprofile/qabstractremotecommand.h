
#ifndef QABSTRACTREMOTECOMMAND_H
#define QABSTRACTREMOTECOMMAND_H

#include "qprofile_export.h"
#include "qprofile_debug.h"
#include "qprofile_global.h"
#include "qabstractjsonelement.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QProfile
{
    class QAbstractRemoteCommand : public QAbstractJsonElement
    {
            Q_OBJECT

            Q_PROPERTY(QString cmd READ cmd WRITE setCmd)
            Q_PROPERTY(QString input READ input WRITE setInput)
            Q_PROPERTY(QString output READ output WRITE setOutput)
            Q_PROPERTY(QString inTopic READ inTopic WRITE setInTopic)
            Q_PROPERTY(QString outTopic READ outTopic WRITE setOutTopic)
            Q_PROPERTY(quint32 timeout READ timeout WRITE setTimeout)
            Q_PROPERTY(bool isCache READ isCache WRITE setIsCache)
            Q_PROPERTY(QString executor READ executor WRITE setExecutor)
            Q_PROPERTY(QString method READ method WRITE setMethod)

        public:
            static const QString PropertyCMD;
            static const QString PropertyExecutor;
            static const QString PropertyMethod;
            static const QString DefaultRemoteExecutorCMDProfileConfig;

        public:
            QAbstractRemoteCommand(QObject *parent = nullptr);
            QAbstractRemoteCommand(const QAbstractRemoteCommand &cmd);
            ~QAbstractRemoteCommand();
            virtual void pureVirtualMock();

            QString cmd() const;
            void setCmd(QString cmd);

            QString input() const;
            virtual void setInput(QString input);

            QString output() const;
            void setOutput(QString output);

            QString inTopic() const;
            void setInTopic(QString inTopic);

            QString outTopic() const;
            void setOutTopic(QString outTopic);

            quint32 timeout() const;
            void setTimeout(quint32 timeout);

            bool isCache() const;
            void setIsCache(bool isCache);

            QString executor() const;
            void setExecutor(QString executor);

            QString method() const;
            void setMethod(const QString method);

            virtual bool isValid();
            virtual const QVariant parseExecOutput(const QVariant &output);
            virtual const QVariant toExecInput();

            static const QString GetCMD(const QVariant &variant);
            static const QString GetExecutor(const QVariant &variant);
            static const QStringList ArguList;

        private:
            static const QStringList propertyList();
            static const QStringList arguList();

        private:
            QString m_cmd;
            QString m_input;
            QString m_output;
            QString m_inTopic;
            QString m_outTopic;
            quint32 m_timeout;
            bool m_isCache;
            QString m_executor;
            QString m_method;
    };
}

#endif // QABSTRACTREMOTECOMMAND_H
