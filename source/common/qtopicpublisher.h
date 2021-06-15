/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QTOPICPUBLISHER_H
#define QTOPICPUBLISHER_H

#include <QObject>
#include <QPointer>
#include <QList>
#include <QMutexLocker>

namespace QRserver
{
    class QTopicPublisherList : public QObject
    {
            Q_OBJECT

        public:
            QTopicPublisherList(QObject *parent, QObject *publisher): QObject(parent),
                m_published(false)
            {
                registerPublisher(publisher);
            }
            ~QTopicPublisherList()
            {
                m_publisherList.clear();
            }

            bool isPublished()
            {
                return m_published;
            }
            void setPublished(bool published)
            {
                m_published = published;
            }

            void registerPublisher(QObject *publisher)
            {
                QMutexLocker locker(&m_mutex);

                if (!m_publisherList.contains(publisher)) {
                    m_publisherList.append(publisher);
                    connect(publisher, SIGNAL(destroyed(QObject *)), this,
                            SLOT(subDestroyed(QObject *)));
                }
            }
            const QByteArray &getRetainMsg()
            {
                return m_retainMsg;
            }

        protected slots:
            void subDestroyed(QObject *publisher)
            {
                QMutexLocker locker(&m_mutex);
                m_publisherList.removeAll(publisher);
            }

        protected:
            void setRetainMsg(const QByteArray &msg)
            {
                m_retainMsg = msg;
            }

        private:
            QList<QPointer<QObject> >	m_publisherList;
            QByteArray					m_retainMsg;
            bool						m_published;
            QMutex						m_mutex;
    };
}

#endif
