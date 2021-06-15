#include "qdetailtablewidget.h"
#include "qrserverfiletype.h"
#include "abstractlockdownsetting.h"
#include "qrserverprofile.h"
#include <QDir>

using namespace QRserver;

QDetailTableWidget::QDetailTableWidget(QWidget *parent)
{
}

QDetailTableWidget::~QDetailTableWidget()
{
}

void QDetailTableWidget::addItems(const QStringList &pathList)
{
    QDir dir;
    QRserverFileType fileType;
    QStringList fileList;

    foreach (QString path, pathList) {
#ifdef LOCK_LOCAL_PROFILES

        if (!AbstractLockDownSetting::Instance()->isProfileLocked(path.toStdString())) {
            continue;
        }

#endif
        fileType.fromVariant(QRserverProfile::GetFileType(path, true));
        dir.setPath(path);
        fileList = dir.entryList(QDir::Files);

        if (fileList.isEmpty()) {
            continue;
        }

        int row = 0;

        foreach (QString fileName, fileList) {
            if (!AbstractLockDownSetting::Instance()->isProfileLocked(
                    QString(path + "/" + fileName).toStdString())) {
                continue;
            }

            row = rowCount();
            insertRow(row);
            QStandardItem *itemName = new QStandardItem(fileName);
            setItem(row, QUploadFileTableWidget::ColumnIndexDisplayName, itemName);
            QStandardItem *itemType = new QStandardItem(fileType.typeName());
            setItem(row, QUploadFileTableWidget::ColumnIndexTypeName, itemType);
        }
    }

    adjustPageCount();
}
