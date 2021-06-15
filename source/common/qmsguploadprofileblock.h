/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QMSGUPLOADPROFILEBLOCK_H
#define QMSGUPLOADPROFILEBLOCK_H

#include <QMetaType>
#include <QByteArray>
#include <QString>
#include "qabstractbinmsg.h"
#include "qprofile_debug.h"
#include "qprofile_global.h"
#include "qstringattribute.h"

namespace QRserver
{
    class QMsgUploadProfileBlock : public QAbstractBinMsg
    {
            Q_OBJECT

            Q_PROPERTY(QString profileName READ profileName WRITE setProfileName)
            Q_PROPERTY(qint32 profileType READ profileType WRITE setProfileType)
            Q_PROPERTY(qint32 blockIndex READ blockIndex WRITE setBlockIndex)
            Q_PROPERTY(qint32 blockLength READ blockLength)
            Q_PROPERTY(qint32 totalLength READ totalLength WRITE setTotalLength)
            Q_PROPERTY(QByteArray block	READ block WRITE setBlock)

        public:
            static const qint32 MaxUploadFileBlock;
        public:
            QMsgUploadProfileBlock(QObject *parent = nullptr);
            QMsgUploadProfileBlock(const QMsgUploadProfileBlock &message);
            virtual ~QMsgUploadProfileBlock();

            const QString profileName();
            void setProfileName(QString profileName);

            quint32 profileType() const;
            void setProfileType(quint32 profileType);

            quint32 blockIndex() const;
            void setBlockIndex(quint32 blockIndex);

            virtual quint32 blockLength() const;

            quint32 totalLength() const;
            void setTotalLength(quint32 totalLength);

            const QByteArray block() const;
            void setBlock(QByteArray block);

        protected:
            virtual QDataStream &read(QDataStream &in) ;
            virtual QDataStream &write(QDataStream &out) const;
            virtual const QByteArray toByteArray() const;
            virtual void initAttributes();

        private:
            qint32 							m_profileType;
            qint32 							m_blockIndex;
            qint32 							m_totalLength;

        protected:
            QByteArray						m_block;
            QProfile::QStringAttribute 		*m_profileName;
    };
}

Q_DECLARE_METATYPE(QRserver::QMsgUploadProfileBlock)

#endif
