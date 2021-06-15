/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qetherresultfilter.h"
#include "qprofile_debug.h"
#include <QDir>

using namespace QRserver;

QEtherResultFilter::QEtherResultFilter(QObject *parent)
    : QAbstractResultFilter("Ethernet", "", parent)
{
}

const QString QEtherResultFilter::keyFilePattern()
{
    return "xxxx.html";
}

const QString QEtherResultFilter::filterPattern()
{
    return "\\.html";
}

bool QEtherResultFilter::isKey(const QString absolutePath)
{
    QFileInfo fileInfo(absolutePath);

    if (fileInfo.exists() && fileInfo.isFile()) {
        return fileInfo.suffix() == "html";
    }

    return false;
}

const QStringList QEtherResultFilter::getRelatedFileFolderList(
    const QString absoluteKeyPath)
{
    QStringList relatedList;

    if (isKey(absoluteKeyPath)) {
        relatedList << absoluteKeyPath;
        QFileInfo keyInfo(absoluteKeyPath);
        QDir dir = keyInfo.absoluteDir();
        //if not list ,should create default xml file
        QFileInfo xmlFileInfo(keyInfo.absoluteDir().absolutePath() + "/" +
                              xmlFileName(keyInfo));
        patchRelatedFiles(xmlFileInfo.absoluteFilePath(), keyInfo.lastModified());
        relatedList << xmlFileInfo.absoluteFilePath();

        if (dir.cd(keyInfo.completeBaseName() + "_files")) {
            dir.setFilter(QDir::Files | QDir::NoSymLinks);
            QFileInfoList list = dir.entryInfoList();

            for (int i = 0; i < list.size(); ++i) {
                QFileInfo fileInfo = list.at(i);
                relatedList << fileInfo.absoluteFilePath();
            }
        }
    }

    return relatedList;
}

const QString QEtherResultFilter::xmlFileName(const QFileInfo keyInfo)
{
    return keyInfo.completeBaseName();
}

QEtherResultFilter::~QEtherResultFilter()
{
}
