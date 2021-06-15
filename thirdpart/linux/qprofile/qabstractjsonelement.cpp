
#include "qabstractjsonelement.h"
#include <QMetaProperty>
#include <QFile>

#if defined(BUILD_X86)
#include "qjson/serializer.h"
#include "qjson/parser.h"
#include "qjson/qobjecthelper.h"
#else
#include <QJson/Serializer>
#include <QJson/Parser>
#include <QJson/QObjectHelper>
#endif

using namespace QProfile;

const QStringList QAbstractJsonElement::propertyList()
{
    QStringList list;

    for (int propertyIdx = 0;
         propertyIdx < QAbstractJsonElement::staticMetaObject.propertyCount();
         propertyIdx++) {
        QMetaProperty property = QAbstractJsonElement::staticMetaObject.property(
                                     propertyIdx);
        list.append(property.name());
    }

    return list;
}

const QStringList QAbstractJsonElement::ignoredProperties() const
{
    return propertyList();
}

QAbstractJsonElement::QAbstractJsonElement(QObject *parent)
    : QObject(parent)
{
}

QAbstractJsonElement::QAbstractJsonElement(const QAbstractJsonElement &element)
    : QObject(element.parent())
{
    for (int propertyIdx = 0; propertyIdx < metaObject()->propertyCount();
         propertyIdx++) {
        QMetaProperty property = metaObject()->property(propertyIdx);
        int srcIdx = element.metaObject()->indexOfProperty(property.name());

        if (srcIdx > 0) {
            if (!setProperty(property.name(), element.property(property.name())))
                qprofileDebug(QtDebugMsg) << "set property " << property.name() <<
                                          "Failed, for class " << metaObject()->className();
        } else {
            qprofileDebug(QtDebugMsg) << "Miss property "  << property.name() <<
                                      ", for class" << metaObject()->className();
        }
    }
}

void QAbstractJsonElement::fromVariant(const QVariant &variant)
{
    QJson::QObjectHelper::qvariant2qobject(variant.toMap(), this);
}

const QVariant QAbstractJsonElement::toVariant(const QStringList &ignoreList)
const
{
    QStringList list(ignoreList);
    list << ignoredProperties();
    return QVariant(QJson::QObjectHelper::qobject2qvariant(this, list));
}

const QByteArray QAbstractJsonElement::toJson(const QStringList &ignoreList)
const
{
    bool ok = false;
    QJson::Serializer serializer;
    QVariantMap variantMap = toVariant(ignoreList).toMap();
    QByteArray json = serializer.serialize(variantMap, &ok);
    return json;
}

QAbstractJsonElement &QAbstractJsonElement::operator=(const QAbstractJsonElement
        &other)
{
    for (int propertyIdx = 0; propertyIdx < metaObject()->propertyCount();
         propertyIdx++) {
        QMetaProperty property = metaObject()->property(propertyIdx);
        int srcIdx = other.metaObject()->indexOfProperty(property.name());

        if (srcIdx > 0) {
            if (!setProperty(property.name(), other.property(property.name())))
                qprofileDebug(QtDebugMsg) << "set property " << property.name() <<
                                          "Failed, for class " <<
                                          metaObject()->className();
        } else {
            qprofileDebug(QtDebugMsg) << "Miss property "  << property.name() <<
                                      ", for class" <<
                                      metaObject()->className();
        }
    }

    return *this;
}

bool QAbstractJsonElement::fromFile(const QString &pathName)
{
    QFile file(pathName);

    if (file.exists()) {
        if (file.open(QIODevice::ReadOnly)) {
            QByteArray data = file.readAll();
            QJson::Parser parser;
            bool ok = false;
            QVariant variant = parser.parse(data, &ok);

            if (!ok) {
                qprofileDebug(QtWarningMsg) << "Failed to load " << pathName;
                return false;
            }

            QJson::QObjectHelper::qvariant2qobject(variant.toMap(), this);
            file.close();
            return true;
        }
    } else {
        qprofileDebug(QtWarningMsg) << "File missing, file= " << pathName;
    }

    return false;
}

bool QAbstractJsonElement::toFile(const QString &pathName,
                                  const QStringList &ignoreList) const
{
    QFile file(pathName);

    if (file.exists()) {
        file.remove();
    }

    if (!file.open(QIODevice::WriteOnly)) {
        qprofileDebug(QtWarningMsg) << "Failed to open file:" << pathName;
        return false;
    }

    QByteArray array = toJson(ignoreList);
    qprofileDebug(QtDebugMsg) << array;

    if (array.size() != file.write(array)) {
        qprofileDebug(QtWarningMsg) << "Failed to write file:"  << pathName;
        file.remove();
        return false;
    }

#ifdef BUILD_X86

    if (!file.setPermissions(QFile::ReadOwner | QFile::WriteOwner | QFile::ReadUser
                             | QFile::WriteUser | QFile::ReadOther | QFile::WriteOther | QFile::WriteGroup |
                             QFile::ReadGroup | QFile::ExeOwner | QFile::ExeUser | QFile::ExeGroup |
                             QFile::ExeOther)) {
        qprofileDebug(QtDebugMsg) << "Failed to set permission for file " <<
                                  pathName;
    }

#endif
    file.close();
    return true;
}

const QVariant QAbstractJsonElement::GetProperty(const QVariant variant ,
        const QString &propertyName)
{
    QVariantMap map = variant.toMap();
    return map[propertyName];
}

QAbstractJsonElement::~QAbstractJsonElement()
{
}

