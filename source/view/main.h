/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifdef BUILD_X86
#ifndef MAIN_H
#define MAIN_H

#include <QApplication>
#include <QThread>
#include "qautocheckevent.h"
#include "qrserverui.h"

extern QRserver::QRserverUi *pRServerUi;

namespace QRserver
{
    class PostPoneInitializer : public QThread
    {
            Q_OBJECT

        public:
            PostPoneInitializer(QObject *parent = nullptr): QThread(parent)
            {
            }
            ~PostPoneInitializer()
            {
            }
        protected:
            virtual void run()
            {
                QThread::sleep(5);
                autocheck();
            }
        private:
            void autocheck()
            {
                QCoreApplication::postEvent(pRServerUi, new QProfile::QAutoCheckEvent());
            }
    };
}

#endif
#endif
