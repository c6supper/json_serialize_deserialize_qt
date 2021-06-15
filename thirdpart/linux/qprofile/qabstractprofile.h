/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QABSTRACTPROFILE_H
#define QABSTRACTPROFILE_H

#include <QObject>
#include <QMap>
#include <QBuffer>
#include <QMutex>
#include <QPointer>
#include "qabstractparser.h"
#include "qabstractserializer.h"

namespace QProfile
{
    class QAbstractProfile : public QObject
    {
            Q_OBJECT
        public:
            enum PaserType {XMLParser, SCPIParser};
            enum SerializerType {SCPISerializer, XMLSerializer};
            explicit QAbstractProfile(QObject *parent = nullptr);
            virtual ~QAbstractProfile() {}
            virtual bool deSerialize(PaserType type) = 0;
            virtual QByteArray serialize(SerializerType type, bool *ok) = 0;
            void registerParser(PaserType type, QAbstractParser *parser)
            {
                Q_ASSERT(parser != nullptr);

                if (m_parserMap.contains(type))
                    delete m_parserMap[type];

                m_parserMap[type] = parser;
            }

            void registerSerializer(SerializerType type, QAbstractSerializer *serializer)
            {
                Q_ASSERT(serializer != nullptr);

                if (m_serializerMap.contains(type))
                    delete m_serializerMap[type];

                m_serializerMap[type] = serializer;
            }

        protected:
            QMap<PaserType, QPointer<QAbstractParser> > m_parserMap;
            QMap<SerializerType, QPointer<QAbstractSerializer> > m_serializerMap;
            QBuffer m_profileBuffer;
            QMutex  m_profileMutex;
    };
}
#endif // QABSTRACTPROFILE_H
