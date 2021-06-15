#include "qscreenshotresultfilter.h"
#include "qprofile_debug.h"
#include <QFileInfo>
#include <QDir>

using namespace QRserver;

const QString QScreenshotResultFilter::HtmlImageFolderKey = "_files";

QScreenshotResultFilter::QScreenshotResultFilter(QObject *parent)
    : QAbstractResultFilter("Common", "", parent)
{
}

const QString QScreenshotResultFilter::keyFilePattern()
{
    return "xxxx.png|xxxx.jpg|xxxx.bmp";
}

const QString QScreenshotResultFilter::filterPattern()
{
    return "\\.png|\\.jpg|\\.bmp";
}

bool QScreenshotResultFilter::isKey(const QString absolutePath)
{
    QFileInfo fileInfo(absolutePath);

    if (fileInfo.exists() && fileInfo.isFile()) {
        return (fileInfo.suffix() == "png" || fileInfo.suffix() == "jpg"
                || fileInfo.suffix() == "bmp");
    }

    return false;
}

const QString QScreenshotResultFilter::getDisplayName(const QString
        &absoluteFilePath)
{
    QString strDisplayName;
    QFileInfo keyInfo(absoluteFilePath);

    if (keyInfo.dir().dirName().contains(HtmlImageFolderKey)) {
        strDisplayName = keyInfo.dir().dirName().remove(HtmlImageFolderKey);
    } else {
        strDisplayName = keyInfo.baseName();
    }

    return strDisplayName;
}

const QStringList QScreenshotResultFilter::getRelatedFileFolderList(
    const QString absoluteKeyPath)
{
    QStringList relatedList;

    if (isKey(absoluteKeyPath)) {
        //png/jpg/bmp
        relatedList << absoluteKeyPath;
        QFileInfo keyInfo(absoluteKeyPath);
        QString xmlFilePath(keyInfo.absoluteDir().absolutePath() + "/" +
                            keyInfo.completeBaseName());

        if (keyInfo.dir().dirName().contains(HtmlImageFolderKey)) {
            int idx = xmlFilePath.lastIndexOf(HtmlImageFolderKey);
            xmlFilePath = xmlFilePath.left(idx);
        }

        //xml
        QFileInfo xmlFileInfo(xmlFilePath);
        //html
        QFileInfo htmlFileInfo(QString("%1.html").arg(xmlFileInfo.absolutePath() + "/" +
                               xmlFileInfo.baseName()));
        patchHtmlFile(htmlFileInfo.absoluteFilePath(), absoluteKeyPath);
        relatedList << htmlFileInfo.absoluteFilePath();
        //
        patchRelatedFiles(xmlFileInfo.absoluteFilePath(), keyInfo.lastModified());
        relatedList << xmlFileInfo.absoluteFilePath();
        qprofileDebug(QtDebugMsg) << relatedList;
    }

    return relatedList;
}

const bool QScreenshotResultFilter::postUploading(const QString
        &absoluteFilePath,
        const quint32 commandStatus)
{
    return true;
}

void QScreenshotResultFilter::patchHtmlFile(const QString &absoluteHtmlPath,
        const QString &absoluteKeyPath)
{
    QFile htmlFile(absoluteHtmlPath);

    if (htmlFile.exists()) {
        return;
    }

    QFileInfo htmlInfo(absoluteHtmlPath);
    QFileInfo picInfo(absoluteKeyPath);
    QString strImagePath = QString("%1_files/%2").arg(
                               htmlInfo.completeBaseName()).arg(picInfo.fileName());
    htmlFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QString strContent;
    strContent.append("<html lang='en'>");
    strContent.append("<head>");
    strContent.append("\t<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">");
    strContent.append("\t<title>Screen results</title>");
    strContent.append("</head>");
    strContent.append("<body>");
    strContent.append("<h1><a>Screen</a></h1>");
    strContent.append(QString("<br><img src=\"%1\"/> </br>").arg(strImagePath));
    strContent.append("</body>");
    strContent.append("</html>");
    htmlFile.write(strContent.toStdString().c_str());
    htmlFile.flush();
    htmlFile.close();
}

QScreenshotResultFilter::~QScreenshotResultFilter()
{
}
