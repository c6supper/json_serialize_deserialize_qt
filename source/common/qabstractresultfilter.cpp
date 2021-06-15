/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include <QMetaObject>
#include "qprofile_debug.h"
#include "qabstractresultfilter.h"
#include "qmodelfactory.h"
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include "qrserverprofile.h"

using namespace QRserver;
using namespace QProfile;

QAbstractResultFilter::QAbstractResultFilter(const QString &measureMode,
        const QString &measureType, QObject *parent)
    : QProfile::QAbstractJsonElement(parent), m_measureMode(measureMode),
      m_measureType(measureType)
{
    Q_ASSERT(parent != nullptr);
    /*    if (!QMetaObject::invokeMethod(parent, "registerFilter", Qt::DirectConnection,
                                       Q_ARG(QAbstractResultFilter *, this))) {
            qprofileDebug(QtFatalMsg) << "Failed to register filter!";
        }*/
    m_rserverModel = QModelFactory::constModel<QRserverModel>(this);
}

QAbstractResultFilter::~QAbstractResultFilter()
{
}

const QString QAbstractResultFilter::measureType() const
{
    return m_measureType;
}
void QAbstractResultFilter::setMeasureType(QString measureType)
{
    m_measureType = measureType;
}

const QString QAbstractResultFilter::measureMode() const
{
    return m_measureMode;
}
void QAbstractResultFilter::setMeasureMode(QString measureMode)
{
    m_measureMode = measureMode;
}

void QAbstractResultFilter::patchRelatedFiles(const QString &absoluteXmlPath,
        const QDateTime &saveDateTime)
{
    patchXmlFile(absoluteXmlPath, saveDateTime);
    patchSignature(absoluteXmlPath);
}

void QAbstractResultFilter::patchXmlFile(const QString &absoluteXmlPath,
        const QDateTime &saveDateTime)
{
    QFileInfo xmlFileInfo(absoluteXmlPath);

    if (xmlFileInfo.exists()) {
        QFile xmlFile(absoluteXmlPath);
        QDomDocument doc;

        do {
            if (!xmlFile.open(QIODevice::ReadWrite | QIODevice::Text)) {
                qprofileDebug(QtWarningMsg) << "Failed to create xml " << absoluteXmlPath;
                break;
            }

            QString errorMsg;
            int errorLine, errorColumn;

            if (!doc.setContent(&xmlFile, &errorMsg, &errorLine, &errorColumn)) {
                qprofileDebug(QtDebugMsg) << "Failed to set xml content " << errorMsg <<
                                          errorLine << errorColumn;
                break;
            }

            QDomElement data = doc.firstChildElement("data");

            if (data.isNull()) {
                break;
            }

            QDomElement oem = data.firstChildElement("v300_oeminfo");

            if (!isValidOem(oem)) {
                break;
            }

            if (!oem.hasAttribute("save_date")) {
                oem.setAttribute("save_date", saveDateTime.toString("yyyy-MM-dd:hh-mm-ss"));
                xmlFile.resize(0);
                QTextStream stream(&xmlFile);
                stream << doc.toString();
            }

            xmlFile.close();
            return;
        } while (0);

        xmlFile.remove();
    }

    createXml(absoluteXmlPath, saveDateTime);
}

bool QAbstractResultFilter::isValidOem(QDomElement &oem)
{
    if (oem.isNull()) {
        return false;
    }

    return ((oem.hasAttribute("save_date") || oem.hasAttribute("save_time"))
            && oem.hasAttribute("serial_number") && oem.hasAttribute("software_version"));
}

QDomElement QAbstractResultFilter::createOemInfo(QDomDocument &doc,
        const QDateTime &saveDateTime)
{
    QDomElement oem = doc.createElement("v300_oeminfo");
    QDeviceInfo chassisInfo;
    chassisInfo.fromVariant(m_rserverModel->getChassis());
    qprofileDebug(QtDebugMsg) << "chassisInfo : " << chassisInfo.toVariant();

    if (!chassisInfo.serialNumber().isEmpty()) {
        oem.setAttribute("save_time", saveDateTime.toString("yyyy-MM-dd:hh-mm-ss"));
        oem.setAttribute("save_date", saveDateTime.toString("yyyy-MM-dd:hh-mm-ss"));
        oem.setAttribute("serial_number", chassisInfo.serialNumber());
        oem.setAttribute("mac_address", chassisInfo.mac());
        oem.setAttribute("software_version", chassisInfo.version());
    }

    return oem;
}

