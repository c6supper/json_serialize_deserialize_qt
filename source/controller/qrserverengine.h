/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QRSERVERENGINE_H
#define QRSERVERENGINE_H

#include <QPointer>
#include "qprofile_global.h"
#include "qabstractprofile.h"

QT_BEGIN_NAMESPACE
namespace QProfile
{
    class QAbstractProfile;
    class QAbstractIpcClient;
    class QModelFactory;
}
namespace QRserver
{
    class QRserverContainer;
    class QRserverStateMachine;
    class QRserverClient;
    class QVconnectClient;
}
QT_END_NAMESPACE

namespace QRserver
{
    class QRserverEngine: public QProfile::QAbstractProfile
    {
            Q_OBJECT

        public:
            QRserverEngine(QObject *parent = nullptr);
            ~QRserverEngine();

            virtual bool deSerialize(PaserType type)
            {
                Q_ASSERT(false);
                return false;
            }
            virtual QByteArray serialize(SerializerType type, bool *ok)
            {
                Q_ASSERT(false);
                return "";
            }

        protected:
            void initStates();

        private:
            QPointer<QProfile::QAbstractIpcClient>	m_ipcClient;
            QPointer<QRserverContainer>				m_rootContainer;
            QPointer<QRserverClient>				m_rserverClient;
            QPointer<QProfile::QModelFactory>		m_modelFactory;
            QPointer<QVconnectClient>				m_vconnectClient;

        private slots:
            void onIpcConnected();
#ifdef BUILD_X86
            void loadFakeData();
#endif
    };
}


#endif /* QRSERVERENGINE_H */
