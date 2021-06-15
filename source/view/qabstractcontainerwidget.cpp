#include "qabstractcontainerwidget.h"
#if defined(BUILD_X86)
#include "qjson/parser.h"
#include "qjson/serializer.h"
#include "qjson/qobjecthelper.h"
#else
#include <QJson/Parser>
#include <QJson/Serializer>
#include <QJson/QObjectHelper>
#endif
#include <QTimer>
#include <QMetaProperty>
#include "qgenerictopicevent.h"
#include "qabstractcompatibility.h"

using namespace QJson;
using namespace QRserver;

const QStringList QAbstractContainerWidget::ignoredPropertyList =
    IgnoredPropertyList();

const QStringList QAbstractContainerWidget::IgnoredPropertyList()
{
    QStringList ignoredPropertyList;

    for (int propertyIdx = 0;
         propertyIdx < QWidget::staticMetaObject.propertyCount(); propertyIdx++) {
        QMetaProperty property = QWidget::staticMetaObject.property(propertyIdx);
        ignoredPropertyList.append(property.name());
    }

    return ignoredPropertyList;
}

QAbstractContainerWidget::QAbstractContainerWidget(QWidget *parent,
        bool isRetainMessage)
    : QWidget(parent), m_ipcClient(NULL), m_topic(NULL), m_networkChecker(nullptr),
      m_binder(nullptr), m_isRetainMessage(isRetainMessage), m_backProgress(nullptr)
{
    QTimer::singleShot(0, this, SLOT(postponeInit()));
    m_ipcClient = QAbstractIpcClient::Instance();

    if (m_ipcClient != nullptr) {
        connect(m_ipcClient, SIGNAL(connected()), this, SLOT(onIpcConnected()));
        connect(m_ipcClient, SIGNAL(disconnected()), this, SLOT(onIpcDisconnected()));
    }

    m_networkChecker = QRserverNetworkChecker::Instance();
    connect(m_networkChecker, SIGNAL(networkError()), this, SLOT(onNetworkError()));
}

bool QAbstractContainerWidget::isTopWidgetVisible()
{
    QWidget *parent = dynamic_cast<QWidget *>(this);

    while (parent  != nullptr) {
        QAbstractContainerWidget *containerWidget =
            dynamic_cast<QAbstractContainerWidget *>(parent);

        if (containerWidget != nullptr && containerWidget->isTopWidget()) {
            return containerWidget->isVisible();
        }

        parent = dynamic_cast<QWidget *>(parent->parent());
    }

    return false;
}

bool QAbstractContainerWidget::isTopWidget()
{
    return false;
}

bool QAbstractContainerWidget::isCompatible(const QString feature)
{
    QAbstractCompatibility *compatibility = QAbstractCompatibility::Instance();

    if (compatibility != nullptr) {
        return compatibility->isCompatible(feature);
    }

    return false;
}

void QAbstractContainerWidget::onIpcDisconnected()
{
    setDisabled(true);
}

void QAbstractContainerWidget::onIpcConnected()
{
    setDisabled(false);
}

void QAbstractContainerWidget::onNetworkError()
{
}

QRserverNetworkChecker *QAbstractContainerWidget::getNetworkChecker()
{
    return m_networkChecker;
}

void QAbstractContainerWidget::postponeInit()
{
    if (m_ipcClient == nullptr) {
        m_ipcClient = QAbstractIpcClient::Instance();

        if (m_ipcClient != nullptr) {
            connect(m_ipcClient, SIGNAL(connected()), this, SLOT(onIpcConnected()));
            connect(m_ipcClient, SIGNAL(disconnected()), this, SLOT(onIpcDisconnected()));
        } else {
            qprofileDebug(QtWarningMsg) << "m_ipcClient is null! ";
        }
    }

    allocBackProgress();
    allocModel();
    allocTopic();
}

void QAbstractContainerWidget::allocBackProgress()
{
}

QBaseProgress *QAbstractContainerWidget::getBackProgress()
{
    QBaseProgress *backProgress = nullptr;

    if (isTopWidgetVisible()) {
        backProgress = QBackProgress::Instance();
    } else {
        backProgress = QEventProgress::Instance();
    }

    if (m_backProgress != nullptr) {
        backProgress = m_backProgress;
    }

    return backProgress;
}

QString QAbstractContainerWidget::serialize()
{
    QVariantMap variantMap = QJson::QObjectHelper::qobject2qvariant(this,
                             ignoredPropertyList);
    Serializer serializer;
    QByteArray array = serializer.serialize(variantMap);
    return QString(array);
}

