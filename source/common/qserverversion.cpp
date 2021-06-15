/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */


#include "qserverversion.h"

using namespace QRserver;

const QRegExp
QServerVersion::m_versionRegExp("^(\\d+)\\.(\\d)\\.(\\d)\\.(\\d+)$");

QServerVersion::QServerVersion(const QString &version, QObject *parent)
    : QAbstractJsonElement(parent), m_major(-1), m_minor(-1), m_patch(-1),
      m_build(-1)
{
    if (m_versionRegExp.indexIn(version) > -1) {
        QStringList capturedTexts = m_versionRegExp.capturedTexts();

        if (capturedTexts.count() > 4) {
            int idx = 1;
            m_major = capturedTexts.at(idx++).toInt();
            m_minor = capturedTexts.at(idx++).toInt();
            m_patch = capturedTexts.at(idx++).toInt();
            m_build = capturedTexts.at(idx).toInt();
            return;
        }
    }

    qprofileDebug(QtFatalMsg) << "Get invalid format R-Server version " <<
                              version;
}

bool QServerVersion::isValid() const
{
    return (m_major > -1 && m_minor > -1 && m_patch > -1 && m_build > -1);
}

const QString QServerVersion::versionString() const
{
    if (isValid()) {
        return QString("%1.%2.%3.%4").arg(m_major).arg(m_minor).arg(m_patch).arg(
                   m_build);
    }

    return "";
}

const qint32 QServerVersion::version() const
{
    if (isValid()) {
        return (m_patch + 10 * m_minor + 100 * m_major);
    }

    return invalidVersionIntger;
}
