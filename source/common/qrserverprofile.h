/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QRSERVERPROFILE_H
#define QRSERVERPROFILE_H

#include "qrserverfile.h"
#include "qdevicerelatedfiletype.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QRserver
{
    class QRserverProfile: public QRserverFile
    {
            Q_OBJECT
            Q_ENUMS(ProfileType)

            Q_PROPERTY(QVariantList deviceRealtedProfileList READ deviceRealtedProfileList
                       WRITE setDeviceRealtedProfileList)
            Q_PROPERTY(QString resultSignaturePath READ resultSignaturePath WRITE
                       setResultSignaturePath)

        public:
            enum ProfileType {
                eProfBert1GC1 = 300,
                eProfBert1GC2,
                eProfBert1GF1,
                eProfBert1GF2,
                eProfBert10G,
                eProfRfc25441GC1,
                eProfRfc25441GC2,
                eProfRfc25441GF1,
                eProfRfc25441GF2,
                eProfRfc254410G,
                eProfThrpt1GC1,
                eProfThrpt1GC2,
                eProfThrpt1GF1,
                eProfThrpt1GF2,
                eProfThrpt10G,
                eProfScan1GC1,
                eProfScan1GC2,
                eProfScan1GF1,
                eProfScan1GF2,
                eProfScan10G,
                eProfVsam1GC1,
                eProfVsam1GC2,
                eProfVsam1GF1,
                eProfVsam1GF2,
                eProfVsam10G,

                eProfBert10G2,
                eProfRfc254410G2,
                eProfThrpt10G2,
                eProfScan10G2,
                eProfVsam10G2,

                eProfVperf1GC1 = 330,
                eProfVperf1GC2,
                eProfVperf1GF1,
                eProfVperf1GF2,
                eProfVperf10G,
                eProfVperf10G2,

                // 40g
                eProfBert40G = 336,
                eProfRfc254440G,
                eProfThrpt40G,
                eProfVsam40G,
                eProfEthernetPort40G = 528,

                // 100g
                eProfBert100G = 340,
                eProfRfc2544100G,
                eProfThrpt100G,
                eProfVsam100G,
                eProfVperf100G,
                eProfEthernetPort100G = 529,

                //FC
                eProfFcBert1GF1 = 400,
                eProfFcBert1GF2,
                eProfFcBert10G,
                eProfFcBert10G2,
                eProfFcRfc25441GF1,
                eProfFcRfc25441GF2,
                eProfFcRfc254410G,
                eProfFcRfc254410G2,
                eProfFcThrpt1GF1,
                eProfFcThrpt1GF2,
                eProfFcThrpt10G,
                eProfFcThrpt10G2,

                eProfEthernetIP1GC1 = 450,//todo IP/network IP/PPPoE IP/port profile saved in a same directory
                eProfPPPoE1GC1,//todo IP/network IP/PPPoE IP/port profile saved in a same directory
                eProfEthernetPort1GC1,//todo IP/network IP/PPPoE IP/port profile saved in a same directory
                eProfPing1GC1,
                eProfTraceRoute1GC1,
                eProfARPWiz1GC1,
                eProfFTPDownload1GC1,
                eProfFTPUpload1GC1,
                eProfWebTest1GC1,//todo can't find
                eProfWebBrowser1GC1,//todo can't find
                eProfIPPhone1GC1,
                eProfVoIPClient1GC1,
                eProfVoIPCheck1GC1,
                eProfEthernetIP1GC2,//todo IP/network IP/PPPoE IP/port profile saved in a same directory
                eProfPPPoE1GC2,//todo IP/network IP/PPPoE IP/port profile saved in a same directory
                eProfEthernetPort1GC2,//todo IP/network IP/PPPoE IP/port profile saved in a same directory
                eProfPing1GC2,
                eProfTraceRoute1GC2,
                eProfARPWiz1GC2,
                eProfFTPDownload1GC2,
                eProfFTPUpload1GC2,
                eProfWebTest1GC2,//todo can't find
                eProfWebBrowser1GC2,//todo can't find
                eProfIPPhone1GC2,
                eProfVoIPClient1GC2,
                eProfVoIPCheck1GC2,
                eProfEthernetIP1GF1,//todo IP/network IP/PPPoE IP/port profile saved in a same directory
                eProfPPPoE1GF1,//todo IP/network IP/PPPoE IP/port profile saved in a same directory
                eProfEthernetPort1GF1,//todo IP/network IP/PPPoE IP/port profile saved in a same directory
                eProfPing1GF1,
                eProfTraceRoute1GF1,
                eProfARPWiz1GF1,
                eProfFTPDownload1GF1,
                eProfFTPUpload1GF1,
                eProfWebTest1GF1,//todo can't find
                eProfWebBrowser1GF1,//todo can't find
                eProfIPPhone1GF1,
                eProfVoIPClient1GF1,
                eProfVoIPCheck1GF1,
                eProfEthernetIP1GF2,//todo IP/network IP/PPPoE IP/port profile saved in a same directory
                eProfPPPoE1GF2,//todo IP/network IP/PPPoE IP/port profile saved in a same directory
                eProfEthernetPort1GF2,//todo IP/network IP/PPPoE IP/port profile saved in a same directory
                eProfPing1GF2,
                eProfTraceRoute1GF2,
                eProfARPWiz1GF2,
                eProfFTPDownload1GF2,
                eProfFTPUpload1GF2,
                eProfWebTest1GF2,//todo can't find
                eProfWebBrowser1GF2,//todo can't find
                eProfIPPhone1GF2,
                eProfVoIPClient1GF2,
                eProfVoIPCheck1GF2,
                eProfEthernetIP10G,//todo IP/network IP/PPPoE IP/port profile saved in a same directory
                eProfPPPoE10G,//todo IP/network IP/PPPoE IP/port profile saved in a same directory
                eProfEthernetPort10G,//todo IP/network IP/PPPoE IP/port profile saved in a same directory
                eProfPing10G,
                eProfTraceRoute10G,
                eProfARPWiz10G,
                eProfFTPDownload10G,
                eProfFTPUpload10G,
                eProfWebTest10G,//todo can't find
                eProfWebBrowser10G,//todo can't find
                eProfIPPhone10G,
                eProfVoIPClient10G,
                eProfVoIPCheck10G,
                eProfEthernetIP10G2,//todo IP/network IP/PPPoE IP/port profile saved in a same directory
                eProfPPPoE10G2,//todo IP/network IP/PPPoE IP/port profile saved in a same directory
                eProfEthernetPort10G2,//todo IP/network IP/PPPoE IP/port profile saved in a same directory
                eProfPing10G2,
                eProfTraceRoute10G2,
                eProfARPWiz10G2,
                eProfFTPDownload10G2,
                eProfFTPUpload10G2,
                eProfWebTest10G2,//todo can't find
                eProfWebBrowser10G2,//todo can't find
                eProfIPPhone10G2,
                eProfVoIPClient10G2,
                eProfVoIPCheck10G2,

                //universal profile types
                eProfBert1GC = 650,
                eProfBertEth10G,
                eProfRfc25441GC,
                eProfRfc2544Eth10G,
                eProfThrpt1GC,
                eProfThrptEth10G,
                eProfScan1GC,
                eProfScanEth10G,
                eProfVsam1GC,
                eProfVsamEth10G,
                eProfVperf1GC,
                eProfVperfEth10G,
                eProfBertCSP,
                eProfRfc2544CSP,
                eProfThrptCSP,
                eProfVsamCSP,
                eProfVperfCSP,
                eProfBertEth25G,
                eProfRfc2544Eth25G,
                eProfThrptEth25G,
                eProfScanEth25G,
                eProfVsamEth25G,
                eProfBertEth50G,
                eProfRfc2544Eth50G,
                eProfThrptEth50G,
                eProfScanEth50G,
                eProfVsamEth50G,
                eProfBert1GF = 683,
                eProfRfc25441GF,
                eProfThrpt1GF,
                eProfScan1GF,
                eProfVsam1GF,
                eProfVperf1GF,

                // 25G
                eProfBert25G = 700,
                eProfBert25G2,
                eProfRfc254425G,
                eProfRfc254425G2,
                eProfThrpt25G,
                eProfThrpt25G2,
                eProfScan25G,
                eProfScan25G2,
                eProfVsam25G,
                eProfVsam25G2,
                eProfEthernetIP25G,//todo IP/network IP/PPPoE IP/port profile saved in a same directory
                eProfEthernetIP25G2,//todo IP/network IP/PPPoE IP/port profile saved in a same directory
                eProfPPPoE25G,//todo IP/network IP/PPPoE IP/port profile saved in a same directory
                eProfPPPoE25G2,//todo IP/network IP/PPPoE IP/port profile saved in a same directory
                eProfEthernetPort25G,//todo IP/network IP/PPPoE IP/port profile saved in a same directory
                eProfEthernetPort25G2,//todo IP/network IP/PPPoE IP/port profile saved in a same directory
                eProfPing25G,
                eProfPing25G2,
                eProfTraceRoute25G,
                eProfTraceRoute25G2,
                eProfARPWiz25G,
                eProfARPWiz25G2,

                // 50G
                eProfBert50G = 750,
                eProfBert50G2,
                eProfRfc254450G,
                eProfRfc254450G2,
                eProfThrpt50G,
                eProfThrpt50G2,
                eProfScan50G,
                eProfScan50G2,
                eProfVsam50G,
                eProfVsam50G2,
                eProfEthernetIP50G,//todo IP/network IP/PPPoE IP/port profile saved in a same directory
                eProfEthernetIP50G2,//todo IP/network IP/PPPoE IP/port profile saved in a same directory
                eProfPPPoE50G,//todo IP/network IP/PPPoE IP/port profile saved in a same directory
                eProfPPPoE50G2,//todo IP/network IP/PPPoE IP/port profile saved in a same directory
                eProfEthernetPort50G,//todo IP/network IP/PPPoE IP/port profile saved in a same directory
                eProfEthernetPort50G2,//todo IP/network IP/PPPoE IP/port profile saved in a same directory
                eProfPing50G,
                eProfPing50G2,
                eProfTraceRoute50G,
                eProfTraceRoute50G2,
                eProfARPWiz50G,
                eProfARPWiz50G2,

                // Copper SFP+
                eProfBertCSP1 = 850,
                eProfRfc2544CSP1,
                eProfThrptCSP1,
                eProfVsamCSP1,
                eProfEthernetIPCSP1,
                eProfPPPoECSP1,
                eProfEthernetPortCSP1,
                eProfPingCSP1,
                eProfTraceRouteCSP1,
                eProfARPWizCSP1,
                eProfIPPhoneCSP1,
                eProfVoIPClientCSP1,
                eProfVoIPCheckCSP1,
                eProfVperfCSP1,
                eProfFTPDownloadCSP1,
                eProfFTPUploadCSP1,

                eProfBertCSP2 = 900,
                eProfRfc2544CSP2,
                eProfThrptCSP2,
                eProfVsamCSP2,
                eProfEthernetIPCSP2,
                eProfPPPoECSP2,
                eProfEthernetPortCSP2,
                eProfPingCSP2,
                eProfTraceRouteCSP2,
                eProfARPWizCSP2,
                eProfIPPhoneCSP2,
                eProfVoIPClientCSP2,
                eProfVoIPCheckCSP2,
                eProfVperfCSP2,
                eProfFTPDownloadCSP2,
                eProfFTPUploadCSP2,

                /* Combine IPv4 PPPoE IPv6 */
                eProfNetwork1GC1 = 950,
                eProfNetwork1GC2,
                eProfNetwork1GF1,
                eProfNetwork1GF2,
                eProfNetwork10G,
                eProfNetwork10G2,
                eProfNetwork25G,
                eProfNetwork25G2,
                eProfNetwork50G,
                eProfNetwork50G2,
                eProfNetworkCSP1,
                eProfNetworkCSP2,

                // Common 1000
                eProfLocator = 1000, //todo can't find
                eProfType,//todo can't find
                eProfIPTVChannelTable,//todo can't find
                eProfEthernetIP,//todo can't find
                eProfPPPoE,//todo can't find
                eProfEthernetPort,//todo can't find
                eProfPing,//todo can't find
                eProfTraceRoute,//todo can't find
                eProfARPWiz,//todo can't find
                eProfFTPDownload,//todo can't find
                eProfFTPUpload,//todo can't find
                eProfWebTest,//todo can't find
                eProfWebBrowser,//todo can't find
                eProfIPPhone,//todo can't find
                eProfVoIPClient,//todo can't find
                eProfVoIPCheck,//todo can't find

                /* Anydsl:520/521/522/523 */
                eProfDSLIp = 50300,
                eProfDSLPassThrough,
                eProfVDSLIp,
                eProfVDSLPassThrough,
                eProfTIIp,
                eProfTIPassThrough,
                eProfLinkUpThreshold,
                eProfDMMAutoTestThreshold,
            };
        public:
            QRserverProfile(QObject *parent = nullptr);
            ~QRserverProfile();
            virtual const QString type2String(const qint32 type);
            virtual const qint32 stringToType(const QString typeString);
            static const QVariant GetFileType(const QString &filePath,
                                              const bool enableUseDirPath = false);
            static const QVariant GetFileType(const qint32 type);
            static const QVariant GetDeviceRelatedFileType(const QString &localPath);
            static const QVariantList GetDeviceRelatedFileType(const qint32 type);
            static const QString getResultSignaturePath();
            static const QStringList GetDeviceRelatedProfilePathList();

            QVariantList deviceRealtedProfileList();
            void setDeviceRealtedProfileList(QVariantList &deviceRealtedProfileList);

            const QString resultSignaturePath() const;
            void setResultSignaturePath(QString resultSignaturePath);

        public:
            static const QString DefaultRserverProfileConfig;
            static const QString DeviceRelatedProfileConfig;
            static const QString DefaultResultSignaturePath;

        private:
            static QRserverProfile *instance();
            void loadDeviceRelatedProfileConfig();
            const QVariant getDeviceRelatedFileType(const QString &localPath);
            const QVariantList getDeviceRelatedFileType(const qint32 type);
            const QStringList getDeviceRelatedProfilePathList();

        protected:
            virtual bool isKey(const QString &filePath,
                               const QString measureMode = "", const QString measureType = "");
            virtual const QString getDisplayName(const QString &filePath,
                                                 const QString measureMode = "", const QString measureType = "");

        private:
            static QRserverProfile				 *m_instance;
            QMutex								m_mutex;
            QList<QDeviceRelatedFileType *>		m_deviceRealtedProfileList;
            QString                             m_resultSignaturePath;
    };
}


#endif /* QRSERVERPROFILE_H */
