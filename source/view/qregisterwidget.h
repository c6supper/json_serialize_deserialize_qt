#ifndef QREGISTERWIDGET_H_
#define QREGISTERWIDGET_H_

#include "qrservercontainerwidget.h"
#include <QLabel>
#include "qparmlineedit.h"
#include <QHostInfo>
#include "qregistermodel.h"
#include "qvconnectmodel.h"

namespace QRserver
{
    class QRegisterWidget : public QRserverContainerWidget
    {
            Q_OBJECT

            Q_PROPERTY(QString serverAddress READ serverAddress WRITE setServerAddress)
            Q_PROPERTY(QString techId READ techId WRITE setTechId)
            Q_PROPERTY(QString techName READ techName WRITE setTechName)
            Q_PROPERTY(QString company READ company WRITE setCompany)
            Q_PROPERTY(QString supervisorId READ supervisorId WRITE setSupervisorId)
            Q_PROPERTY(QString region READ region WRITE setRegion)
            Q_PROPERTY(QString district READ district WRITE setDistrict)
            Q_PROPERTY(QString system READ system WRITE setSystem)
            Q_PROPERTY(QString serverIp READ serverIp WRITE setServerIp)

        public:
            QRegisterWidget(QWidget *parent = 0, const QString &tagName = "qregister");
            ~QRegisterWidget();

            QString serverAddress() const;
            void setServerAddress(const QString &serverAddress);
            QString techId() const;
            void setTechId(const QString &techId);
            QString techName() const;
            void setTechName(const QString &techName);
            QString company() const;
            void setCompany(const QString &company);
            QString supervisorId() const;
            void setSupervisorId(const QString &supervisorId);
            QString region() const;
            void setRegion(const QString &region);
            QString district() const;
            void setDistrict(const QString &district);
            QString system() const;
            void setSystem(const QString &system);
            QString serverIp() const;
            void setServerIp(QString serverIp);

            void translate(const QFont &font);

        private:
            QParmLineEdit	*m_serverAddress;
            QParmLineEdit	*m_techID;
            QParmLineEdit	*m_techName;
            QParmLineEdit	*m_company;
            QParmLineEdit	*m_supervisorId;
            QLabel			*m_info;
            QParmLineEdit	*m_region;
            QParmLineEdit	*m_district;
            QParmLineEdit	*m_system;
            QRegisterModel	*m_registerModel;
            QTimer          m_lookupHostTimer;
            QTimer			m_updateTechInfoTimer;
            QLabel			*m_vconnectInfo;
            QLabel          *m_clientInfo;

            static const int m_techInfoLength;
            static const int m_supervisorIdLength;
            static const int m_companyLength;

        signals:
            void checkDeviceSucceed();
            void upateDeviceInfoSucceed();
            void serverAddressChanged();
            void getProfileInfoList();

        public slots:
            void setRegionItemsVisible(bool bVisible);
            void onUpdateTechName();
            void onRegister();
            void onUnregister();
            void onCheck();
            void onRegistrationBombExpired();
            void onRestoreDefault();
            void onUpdate();
            void setRegisterApproved(bool bApproved);
            void onRServerModeChanged(qint32 rserverMode);

        protected:
            virtual void allocModel();

        protected slots:
            virtual void postponeInit();
            virtual void onIpcDisconnected();
            virtual void onIpcConnected();
            virtual void onNetworkError();

        private slots:
            void hostChanged(const QString &hostname);
            void lookUpHost(const QHostInfo &host);
            void onSubscribeFinished();
            void saveProfile();
            void onLookupHost();
            void onRegisterParamChanged(const QString &text);
            void onCheckTechInfo();

        private:
            void setInformation(const QString &information);
            void setValid(bool bValid);
            void showNetworkErrorMessageBox();
            bool loadProfile(const QString filePathName);
            bool loadLastProfile();
            bool checkConnective();
            void getCompatibility(const QString className);
            qint32 getDeviceStatus(const QRserverModel &rserverModel);
            bool isTechNameCanBeChanged();
    };
};

#endif // QREGISTERWIDGET_H_
