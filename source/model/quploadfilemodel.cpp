/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */


#include "quploadfilemodel.h"
#include "qobjectfactory.h"
#include "qabstractcommand.h"

using namespace QProfile;
using namespace QRserver;

QObjectSelfRegisteration<QUploadFileModel> registerQUploadFileModel;

const QString QUploadFileModel::CommandGetFileList = "getFileList";
const QString QUploadFileModel::CommandUpload = "upload";
const QString QUploadFileModel::CommandUploadDelete = "uploadDelete";
const quint32 QUploadFileModel::UploadSingleFileTimeout = 20000;
const quint32 QUploadFileModel::GetFileListTimeout = 5 * 60 * 1000;

QUploadFileModel::QUploadFileModel(QObject *parent) : QAbstractModel(parent),
    m_mutex(new QMutex())
{
    Q_ASSERT(parent != nullptr);
}

void QUploadFileModel::initAttributes()
{
}

const QVariantList QUploadFileModel::fileModelList() const
{
    QMutexLocker locker(m_mutex);
    QVariantList fileList;
    QList<QFileModel *>::const_iterator i;

    for (i = m_fileList.constBegin(); i != m_fileList.constEnd(); ++i) {
        fileList.append((*i)->toVariant());
    }

    return fileList;
}

/*
void QUploadFileModel::appendFile(const QVariant fileVariant)
{
    QMutexLocker locker(m_mutex);
    QFileModel *file = new QFileModel(this);
    file->fromVariant(fileVariant.toMap());
    QList<QFileModel *>::iterator i;

    for (i = m_fileList.begin(); i != m_fileList.end(); ++i) {
        if (file->pathName() == (*i)->pathName()) {
            bool lastStatus = blockNotify(true);
            m_fileList.removeOne(*i);
            blockNotify(lastStatus);
            break;
        }
    }

    m_fileList.append(file);
}
*/

/*const QVariantList QUploadFileModel::takeFileList()
{
    QMutexLocker locker(m_mutex);
    QVariantList fileList;
    QList<QFileModel *>::const_iterator i;

    while (!m_fileList.isEmpty()) {
        QFileModel *file = m_fileList.takeFirst();
        fileList.append(file->toVariant());
        delete file;
    }

    return fileList;
}*/

void QUploadFileModel::setFileModelList(const QVariantList &fileModelList)
{
    if (fileModelList == this->fileModelList()) {
        qprofileDebug(QtDebugMsg) << "fileModelList is the same !";
        return ;
    }

    QMutexLocker locker(m_mutex);

    while (!m_fileList.isEmpty())
        delete m_fileList.takeFirst();

    QList<QVariant>::const_iterator i;

    for (i = fileModelList.constBegin(); i != fileModelList.constEnd(); ++i) {
        QFileModel *file = new QFileModel(this);
        file->blockNotify(true);
        file->fromVariant((*i).toMap());
        file->blockNotify(false);
        m_fileList.append(file);
    }

    asynPropertyChangeNotify();
}

void QUploadFileModel::clear()
{
    QMutexLocker locker(m_mutex);

    while (!m_fileList.isEmpty())
        delete m_fileList.takeFirst();
}

const QVariantList QUploadFileModel::getFileModeList(quint32 fileType,
        const QString profileName)
{
    QVariantList fileModelList = this->fileModelList() ;
    QVariantList::const_iterator i;
    QVariantList relatedFileList;

    for (i = fileModelList.constBegin(); i != fileModelList.constEnd(); ++i) {
        QFileModel fileModel;
        fileModel.fromVariant(*i);

        if (fileModel.fileName() == profileName
            && fileModel.fileType() == fileType) {
            relatedFileList.append(*i);
        }
    }

    return relatedFileList;
}

QUploadFileModel::~QUploadFileModel()
{
    clear();
    delete m_mutex;
}