void QAbstractResultFilter::createXml(const QString &absoluteXmlPath,
                                      const QDateTime &saveDateTime)
{
    QFile xmlFile(absoluteXmlPath);
    QDomDocument doc;

    if (!xmlFile.open(QIODevice::Truncate | QIODevice::ReadWrite |
                      QIODevice::Text)) {
        qprofileDebug(QtWarningMsg) << "Failed to create xml " << absoluteXmlPath;
        return;
    }

    QDomProcessingInstruction instruction = doc.createProcessingInstruction("xml",
                                            "version=\"1.0\"");
    doc.appendChild(instruction);
    QDomElement data = doc.createElement("data");
    QString testtype = m_measureMode;

    if (!m_measureType.isEmpty())
        testtype += " " + m_measureType;

    data.setAttribute("type", testtype);
    data.setAttribute("version", "2.0");
    data.appendChild(createOemInfo(doc, saveDateTime));

    if (isNeedPatchAdvInfoFromHtmlToXml()) {
        try {
            data.appendChild(patchAdvInfoFromHtmlToXml(doc, absoluteXmlPath));
        } catch (...) {
            qprofileDebug(QtWarningMsg)
                    << "Exception occurred when patching advance info";
        }
    }

    doc.appendChild(data);
    QTextStream stream(&xmlFile);
    //    qprofileDebug(QtDebugMsg) << "doc.toString() : " << doc.toString();
    stream << doc.toString();
#ifdef BUILD_X86

    if (!xmlFile.setPermissions(QFile::ReadOwner | QFile::WriteOwner |
                                QFile::ReadUser
                                | QFile::WriteUser | QFile::ReadOther | QFile::WriteOther | QFile::WriteGroup |
                                QFile::ReadGroup | QFile::ExeOwner | QFile::ExeUser | QFile::ExeGroup |
                                QFile::ExeOther)) {
        qprofileDebug(QtDebugMsg) << "Failed to set permission for file " <<
                                  absoluteXmlPath;
    }

#endif
    xmlFile.close();
}

bool QAbstractResultFilter::matched(const QString &measureMode,
                                    const QString &measureType)
{
    if (this->measureMode() == measureMode && (this->measureType() == measureType
            || this->measureType().isEmpty())) {
        return true;
    }

    return false;
}

const QString QAbstractResultFilter::getDisplayName(const QString
        &absoluteFilePath)
{
    return QString();
}

const bool QAbstractResultFilter::postUploading(const QString &absoluteFilePath,
        const quint32 commandStatus)
{
    return true;
}

void QAbstractResultFilter::patchSignature(const QString &absoluteXmlPath)
{
    QFileInfo xmlFileInfo(absoluteXmlPath);
    QFileInfo htmlFileInfo(QString("%1.html").arg(xmlFileInfo.absolutePath() + "/" +
                           xmlFileInfo.baseName()));

    //is Contains Signture
    if (containsSignature(htmlFileInfo.absoluteFilePath())) {
        return;
    }

    //xml
    QString strJobId = getJobIdFromXml(xmlFileInfo.absoluteFilePath());

    //html
    if (strJobId.isNull() || strJobId.isEmpty()) {
        strJobId = getJobIdFromHtml(htmlFileInfo.absoluteFilePath());
    }

    if (strJobId.isNull() || strJobId.isEmpty()) {
        return;
    }

    //add Signature
    if (!addSignatureToHtml(strJobId, htmlFileInfo.absoluteFilePath())) {
        qprofileDebug(QtDebugMsg) << "addSignatureToHtml failed." << strJobId <<
                                  htmlFileInfo.absoluteFilePath();
    }
}

bool QAbstractResultFilter::containsSignature(const QString &absoluteHtmlPath)
{
    QFile file(absoluteHtmlPath);

    if (!file.exists())
        return false;

    if (!file.open(QIODevice::ReadOnly)) {
        qprofileDebug(QtDebugMsg) << "Unable to open file, errorString = " <<
                                  file.errorString();
        return false;
    }

    QString strFileData = QString(file.readAll());
    file.close();

    if (strFileData.contains("Signature")
        && strFileData.contains("_files/sig-")) {
        return true;
    }

    return false;
}

// TODO: v300_advinfo/location/job_id <hardcoding>
QString QAbstractResultFilter::getLocationFromXml(const QString
        &absoluteXmlPath)
{
    QString strLocation;
    QFile xmlFile(absoluteXmlPath);

    if (xmlFile.exists()) {
        QDomDocument doc;

        do {
            if (!xmlFile.open(QIODevice::ReadOnly)) {
                qprofileDebug(QtDebugMsg) << "Unable to open file, errorString = " <<
                                          xmlFile.errorString();
                break;
            }

            QString errorMsg;
            int errorLine, errorColumn;

            if (!doc.setContent(&xmlFile, &errorMsg, &errorLine, &errorColumn)) {
                qprofileDebug(QtDebugMsg) << "Failed to set xml content " << errorMsg <<
                                          errorLine << errorColumn;
                xmlFile.close();
                break;
            }

            xmlFile.close();
            QDomElement data = doc.firstChildElement("data");

            if (data.isNull()) {
                break;
            }

            QDomElement oem = data.firstChildElement("v300_advinfo");

            if (oem.isNull()) {
                break;
            }

            if (!oem.hasAttribute("location")) {
                break;
            }

            strLocation = oem.attribute("location");
        } while (0);
    }

    return strLocation.trimmed();
}

