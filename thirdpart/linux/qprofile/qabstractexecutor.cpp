
#include "qabstractexecutor.h"
#include <QMetaProperty>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include "qabstractremotecommand.h"

using namespace QProfile;

QAbstractExecutor::QAbstractExecutor(QObject *parent)
    : QAbstractJsonElement(parent)
{
}

QAbstractExecutor::QAbstractExecutor(const QAbstractExecutor &executor)
    : QAbstractJsonElement(executor.parent())
{
    for (int propertyIdx = 0; propertyIdx < metaObject()->propertyCount();
         propertyIdx++) {
        QMetaProperty property = metaObject()->property(propertyIdx);
        int srcIdx = executor.metaObject()->indexOfProperty(property.name());

        if (srcIdx > 0) {
            if (!setProperty(property.name(), executor.property(property.name())))
                qprofileDebug(QtDebugMsg) << "set property " << property.name() <<
                                          "Failed, for class " << metaObject()->className();
        } else {
            qprofileDebug(QtDebugMsg) << "Miss property "  << property.name() <<
                                      ", for class" << metaObject()->className();
        }
    }
}

void QAbstractExecutor::pureVirtualMock()
{
}

bool QAbstractExecutor::saveFile(const QString &pathName,
                                 const QByteArray &json)
{
    QFile file(pathName);
    QFileInfo fileInfo(pathName);
    QDir dir(fileInfo.absolutePath());

    if (!dir.exists()) {
        if (!dir.mkpath(fileInfo.absolutePath())) {
            qprofileDebug(QtWarningMsg) << "Failed to create dir " <<
                                        fileInfo.absolutePath();
            return false;
        }
    }

    if (file.exists()) {
        file.remove();
    }

    if (!file.open(QIODevice::WriteOnly)) {
        qprofileDebug(QtWarningMsg) << "Failed to open file:" << pathName;
        return false;
    }

    if (json.size() != file.write(json)) {
        qprofileDebug(QtWarningMsg) << "Failed to write file:"  << pathName;
        file.remove();
        return false;
    }

    file.close();
    return true;
}

QAbstractExecutor::~QAbstractExecutor()
{
}

