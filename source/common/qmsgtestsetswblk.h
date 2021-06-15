/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QMSGTESTSETSWBLOCK_H
#define QMSGTESTSETSWBLOCK_H

#include <QByteArray>
#include "qabstractbinmsg.h"

namespace QRserver
{
    class QMsgTestsetSwBlk : public QAbstractBinMsg
    {
            Q_OBJECT

            Q_PROPERTY(QByteArray block READ block WRITE setBlock)

        public:
            static const qint32 MaxSwFileBlock;

        public:
            QMsgTestsetSwBlk(QObject *parent = nullptr);
            QMsgTestsetSwBlk(const QMsgTestsetSwBlk &message);
            virtual ~QMsgTestsetSwBlk();

            const QByteArray block() const;
            void setBlock(QByteArray block);

            const qint32 length() const;

        protected:
            virtual QDataStream &read(QDataStream &in) ;
            virtual QDataStream &write(QDataStream &out) const;
            virtual const QByteArray toByteArray() const;
            virtual void initAttributes();

        private:
            QByteArray	m_block;
    };
}

Q_DECLARE_METATYPE(QRserver::QMsgTestsetSwBlk)

#endif