QString QAbstractResultFilter::getJobIdFromXml(const QString &absoluteXmlPath)
{
    QString strJobId;
    QFile xmlFile(absoluteXmlPath);

    if (xmlFile.exists()) {
        QDomDocument doc;

        do {
            if (!xmlFile.open(QIODevice::ReadOnly)) {
                qprofileDebug(QtDebugMsg) << "Unable to open file, errorString = " <<
                                          xmlFile.errorString();
                break;
            }

            QString errorMsg;
            int errorLine, errorColumn;

            if (!doc.setContent(&xmlFile, &errorMsg, &errorLine, &errorColumn)) {
                qprofileDebug(QtDebugMsg) << "Failed to set xml content " << errorMsg <<
                                          errorLine << errorColumn;
                xmlFile.close();
                break;
            }

            xmlFile.close();
            QDomElement data = doc.firstChildElement("data");

            if (data.isNull()) {
                break;
            }

            QDomElement oem = data.firstChildElement("v300_advinfo");

            if (oem.isNull()) {
                break;
            }

            if (!oem.hasAttribute("job_id")) {
                break;
            }

            strJobId = oem.attribute("job_id");
        } while (0);
    }

    return strJobId.trimmed();
}

QString QAbstractResultFilter::getJobIdFromHtml(const QString &absoluteHtmlPath)
{
    /* html ex.: "<tr> <font size="3"> <td> Current Job ID </td> <td> 123 </td> </font> </tr>" */
    QString strJobId;
    QFile htmlFile(absoluteHtmlPath);

    if (htmlFile.exists()) {
        do {
            if (!htmlFile.open(QIODevice::ReadOnly)) {
                qprofileDebug(QtDebugMsg) << "Unable to open file, errorString = " <<
                                          htmlFile.errorString();
                break;
            }

            QString strFileData = QString(htmlFile.readAll());
            htmlFile.close();
            int index = strFileData.indexOf("Current Job ID");

            if (index == -1) {
                break;
            }

            /* 20: sizeof("Current Job ID </td>") */
            int indexStart = strFileData.indexOf("<td>", index + 20);

            if (indexStart == -1) {
                break;
            }

            int indexEnd = strFileData.indexOf("</td>", index + 20);

            if (indexEnd == -1) {
                break;
            }

            /* 4: sizeof("<td>") */
            strJobId = strFileData.mid(indexStart + 4, indexEnd - indexStart - 4);
        } while (0);
    }

    return strJobId.trimmed();
}

bool QAbstractResultFilter::addSignatureToHtml(const QString &jobId,
        const QString &absoluteHtmlPath)
{
    QFile htmlFile(absoluteHtmlPath);

    if (htmlFile.exists()) {
        if (!htmlFile.open(QIODevice::ReadWrite)) {
            qprofileDebug(QtDebugMsg) << "Unable to open file, errorString = " <<
                                      htmlFile.errorString();
            return false;
        }

        QTextStream htmlStream(&htmlFile);
        QString strFileData = htmlStream.readAll();
        int indexInsert = strFileData.indexOf("</body>");

        if (indexInsert == -1) {
            qprofileDebug(QtDebugMsg) << "Cannot find </body> node!";
            return false;
        }

        //copy signature file
        QFile sigSourceFile(QString("%1/%2.png").arg(
                                QRserverProfile::getResultSignaturePath()).arg(jobId));

        if (!sigSourceFile.exists()) {
            return false;
        }

        QFileInfo htmlFileInfo(absoluteHtmlPath);
        QFileInfo sigDestFileInfo(QString("%1/%2_files/sig-%3.png").arg(
                                      htmlFileInfo.absoluteDir().absolutePath()).arg(
                                      htmlFileInfo.completeBaseName()).arg((int)random()));

        if (!sigDestFileInfo.absoluteDir().exists()) {
            htmlFileInfo.absoluteDir().mkdir(sigDestFileInfo.absoluteDir().absolutePath());
        }

        sigSourceFile.copy(sigDestFileInfo.absoluteFilePath());
        QString strInsert =
            QString("<br /><br> <font size=\"3\"> Signature </font> </br>\n<br> <img src=\"%1_files/%2\" /> </br>\n").arg(
                htmlFileInfo.completeBaseName()).arg(sigDestFileInfo.fileName());
        strFileData.insert(indexInsert, strInsert);
        htmlStream.seek(0);
        htmlStream << strFileData;
        htmlFile.close();
        return true;
    }

    return false;
}

bool QAbstractResultFilter::isNeedPatchAdvInfoFromHtmlToXml()
{
    return false;
}

QDomElement QAbstractResultFilter::patchAdvInfoFromHtmlToXml(QDomDocument &doc,
        const QString &absoluteXmlPath)
{
    QDomElement aNullNode;
    return aNullNode;
}

const QStringList QAbstractResultFilter::getFileListNeedToBeDeleted(
    const QString absoluteKeyPath)
{
    return getRelatedFileFolderList(absoluteKeyPath);
}
