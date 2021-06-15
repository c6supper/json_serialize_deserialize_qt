/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include <QDateTime>
#include "qmessagefactory.h"
#include "qmsgprofileinfo.h"
#include "qfilemodel.h"

using namespace QRserver;
using namespace QProfile;
using namespace rserver;

QMessageSelfRegisteration<QMsgProfileInfo> registerQMsgProfileInfo(
    rserver::eProfileInfo);

QDataStream &QMsgProfileInfo::read(QDataStream &in)
{
    QMutexLocker locker(m_mutex);
    st_ProfileInfo stMessage;
    m_profileInfoList.clear();

    if ((qint32)sizeof(stMessage) <= in.readRawData((char *)&stMessage,
            sizeof(stMessage))) {
        qint32 profileCount = stMessage.nCount < 100 ? stMessage.nCount : 100;

        for (int idx = 0; idx < profileCount; idx++) {
            st_CoreProfileInfo profileInfo = stMessage.ArrInfo[idx];
            QVariantMap profileInfoMap;
            profileInfoMap[QFileModel::PropertyFileName] = QString(profileInfo.szName);
            profileInfoMap[QFileModel::PropertyFileType] = profileInfo.nType;
            profileInfoMap[QFileModel::PropertyFileSize] = profileInfo.nSize;
#if defined(Q_OS_LINUX)
            struct tm tmDateTime = {0};
            tmDateTime.tm_sec = profileInfo.nArrDate[5];
            tmDateTime.tm_min = profileInfo.nArrDate[4];
            tmDateTime.tm_hour = profileInfo.nArrDate[3];
            tmDateTime.tm_mday = profileInfo.nArrDate[2];
            tmDateTime.tm_mon = profileInfo.nArrDate[1] - 1;
            tmDateTime.tm_year = profileInfo.nArrDate[0] - 1900;
            tmDateTime.tm_wday = 0;
            tmDateTime.tm_yday = 0;
            tmDateTime.tm_isdst = 0;
            time_t time_new = timelocal((struct tm *)&tmDateTime);
            profileInfoMap[QFileModel::PropertyRemoteDateTime] = QVariant(
                        QDateTime::fromTime_t(time_new));
#else
            profileInfoMap[QFileModel::PropertyRemoteDateTime] = QVariant(
                        QDateTime::currentDateTime());
#endif
            m_profileInfoList.append(profileInfoMap);
        }
    }

    return in;
}

const QByteArray QMsgProfileInfo::toByteArray() const
{
    st_ProfileInfo stMessage;
    bzero(&stMessage, sizeof(st_ProfileInfo));
    QByteArray messageArray((const char *)&stMessage, sizeof(stMessage));
    QDataStream out(&messageArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_6);
    out << *this;
    return messageArray;
}

QDataStream &QMsgProfileInfo::write(QDataStream &out) const
{
    QMutexLocker locker(m_mutex);
    st_ProfileInfo stMessage;
    bzero(&stMessage, sizeof(st_ProfileInfo));
    stMessage.nCount = (m_profileInfoList.count() < 100 ? m_profileInfoList.count()
                        : 100);
    qint32 index = 0;
    QVariantList::const_iterator i;

    for (i = m_profileInfoList.constBegin(); (i != m_profileInfoList.constEnd())
         && (index < 100);
         ++i) {
        QVariantMap profileInfoMap;
        profileInfoMap = (*i).toMap();
        st_CoreProfileInfo profileInfo;
        bzero(&profileInfo, sizeof(profileInfo));
        QStringAttribute::StringToChar(
            profileInfoMap[QFileModel::PropertyFileName].toString(),
            profileInfo.szName, sizeof(profileInfo.szName));
        profileInfo.nType = profileInfoMap[QFileModel::PropertyFileType].toInt();
        profileInfo.nSize = profileInfoMap[QFileModel::PropertyLocalSize].toInt();
#if defined(Q_OS_LINUX)
        time_t timeT = (time_t)(
                           profileInfoMap[QFileModel::PropertyDateTime].toDateTime().toTime_t());
        struct tm *timeTM = localtime(&timeT);
        profileInfo.nArrDate[5] = timeTM->tm_sec;
        profileInfo.nArrDate[4] = timeTM->tm_min;
        profileInfo.nArrDate[3] = timeTM->tm_hour;
        profileInfo.nArrDate[2] = timeTM->tm_mday;
        profileInfo.nArrDate[1] = timeTM->tm_mon + 1;
        profileInfo.nArrDate[0] = timeTM->tm_year + 1900;
#endif
        stMessage.ArrInfo[index++] = profileInfo;
    }

    out.writeRawData((const char *)&stMessage, sizeof(stMessage));
    return out;
}

QMsgProfileInfo::QMsgProfileInfo(QObject *parent)
    : QAbstractBinMsg(rserver::eProfileInfo, parent), m_mutex(new QMutex())
{
}

QMsgProfileInfo::QMsgProfileInfo(const QMsgProfileInfo &message)
    : QAbstractBinMsg(message), m_mutex(new QMutex())
{
}

QMsgProfileInfo::~QMsgProfileInfo()
{
    delete m_mutex;
}

QVariantList QMsgProfileInfo::profileInfoList() const
{
    return m_profileInfoList;
}
void QMsgProfileInfo::setProfileInfoList(QVariantList profileInfoList)
{
    m_profileInfoList = profileInfoList;
}


