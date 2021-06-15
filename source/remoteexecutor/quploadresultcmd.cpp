
#include "quploadresultcmd.h"
#include "qcmdfactory.h"
#include "quploadcommand.h"
#include "qfilemodel.h"
#include <QFileInfo>
#include "qrserverresult.h"

using namespace QProfile;
using namespace QRserver;

QCMDSelfRegisteration<QUploadResultCMD>
registerQUploadResultCMD("QUploadResultCMD");

QUploadResultCMD::QUploadResultCMD(QObject *parent)
    : QAbstractRemoteCommand(parent), m_fileModel(nullptr)
{
}

QUploadResultCMD::QUploadResultCMD(const QUploadResultCMD &cmd)
    : QAbstractRemoteCommand(cmd)
{
}

QUploadResultCMD::~QUploadResultCMD()
{
    if (m_fileModel != nullptr) {
        m_fileModel->deleteLater();
    }

    m_fileModel = nullptr;
}

const QVariant QUploadResultCMD::parseExecOutput(const QVariant &output)
{
    return output;
}

bool QUploadResultCMD::isValid()
{
    return (m_fileModel != nullptr) && QAbstractRemoteCommand::isValid();
}

void QUploadResultCMD::setInput(QString input)
{
    QFileInfo file(input);

    if (m_fileModel != nullptr) {
        m_fileModel->deleteLater();
    }

    m_fileModel = nullptr;

    if (file.exists()) {
        QVariant fileType = QRserverResult::GetFileType(
                                file.absoluteFilePath());

        if (fileType.isValid()) {
            m_fileModel = new QFileModel(this);

            if (m_fileModel == nullptr) {
                qprofileDebug(QtCriticalMsg) << "Failed to allocate memory!";
                return QAbstractRemoteCommand::setInput(input);
            }

            m_fileModel->fromVariant(fileType);

            if (m_fileModel->displayName().isEmpty()) {
                m_fileModel->setDisplayName(file.completeBaseName());
            }

            m_fileModel->setFileName(file.completeBaseName());
            m_fileModel->setPathName(file.absoluteFilePath());
            m_fileModel->setDateTime(file.lastModified());
        }
    }

    QAbstractRemoteCommand::setInput(input);
}

const QVariant QUploadResultCMD::toExecInput()
{
    QVariantList fileModelList;

    if (m_fileModel != nullptr) {
        fileModelList.append(m_fileModel->toVariant());
    }

    QUploadCommand uploadCommand(this, method(), QVariant(fileModelList));
    uploadCommand.setTopic(inTopic());
    return uploadCommand.toVariant();
}
