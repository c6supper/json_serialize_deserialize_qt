
#include "qvconnectresultmodel.h"
#include "qobjectfactory.h"
#include "qabstractcommand.h"

using namespace QProfile;
using namespace QRserver;

QObjectSelfRegisteration<QVConnectResultModel> registerQVConnectResultModel;

const QString QVConnectResultModel::CommandGetResultList = "getResultList";

QVConnectResultModel::QVConnectResultModel(QObject *parent) : QAbstractModel(
        parent),
    m_mutex(new QMutex())
{
    Q_ASSERT(parent != nullptr);
}

void QVConnectResultModel::initAttributes()
{
}

const QVariantList QVConnectResultModel::resultModelFileList() const
{
    QMutexLocker locker(m_mutex);
    QVariantList resultList;
    QList<QResultModel *>::const_iterator i;

    for (i = m_resultModelList.constBegin(); i != m_resultModelList.constEnd();
         ++i) {
        resultList.append((*i)->toVariant());
    }

    return resultList;
}

void QVConnectResultModel::setResultModelFileList(const QVariantList
        &resultModelFileList)
{
    if (resultModelFileList == this->resultModelFileList()) {
        qprofileDebug(QtDebugMsg) << "resultModelList is the same !";
        return ;
    }

    QMutexLocker locker(m_mutex);

    while (!m_resultModelList.isEmpty())
        delete m_resultModelList.takeFirst();

    QList<QVariant>::const_iterator i;

    for (i = resultModelFileList.constBegin(); i != resultModelFileList.constEnd();
         ++i) {
        QResultModel *result = new QResultModel(this);
        result->blockNotify(true);
        result->fromVariant((*i).toMap());
        result->blockNotify(false);
        m_resultModelList.append(result);
    }

    asynPropertyChangeNotify();
}

void QVConnectResultModel::clear()
{
    QMutexLocker locker(m_mutex);

    while (!m_resultModelList.isEmpty())
        delete m_resultModelList.takeFirst();
}

QVConnectResultModel::~QVConnectResultModel()
{
    clear();
    delete m_mutex;
}

