/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qotdrresultfilter.h"
#include "qprofile_debug.h"
#include <QFileInfo>
#include <QDir>

using namespace QRserver;

const QString QOtdrResultFilter::otdrKeyFile = "metadata.xml";

QOtdrResultFilter::QOtdrResultFilter(QObject *parent)
    : QAbstractResultFilter("OTDR", "", parent)
{
}

const QString QOtdrResultFilter::keyFilePattern()
{
    return QOtdrResultFilter::otdrKeyFile;
}

const QString QOtdrResultFilter::filterPattern()
{
    return "\\.xml";
}

bool QOtdrResultFilter::isKey(const QString absolutePath)
{
    QFileInfo fileInfo(absolutePath);

    if (fileInfo.exists() && fileInfo.isFile()) {
        return fileInfo.fileName().toLower() == otdrKeyFile;
    }

    return false;
}

const QString QOtdrResultFilter::getDisplayName(const QString &absoluteFilePath)
{
    QFileInfo fileInfo(absoluteFilePath);
    return fileInfo.absoluteDir().dirName();
}

const QStringList QOtdrResultFilter::getRelatedFileFolderList(
    const QString absoluteKeyPath)
{
    QStringList relatedList;

    if (isKey(absoluteKeyPath)) {
        //metadata.xml should not be upload
        QFileInfo keyInfo(absoluteKeyPath);
        QDir dir = keyInfo.absoluteDir();
        //todo OTDR have it's own advanced info,should parse it's xml file
        //if not list ,should create default xml file
        QFileInfo xmlFileInfo(dir.absolutePath() + "/" + dir.dirName());
        patchRelatedFiles(xmlFileInfo.absoluteFilePath(), keyInfo.lastModified());
        dir.setFilter(QDir::Files | QDir::NoSymLinks);
        QFileInfoList list = dir.entryInfoList();

        for (int i = 0; i < list.size(); ++i) {
            QFileInfo fileInfo = list.at(i);

            if (fileInfo.fileName() != otdrKeyFile) {
                relatedList << fileInfo.absoluteFilePath();
            }
        }

        qprofileDebug(QtDebugMsg) << relatedList;
    }

    return relatedList;
}

const QStringList QOtdrResultFilter::getFileListNeedToBeDeleted(
    const QString absoluteKeyPath)
{
    QStringList fileListNeedToBeDeleted = getRelatedFileFolderList(absoluteKeyPath);
    fileListNeedToBeDeleted << absoluteKeyPath;
    return fileListNeedToBeDeleted;
}

QOtdrResultFilter::~QOtdrResultFilter()
{
}