void QAbstractContainerWidget::deSerialize(const QString &data)
{
    Parser parser;
    QVariant variant = parser.parse(data.toLatin1());
    QObjectHelper::qvariant2qobject(variant.toMap(), this);
}

const QVariant QAbstractContainerWidget::toVariant() const
{
    return QVariant(QJson::QObjectHelper::qobject2qvariant(this,
                    ignoredPropertyList));
}

const QString QAbstractContainerWidget::tagName()
{
    QList<QByteArray> nameList = dynamicPropertyNames();

    foreach (QByteArray name, nameList) {
        QString strName = QString(name);

        if (strName == "tagName") {
            QVariant value = property(strName.toStdString().c_str());
            return value.toString();
        }
    }

    return QString("");
}

void QAbstractContainerWidget::customEvent(QEvent *event)
{
    if (QGenericTopicEvent::topicEventType() == event->type()) {
        QGenericTopicEvent *topicEvent = static_cast<QGenericTopicEvent *>(event);
        QJson::Parser parser;
        bool ok = false;
        QVariant variant = parser.parse(topicEvent->message(), &ok);

        if (!ok) {
            qprofileDebug(QtDebugMsg) << "Get a unknown custom event! " <<
                                      topicEvent->message();
            return QObject::customEvent(event);
        }

        if (m_topic->getRestfulTopic(QAbstractTopic::get) == topicEvent->topic()) {
            QGenericMessage message(this, topicEvent->topic(), variant);
            QByteArray ret = get(message);

            if (ret.count() > MAX_QPROFILE_DEBUG_LENGTH) {
                qprofileDebug(QtDebugMsg) << this << ret.left(MAX_QPROFILE_DEBUG_LENGTH);
            } else if (ret.count() > 0) {
                qprofileDebug(QtDebugMsg) << this << ret;
            }
        }
    }

    return QObject::customEvent(event);
}

void QAbstractContainerWidget::onModelChanged()
{
    if (!m_model.isNull()) {
        QString topic = m_topic->getRestfulTopic(QAbstractTopic::put);
        QGenericMessage message(this);
        message.setTopic(topic);
        message.setContent(m_model->toVariant());
        QByteArray json = message.toJson();

        if (json.length() <= 0) {
            qprofileDebug(QtDebugMsg) << "Failed to serialize message " << message.topic()
                                      <<
                                      m_model->toJson() << " for " << metaObject()->className();
        } else {
            if (0 == m_ipcClient->publish(topic, json, QAbstractIpcClient::QOS1,
                                          m_isRetainMessage)) {
                qprofileDebug(QtDebugMsg) << "Publish failed, topic = " <<
                                          m_topic->getRestfulTopic(
                                              QAbstractTopic::put) << " message = " << json;
            } else {
                if (qobject_cast<QTimer *>(sender()) == nullptr) {
                    if (json.length() > MAX_QPROFILE_DEBUG_LENGTH) {
                        qprofileDebug(QtDebugMsg) << sender() << this << "publish topic = " << topic <<
                                                  "content = " << json.left(MAX_QPROFILE_DEBUG_LENGTH);
                    } else {
                        qprofileDebug(QtDebugMsg) << sender() << this << "publish topic = " << topic <<
                                                  "content = " << json;
                    }
                }
            }
        }
    }
}

const QByteArray QAbstractContainerWidget::get(const QGenericMessage &
        message)
{
    if (!m_model.isNull()) {
        const QVariant &content = message.content();

        if (content == model()->toVariant()) {
            return "";
        }

        bool lastStatus = model()->blockNotify(true);
        QObjectHelper::qvariant2qobject(content.toMap(), model());

        if (m_binder == nullptr) {
            QObjectHelper::qvariant2qobject(model()->toVariant().toMap(), this);
        }

        model()->blockNotify(lastStatus);
        model()->emitModelChangedByOthers();
        return model()->toJson();
    }

    return QByteArray();
}
void QAbstractContainerWidget::setModel(QAbstractModel *model, bool needBinder)
{
    if (!m_model.isNull()) {
        delete m_model.data();
    }

    m_model = model;

    if (needBinder) {
        m_binder = new QRserverPropertyBinder(this, model);
    }
}
QAbstractModel *QAbstractContainerWidget::model()
{
    return m_model.data();
}

QAbstractContainerWidget::~QAbstractContainerWidget()
{
}
