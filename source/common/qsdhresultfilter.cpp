/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qsdhresultfilter.h"
#include <QFileInfo>

using namespace QRserver;

QSdhResultFilter::QSdhResultFilter(QObject *parent)
    : QEtherResultFilter(parent)
{
    setMeasureMode("OTN/SDH/SONET/PDH/Dsn");
}

bool QSdhResultFilter::isNeedPatchAdvInfoFromHtmlToXml()
{
    return true;
}

QDomElement QSdhResultFilter::patchAdvInfoFromHtmlToXml(QDomDocument &doc,
        const QString &absoluteXmlPath)
{
    QDomElement advInfo;//Constructs a null node.
    QFileInfo xmlFileInfo(absoluteXmlPath);
    QFileInfo htmlFileInfo(QString("%1.html").arg(xmlFileInfo.absolutePath() + "/" +
                           xmlFileInfo.baseName()));
    QFile file(htmlFileInfo.absoluteFilePath());

    if (!file.exists())
        return advInfo;

    if (!file.open(QIODevice::ReadOnly)) {
        qprofileDebug(QtDebugMsg) << "Unable to open file, errorString = " <<
                                  file.errorString();
        return advInfo;
    }

    QString strFileData = QString(file.readAll());
    file.close();

    if (strFileData.contains("Job Information")) {
        QMap<QString, QString> mapOfKeysFromHtmlToXml;
        mapOfKeysFromHtmlToXml["Current Job ID"] = "job_id";
        mapOfKeysFromHtmlToXml["Current NODE ID"] = "node_id";
        mapOfKeysFromHtmlToXml["Current Location"] = "location";
        mapOfKeysFromHtmlToXml["Comments"] = "comment";
        mapOfKeysFromHtmlToXml["Locator"] = "locator";
        mapOfKeysFromHtmlToXml["Type"] = "advtype";
        mapOfKeysFromHtmlToXml["Current Account"] = "account_id";
        mapOfKeysFromHtmlToXml["Tech ID"] = "tech_id";
        mapOfKeysFromHtmlToXml["Tech Name"] = "tech_name";
        QMap<QString, QString>::const_iterator i;
        QMap<QString, QString> advInfoMap;

        for (i = mapOfKeysFromHtmlToXml.constBegin();
             i != mapOfKeysFromHtmlToXml.constEnd(); ++i) {
            QRegExp advElementRegEx((i.key() + " </td> <td> (.*) </td>"),
                                    Qt::CaseInsensitive);
            advElementRegEx.setMinimal(true);

            if (-1 == advElementRegEx.indexIn(strFileData)) {
                advInfoMap[i.value()] = "";
            } else {
                advInfoMap[i.value()] = advElementRegEx.cap(1);
            }
        }

        advInfo = doc.createElement("v300_advinfo");

        for (i = advInfoMap.constBegin(); i != advInfoMap.constEnd(); ++i) {
            advInfo.setAttribute(i.key(), i.value());
        }
    }

    return advInfo;
}

const QString QSdhResultFilter::xmlFileName(const QFileInfo keyInfo)
{
    return keyInfo.completeBaseName() + ".xml";
}

QSdhResultFilter::~QSdhResultFilter()
{
}
