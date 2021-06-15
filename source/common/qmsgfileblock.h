/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QMSGFILEBLOCK_H
#define QMSGFILEBLOCK_H

#include <QMetaType>
#include <QByteArray>
#include <QString>
#include "qabstractbinmsg.h"
#include "qprofile_debug.h"
#include "qprofile_global.h"
#include "qstringattribute.h"

namespace QRserver
{
    class QMsgFileBlock : public QAbstractBinMsg
    {
            Q_OBJECT

            Q_PROPERTY(QString folderName READ folderName WRITE setFolderName)
            Q_PROPERTY(QString fileName READ fileName WRITE setFileName)
            Q_PROPERTY(qint32 blockIndex READ blockIndex WRITE setBlockIndex)
            Q_PROPERTY(qint32 blockLength READ blockLength WRITE setBlockLength)
            Q_PROPERTY(QByteArray block	READ block WRITE setBlock)

        public:
            static const qint32 MaxFileBlock;
        public:
            QMsgFileBlock(QObject *parent = nullptr);
            QMsgFileBlock(const QMsgFileBlock &message);
            virtual ~QMsgFileBlock();

            const QString folderName();
            void setFolderName(QString folderName);

            const QString fileName();
            void setFileName(QString fileName);

            quint32 blockIndex() const;
            void setBlockIndex(quint32 blockIndex);

            quint32 blockLength() const;
            void setBlockLength(quint32 blockLength);

            const QByteArray block() const;
            void setBlock(QByteArray block);

        protected:
            virtual QDataStream &read(QDataStream &in) ;
            virtual QDataStream &write(QDataStream &out) const;
            virtual const QByteArray toByteArray() const;
            virtual void initAttributes();

            void getFolderName(char *string, int length) const;
            void getFileName(char *string, int length) const;
            void clearBlock();
            void resizeBlock(int size);
            const char *blockData() const;
            char *blockData();

        private:
            QProfile::QStringAttribute 		*m_folderName;
            QProfile::QStringAttribute 		*m_fileName;
            qint32 							m_blockIndex;
            qint32 							m_blockLength;
            QByteArray						m_block;
    };
}

Q_DECLARE_METATYPE(QRserver::QMsgFileBlock)

#endif
