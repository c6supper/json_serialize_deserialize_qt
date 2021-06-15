#include "qrserverprofileinfo.h"

using namespace QRserver;

QRserverProfileInfo::QRserverProfileInfo(QObject *parent)
    : QProfile::QAbstractJsonElement(parent), m_fileSize(0)
{
}

QRserverProfileInfo::~QRserverProfileInfo()
{
}

void QRserverProfileInfo::setProfileInfo(const QString &pathName)
{
    QFileInfo fileInfo(pathName);

    if (!fileInfo.exists()) {
        return;
    }

    m_pathName = pathName;
    m_downloadTime = fileInfo.created();
    m_fileSize = fileInfo.size();
    m_md5sum = fileChecksum(pathName, QCryptographicHash::Md5);
}

QString QRserverProfileInfo::pathName() const
{
    return m_pathName;
}
void QRserverProfileInfo::setPathName(QString pathName)
{
    m_pathName = pathName;
}

QDateTime QRserverProfileInfo::downloadTime() const
{
    return m_downloadTime;
}
void QRserverProfileInfo::setDownloadTime(QDateTime dateTime)
{
    m_downloadTime = dateTime;
}

quint32 QRserverProfileInfo::fileSize() const
{
    return m_fileSize;
}
void QRserverProfileInfo::setFileSize(quint32 fileSize)
{
    m_fileSize = fileSize;
}

QString QRserverProfileInfo::md5sum() const
{
    return m_md5sum;
}
void QRserverProfileInfo::setMd5sum(QString md5sum)
{
    m_md5sum = md5sum;
}

QString QRserverProfileInfo::fileChecksum(const QString &fileName,
        QCryptographicHash::Algorithm hashAlgorithm)
{
    QFile sourceFile(fileName);
    const qint64 bufferSize = 1024;

    if (sourceFile.size() > 0 && sourceFile.open(QIODevice::ReadOnly)) {
        QCryptographicHash hash(hashAlgorithm);
        QByteArray buffer;

        while (true) {
            buffer = sourceFile.read(bufferSize);

            if (buffer.isEmpty()) {
                break;
            } else {
                hash.addData(buffer);
            }
        }

        sourceFile.close();
        return QString(hash.result().toHex());
    }

    return QString();
}
