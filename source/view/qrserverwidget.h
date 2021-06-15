#ifndef QRSERVERWIDGET_H_
#define QRSERVERWIDGET_H_

#include <QTabWidget>
#include <QPushButton>
#include <QTimer>
#include "qregisterwidget.h"
#include "quploadwidget.h"
#include "qdownloadwidget.h"
#include "qsoftwarewidget.h"
#include "qvconnectmodel.h"
#include "qregistermodel.h"
#include <QFileSystemWatcher>

namespace QRserver
{
    class QRserverWidget : public QRserverContainerWidget
    {
            Q_OBJECT

        public:
            enum RserverTabIndex {
                RserverTabRegister = 0,
                RserverTabUpload,
                RserverTabDownload,
                RserverTabSoftware,
            };
        public:
            QRserverWidget(QWidget *parent = 0, const QString &tagName = "qrserver");
            ~QRserverWidget();

            static const qint32 CheckRegistrationBombInterval;

            void translate(const QFont &font);
            virtual bool isTopWidget();

        private:
            QTabWidget			*m_ptab;
            QRegisterWidget		*m_pRegister;
            QUploadWidget               *m_pUploadWidget;
            QDownloadWidget		*m_pDownloadWidget;
            QSoftwareWidget		*m_pSoftwareWidget;
            QPushButton			*m_pBtnRegister;
            QPushButton			*m_pBtnUnRegister;
            QPushButton			*m_pBtnCheck;
            QPushButton			*m_pBtnRServerMode;
            QPushButton			*m_pBtnUpload;
            QPushButton			*m_pBtnJob;
            QPushButton			*m_pBtnUploadAndDel;
            QPushButton			*m_pBtnDownload;
            QPushButton			*m_pBtnUpgrade;
            QPushButton			*m_pBtnUpdate;
            QVConnectModel *m_vconnectModel;
            const QRegisterModel *m_registerModel;
            QFileSystemWatcher m_fileSystemWatcher;
            QTimer m_loadVConncetServerConfigTimer;
            bool m_uploadTabCurrentIdx;
            QTimer m_registrationBombTimer;
            QVBoxLayout *m_pBtLayout;

        signals:
            void setRegisterApproved(bool bApproved);
            void registrationBombExpired();
            void lockUploadProfileTab(bool lock);
            void registerRserver();
            void updateTechName();

        private slots:
            void onUploadWidgetTabChanged(int idx);
            void onTabChange(int idx);
            void onRserverModelChanged();
            void onCheckDeviceSucceed();
            void onAutoCheck();
            void lockTab(bool lock = true);
            void onRegisterModelChanged();
            void onDirectoryChanged(const QString &path);
            void onRServerModeChanged();
            void onVConnectModelChanged();
            void onLoadVConncetServerConfig();
            void onUnregister();
            void onCheckRegistrationBomb();
            void onRestoreDefault();
            void onUpateDeviceInfoSucceed();
            void onRegisterButtonClicked();

        protected:
            virtual void allocModel();
            virtual void customEvent(QEvent *event);
            bool isTechNameCanBeChanged();

        protected slots:
            virtual void onIpcDisconnected();
            virtual void onNetworkError();

    };
}

#endif // QRSERVERWIDGET_H_
