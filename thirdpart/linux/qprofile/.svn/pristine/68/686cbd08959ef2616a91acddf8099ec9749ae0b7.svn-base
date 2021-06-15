/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#include "qxmlparser.h"
#include "qprofile_debug.h"
#include <QDomDocument>
#include <QString>

using namespace QProfile;
QXmlParser::QXmlParser(QObject *parent)
    : QAbstractParser(parent)
{
}

QVariant QXmlParser::parse(const QByteArray &qProfileString, bool *ok)
{
    QDomDocument doc;
    m_error = !doc.setContent(qProfileString, true, &m_errorMsg, &m_errorLine,
                              &m_errorColumn);

    if (m_error)
        qprofileDebug(QtDebugMsg) << "Failed to parse xml string, " << m_errorMsg;

    if (ok != nullptr)
        *ok = !m_error;

    QVariantMap resultMap;

    if (!m_error) {
        QDomElement root = doc.documentElement();

        if (root.hasAttributes()) {
            for (int i = 0; i < root.attributes().count(); i++) {
                QDomNode childNode = root.attributes().item(i);

                if (childNode.nodeType() == QDomNode::AttributeNode)
                    resultMap[childNode.toAttr().name()] = childNode.toAttr().value();
            }
        }
    }

    return QVariant::fromValue(resultMap);
}
