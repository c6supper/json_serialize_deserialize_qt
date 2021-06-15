/*****************************************************************************************

** commif.h: Definition of the commands and results used for communication  between
**           the Back Engine and UIApp. The common commands and results which are
**           processed by architecture can be defined in this public header file.
**
** Author: Witman.Wang
** Copyright (C) 2006 VeTronics(BeiJing) Ltd.
**
** Create date: 06-20-2006
*****************************************************************************************/

#ifndef V100_BEUI_COMMUNICATION_INTERFACE_JHT3425TRER423FSD_EYTT54TRHG_H_
#define V100_BEUI_COMMUNICATION_INTERFACE_JHT3425TRER423FSD_EYTT54TRHG_H_

#include "comndef.h"


#define WEB_BROWSER_OPTION 0x01
#define ADV_IPTOOLS_OPTION 0x02
#define NET_WIZ_OPTION     0x04
#define REMOTE_CTRL_OPTION 0x08
#define VOIP_EXPERT_OPTION 0x10
#define IPTV_EXPERT_OPTION 0x20
#define WIFI_WIZ_OPTION    0x40
#define VOIP_CALL_OPTION   0x80
#define ADMIN_MODE_OPTION  0x0100
#define VOIP_G723CODEC_OPTION 0x0200
#define VOIP_G729CODEC_OPTION 0x0400
#define VOIP_CHECK_OPTION	0x0800
#define DATA_GPS_OPTION	 0x1000

#define RESERVED_CHARS_COUNT     8
#define FPGA_FILE_PATH_LEN       24

#define DATE_TIME_MAX_LEN        24
#define IPADDR_MAX_STRLEN        24
#define IFNAME_MAX_STRLEN        16
#define MACADDR_MAX_STRLEN       48

#define MAX_HOSTNAME_LEN        50
#define MAX_FILE_PATH_LEN		256
#define MAX_USER_PASS_LEN		20
#define DHCP_VCLS_LEN           32
#define DHCP_UCLSHEX_LEN        64
#define DHCP_VENDHEX_LEN        64
#define IPV6ADDR_MAX_STRLEN     64

#define IS_MODULE_CMD(type)      ( type > eCmdPubMax )
#define IS_MODULE_RET(type)      ( type > eRetPubMax )
typedef int BOOL;

typedef enum {
    eStatus_warehouse = 4,
    eStatus_in_training = 0,
    eStatus_customer = 5,
    eStatus_en_route = 1,
    eStatus_break = 2,
    eStatus_lunch = 3,
} enumStatus;

typedef enum {
    e_in = 0,
    e_off = 1,
} enumGlobalSatus;

typedef void *(*CALLBACK_START_THREAD)(void *);

/************************************************************************/
/* Define the commands used by architecture.                            */
/************************************************************************/


//VCT test status structure, mv88e1111 can use this directly
typedef struct {
    unsigned short usVCTFail;			//Unable to perform VCT on this pair
    unsigned short usOpenFt;			//reg26(14:13) 10
    unsigned short usShortFt;			//reg26(14:13) 01
    unsigned short usImpMismatchFt;		//reg26(14:13) 00, and reg26(14:13) < 0xff
    unsigned short usGoodTermination;	//reg26(14:13) 00, and reg26(14:13) = 0xff

    float          usFtDist;			//The distance to fault of this Tx pair

    unsigned char  ucPairSkew;          // Max is 120ns
    unsigned char  ucPolaSwap;          // 0: Positive; 1: Negative

} VCTFTINFO, *PVCTFTINFO;

typedef enum eModuleType {
    eSlm = 1,
    eSlm150,
    eEther,
    eFiber,
    eDsl,
    eDslB,
    eDslTI,
    eLan,
    eVDSL,
    eCommon,
#ifdef TX100_SPEC_MODIFY
    eTx150e,
    eTx130e,
#endif
    eModuleTypeNum
} eModuleType;

typedef struct {
    VCTFTINFO info;		//Rx pair for mv88e3015
} VCTINFO, *PVCTINFO;

typedef struct {
    VCTINFO  vtc_info[4];
    unsigned short usCrossOver;

    // 0: Invalid; 1: Valid
    char     cPairSkewIsValid;
    char     cPolaSwapIsValid;

    char     cPairSwapIsValid;
    // 1: PHY transmit as B and receives A on MDI[0]+/-.
    //    PHY transmit as A and receives B on MDI[1]+/-.
    // 0: PHY transmit as A and receives B on MDI[0]+/-.
    //    PHY transmit as B and receives A on MDI[1]+/-.
    unsigned char  ucABPairSwap;
    // 1: PHY transmit as D and receives C on MDI[2]+/-.
    //    PHY transmit as C and receives D on MDI[3]+/-.
    // 0: PHY transmit as C and receives D on MDI[2]+/-.
    //    PHY transmit as D and receives C on MDI[3]+/-.
    unsigned char  ucCDPairSwap;

} VCTRESULT;

typedef enum enumCmd {
    eCmdConnTest = 0,

    eCmdSetupFPGA,

    /* No corresponding structure */
    eCmdStateTimer,

    eCmdSetClock,		//set Clock.

    eCmdSetBackLight,	//int value. 0-100

    eCmdClearHistory,	// clear history status, no parameter.

    eCmdEnableBuzzer,	//enable buzzer

    eCmdSetLed,
    //add iptools cmd

    eCmdUdhcpc,         //start udhcpc

    eCmdAutoDhcpc,      //auto udhcpc

    eCmdUdhcpv6c,  //start dhcpv6c

    eCmdAutov6,  //start rdisc6

    eCmdCastletudhcpc,	//start udhcpc for castlnet emulation

    eCmdCastletconfig,	//get Castlnet configuration

    eCmdIfconfig,		//ifconfig
    eCmdIfconfigIPv6,	//ifconfig ipv6

    eCmdRoute,			//route

    eCmdDns,			//dns
    eCmdIpupdown,

    eCmdArpRequest,      //arp request
    eCmdPingRequest,      //arp request
    eCmdFtpGetRequest,   //ftp get request
    eCmdFtpPutRequest,   //ftp put request
    eCmdTftpGetRequest,  //tftp download/upload request
    eCmdHttpdRequest,    //http download request
    eCmdEchodRequest,    //echo daemon request
    eCmdPppoeRequest,    //PPPoE request
    eCmdTraceRoute,	     //traceroute
    eCmdTraceRouteStop,	 //stop traceroute
    eCmdDiskSpaceRequest,
    eCmdWebRequest,
    eCmdArpStop,
    eCmdPingStop,
    eCmdFtpWebStop,
    eCmdSwitchInterface,
    eCmdConnectPPPoE,
    eCmdDisconnectPPPoE,

    eCmdPcapRequest,
    eCmdPcapStop,

    /*add UI report switch on|off support*/
    eCmdBlockReport,

    /* New commands can be added here */
    eCmdMountUSBDev,
    eCmdUmountUSBDev,
    eCmdStartUSBTransfer,
    eCmdStopUSBTransfer,
    eCmdManPortSet,
    eCmdStartManPortCheck,
    eCmdStopManPortCheck,
    eCmdStartFtpUpload,
    eCmdStopFtpUpload,
    eCmdPubVCTFe,
    eCmdCVTAdjustFe,
    eCmdStartVOIP,
    eCmdSendVOIPCmd,
    eCmdStopVOIP,
    eCmdIPTV,
    eCmdEnterIPTV,
    eCmdEscIPTV,
    eCmdSuspendMonitor,
    eCmdResumeMonitor,

    eCmdFanctrl,
    eCmdIptoolsStop,

    eCmdDoIPScan,
    eCmdStopIPScan,
    eCmdIPScanSum,

    eCmdInitSIP,
    eCmdExitSIP,
    eCmdSIPRegister,
    eCmdIPPhoneCall,
    eCmdIPPhoneHangUp,
    eCmdIPPhonePlay,
    eCmdIPPhoneDialDTMF,
    eCmdSIPCancelRegister,
    eCmdIncomingCallAccept,
    eCmdIPPhoneStopPlay,
    eCmdTurnOnHeadset,
    eCmdTurnOffHeadset,

    eCmdInitH323,
    eCmdExitH323,
    eCmdH323Register,
    eCmdOhPhoneCall,
    eCmdOhPhoneHangUp,
    eCmdOhPhonePlay,
    eCmdOhPhoneStopPlay,
    eCmdH323CancelRegister,
    eCmdOhIncomingCallAccept,

    eCmdInitClone,
    eCmdExitClone,
    eCmdStartClone,
    eCmdSendClone,
    eCmdRecvClone,

    eCmdStartMos,
    eCmdStopMos,

    eCmdSetAudibleAlarmOff,
    eCmdSetAudibleAlarmOn,

    eCmdEnterWifiWiz,
    eCmdExitWifiWiz,
    eCmdStartWifiWiz,
    eCmdStopWifiWiz,
    eCmdWifiScan,
    eCmdWifiConnect,
    eCmdWifiDisconnect,
    eCmdWifiEnableUSB,
    eCmdWifiDisableUSB,
    eCmdWifiServiceStatus,

    eCmdSyncServerPort,  //  Corresponding command structure is CMD_SyncServerPort

    eCmdSyncUpload,       // Corresponding command structure is CMD_SyncUpload
    eCmdSyncWithoutRegUpload,			//
    eCmdSyncAbort,        // No corresponding command structure

    eCmdSyncRegister,     // Corresponding command structure is CMD_SyncDeviceInfo
    eCmdSyncUnRegister,   // Corresponding command structure is CMD_SyncDeviceInfo
    eCmdSyncCheck,        // Corresponding command structure is CMD_SyncDeviceInfo

    eCmdSyncDownload,     // Corresponding command structure is CMD_SyncDeviceInfo
    eCmdSyncGetAdminSet,  // Corresponding command structure is CMD_SyncDeviceInfo

    eCmdSyncProfiles,     // Corresponding command structure is CMD_SyncDownProfiles

    eCmdSetTrafficRegulatorOn,
    eCmdSetTrafficRegulatorOff,

    eCmdDownloadXilinx,

    eCmdStartDataGPS,
    eCmdStopDataGPS,
    eCmdSendDataGPSCmd,
    eCmdDataGPSReportStart,
    eCmdDataGPSReportStop,

    eCmdSyncGetWorkForceStatus,
    eCmdSyncGetJobsStatus,
    eCmdSyncGetJobsManageStatus,
    eCmdSyncReptClosedJob,
    eCmdSyncSetCurrentJob,
    eCmdSyncUpdateWorkForceTech,
    eCmdSyncUpdateGpsData,
    eCmdSyncUpdateGpsStaus,

    eCmdBlueToothInit,
    eCmdBlueToothScan,
    eCmdBlueToothPair,
    eCmdBlueToothTransfer,
    eCmdBlueToothService,
    eCmdBlueToothDialup,
    eCmdBlueToothExit,

    eCmdFtpGetRequestStop,
    eCmdFtpPutRequestStop,
    eCmdHttpdRequestStop,
    eCmdPingRequestStop,

    eCmdBlueToothScanStop,
    eCmdBlueToothPairStop,
    eCmdBlueToothTransferStop,

    eCmdHciconfig,

    eCmdInitCommon,

    //for net-updater
    eCmdNetUpdate,
    eCmdNetUpdateStop,
    eCmdNetUncompress,

    eCmdPubCount,
    eCmdPubMax = 999

} enumCMD;

typedef struct ConnTestCmd {
    unsigned int  uTestID;
    unsigned char Reserved[RESERVED_CHARS_COUNT];

} ConnTestCMD;

/*
pBuf[0]  // second
pBuf[1]  // minute
pBuf[2]  // hour
pBuf[3]  // weekday, {"Sun", "Mon", "Tue", "Wed", "Thur", "Fri", "Sat"}
pBuf[4]  // day
pBuf[5]  // month
pBuf[6]  // year, 2000 + year
*/
typedef struct RTClock {
    int nTemp;
    unsigned char strDateTime[DATE_TIME_MAX_LEN];

} RTClock;
//US
typedef struct {
    int nValue;
} CMD_BackLight;

typedef struct {
    int testcmd;
} CMD_VOIPpack;

typedef struct {
    BOOL selModule;
    BOOL selCommon;
    eModuleType modeType;
} CLONEFILESEL;


typedef struct {
    int nValue;
} CMD_FANCTRL;


typedef struct {
    int nValue;
} CMD_STANDBY;

typedef struct {
    int 	eLed;
    int	eAction;
} CMD_LED;

typedef struct {
    int enableflag;
} CMD_Buzzer;

typedef struct {
    int   inet;
    char  iface[IFNAME_MAX_STRLEN]; /*interface name used for arp request, such as "eth0" */
    char  nums_ping[8];
    char  sec_ping[8];
    char  lenth_ping[8];
    char  timeout[8];
    char  destIP[IPV6ADDR_MAX_STRLEN];
} CMD_PingReq;

typedef struct {
    char		Dev[IFNAME_MAX_STRLEN];
    char		LocalIpAddr[IPADDR_MAX_STRLEN];
    char        HostName[MAX_HOSTNAME_LEN];
    char        VendorClass[DHCP_VCLS_LEN];
    char        UserClassHex[DHCP_UCLSHEX_LEN];
    char        VendorInfoHex[1024];
} CMD_Udhcpreq;

typedef struct {
    char		Dev[IFNAME_MAX_STRLEN];
    char		LocalIpAddr[IPADDR_MAX_STRLEN];
} CMD_Autodhcpreq;

typedef struct {
    char		Dev[IFNAME_MAX_STRLEN];
    char		LocalIpAddr[IPV6ADDR_MAX_STRLEN];
    char		GatewayAddr[IPV6ADDR_MAX_STRLEN];
    int		IPv6_CIDR;
    BOOL		CmdRWflag; /*0 for write, 1 for read*/
} CMD_Udhcpv6req;

typedef struct {
    char		Dev[IFNAME_MAX_STRLEN];
    char		LocalIpAddr[IPV6ADDR_MAX_STRLEN];
    char		GatewayAddr[IPV6ADDR_MAX_STRLEN];
    int		IPv6_CIDR;
    BOOL		CmdRWflag; /*0 for write, 1 for read*/
} CMD_Autov6req;

typedef struct {
    char		Dev[IFNAME_MAX_STRLEN];
    char		MacAddr[MACADDR_MAX_STRLEN]; /*in XX:XX:XX:XX:XX:XX form*/
    char		LocalIpAddr[IPADDR_MAX_STRLEN];
    char        HostName[MAX_HOSTNAME_LEN];
} CMD_Castlnetdhcpreq;

typedef struct {
    char		Dev[IFNAME_MAX_STRLEN];
    char		ConfFile[MACADDR_MAX_STRLEN];
    char		TftpServ[MAX_HOSTNAME_LEN];
} CMD_Castlnetconfreq;

typedef struct {
    char		Dev[IFNAME_MAX_STRLEN];
    char		LocalIpAddr[IPADDR_MAX_STRLEN];
    char		SubMaskAddr[IPADDR_MAX_STRLEN];
    BOOL		CmdRWflag; /*0 for write, 1 for read*/
} CMD_Ifconfig;

typedef enum {
    ePPP_AUTH_PAP,
    ePPP_AUTH_CHAP,
    ePPP_AUTH_CHAP_PAP,
    eNUM_OF_PPP_AUTH
} ePPP_Auth;

typedef struct {
    char		Dev[IFNAME_MAX_STRLEN];
    char		UserID[60];
    char		Password[20];
    ePPP_Auth   Auth;
    BOOL		CmdRWflag; /*0 for write, 1 for read*/
    unsigned char mode;
    unsigned char provider;
    char APN[64];
    char PhoneNum[32];
} CMD_PPPoEconfig;

/*
typedef struct
{
	unsigned int Mobileprovider;
	char Username[60];
	char Password[32];
	char APN[64];
} stDialUpCfg;
*/

typedef struct {
    char		Dev[IFNAME_MAX_STRLEN];
    char		LocalIpAddr[IPV6ADDR_MAX_STRLEN];
    char		GatewayAddr[IPV6ADDR_MAX_STRLEN];
    int			IPv6_CIDR;
    BOOL		CmdRWflag; /*0 for write, 1 for read*/
} CMD_IfconfigIPv6;

typedef struct {
    char		Dev[IFNAME_MAX_STRLEN];
    char		GateWayAddr[IPADDR_MAX_STRLEN];
    BOOL	CmdRWflag;    /*0 for write, 1 for read*/
    BOOL	Add_Del_flag; /*0 for delete, 1 for add*/
} CMD_Route;

typedef struct {
    char        HostName[MAX_HOSTNAME_LEN];
    char		DomainName[MAX_HOSTNAME_LEN * 2];
    char		DnsSrvAddr[IPADDR_MAX_STRLEN];
} CMD_DNS;

typedef struct {
    char		Dev[IFNAME_MAX_STRLEN];
    BOOL		Updownflag;
} CMD_Ipupdown;


typedef struct {
    char   targetaddr[IPADDR_MAX_STRLEN]; /*ip address in "***.***.***.***" form*/
    char   iface[IFNAME_MAX_STRLEN]; /*interface name used for arp request, such as "eth0" */
    int     timeout;                   /*time out in seconds,default 5*/
    int     maxhops;			/*default 30*/

} CMD_Traceroute;

typedef struct {
    BOOL test;
    char user[MAX_USER_PASS_LEN];
    char pwd[MAX_USER_PASS_LEN];
    char url[MAX_FILE_PATH_LEN];
} CMD_WebRequest;

typedef struct {
    BOOL download;
    int  size;
    char user[MAX_USER_PASS_LEN];
    char pwd[MAX_USER_PASS_LEN];
    char host[MAX_HOSTNAME_LEN]; /*ip address in "***.***.***.***" form*/
    char path[MAX_FILE_PATH_LEN];
    char dev[IFNAME_MAX_STRLEN];
} CMD_FtpRequest;

typedef enum {
    eICMP_Type = 1,     /* only ICMP packet*/
    eALL_Type,          /* All packet */
} enumCaptureType;

typedef struct {
    char
    iface[IFNAME_MAX_STRLEN]; /*interface name used for arp request, such as "eth0" */
    char            filter[256];              /* capture packet filter */
    int             numpkt;                   /* capture packet number */
    int             timeout;                  /* capture packet timeout */
} CMD_CapturePkt;


typedef struct {
    char host[MAX_HOSTNAME_LEN];           /*ip address in "***.***.***.***" form*/
    char file_local[MAX_FILE_PATH_LEN / 2]; /* file stored in local */
    char file_remote[MAX_FILE_PATH_LEN / 2]; /* file stored in server */
} CMD_TftpRequest;

typedef struct {
    BOOL  block;
} CMD_BlockReport;

typedef enum {
    eTestPort,
    eManagementPort,
    eNULLPort
} eIPPort;

typedef struct {
    eIPPort port;
} CMD_IPPortSwitch;

typedef enum {
    ePINGBC  = 0,
    eARP     = 1,
    eSNMP    = 2,
    ePING    = 4,
    eNETBOIS = 8
} enumIPScanOption;

typedef struct NetWiz_DoIPScanCmd {
    char          szEthName[IFNAME_MAX_STRLEN];
    unsigned char ucLocalIPArr[4];
    unsigned char ucLocalMacArr[6];
    unsigned short wScanOption;
    unsigned char szStartIP[4];
    unsigned char szStopIP[4];

} NetWiz_DoIPScanCmd;

typedef struct {
    struct UPLOAD_FILE *pUpload;
} CMD_FtpUpload;

typedef struct {
    struct USB_TRANSFER_FILE *pTransfer;
} CMD_USBTransfer;


typedef enum {
    start = 0,
    stop
} MANPORT_STATUS;

typedef struct {
    MANPORT_STATUS status;
} CMD_ManPortCheck;

typedef enum {
    eLINERATE_10BT,
    eLINERATE_100BT,
    eLINERATE_1000,
    eLINERATE_COUNT
} eLineRate;
//Auto nego mode.
typedef enum {
    eAN_DISABLE,
    eAN_ENABLE
} eAutoNego;
//Duplex mode.
typedef enum {
    eDUPMODE_Half,
    eDUPMODE_Full,
    eDUPMODE_COUNT
} eDuplexMode;

typedef enum {
    eMANUAL_MDI,
    eMANUAL_MDIX,
    eAUTO_MDIX
} eMDIMode;

//disable/enable.
typedef enum {
    eCTRL_DISABLE,
    eCTRL_ENABLE
} eControl;
//flow control
typedef enum {
    eTx_FlowCtrl,
    eRx_FlowCtrl,
    eBoth_FlowCtrl,
    eNone_FlowCtrl,
    eFlowControlNum
} eFlowControl;

typedef enum {
    ePORTTYPE_BT,
    ePORTTYPE_FX,
    NUM_OF_PORT_TYPE
} ePortType;

typedef enum {
    ePORT_1,
    ePORT_2
} ePortID;

typedef struct {
    char HalfDuplex10M;
    char FullDuplex10M;
    char HalfDuplex100M;
    char FullDuplex100M;
    char HalfDuplex1000M;
    char FullDuplex1000M;
} stMXAdvertisement;

typedef struct {
    char HalfDuplex10M;
    char FullDuplex10M;
    char HalfDuplex100M;
    char FullDuplex100M;
    char HalfDuplex1000M;
    char FullDuplex1000M;
    char RemoteFault;
    char AckKnowledge;
} stLinkPartnerAdvertisement;

typedef struct {

    eAutoNego	autoNego;
    eFlowControl	flowControl;		// for Non-AutoNego, valid only in FULL duplex mode.
    ePortType	type;
    eLineRate	speed;
    eDuplexMode duplex;
    eMDIMode autoCrossover;
    // 1: link up
    // 0: link down
    // 2: init port
    BYTE  linkstatus;
    // bit 7: speed change
    // bit 6: link status change
    // bit 5: duplex change
    // bit 4: port type change
    // bit 3: port flow control
    // bit 2: port auto-nego
    BYTE		changeItem;
    BYTE		signal_status;
    int			portId;
    stMXAdvertisement MXAdvertisement;
    stLinkPartnerAdvertisement LinkPartnerAdvertisement;
} stPortStatus;

typedef struct {
    eAutoNego autonego;
    eLineRate linerate;
    eDuplexMode duplexmode;
} CMD_ManPortSet;

typedef struct {
    char SSiD[40];
    char key[100];
    unsigned char BSSiD[20];
    unsigned char channel;  //1-11
    unsigned char encryp;
    unsigned char type;
} CMD_WiFiConnReq;


enum {
    eDataCard_Sierra,
    eDataCard_Velocity
};


typedef struct CMD_SyncServerPort {
    int nSyncServerPortNum;

} CMD_SyncServerPort;

typedef struct {
    char jobId[32];
} st_SyncCurrentJob;

typedef struct {

} CMD_BlueToothScan;

typedef struct {
    char Mac[64];
    char Name[64];
    int  Channel;
    int  itemIndex;

    char pin[5];
} CMD_BlueToothPair;

typedef struct {
    char Mac[64];
    char prefix[64];
    char uploadpath[64];
    int  channel;
} CMD_BlueToothTransfer;

typedef struct {
    char Mac[64];
    int  itemIndex;
} CMD_BlueToothService;;

typedef struct {
    char Mac[64];
    char Name[64];
    int  Channel;
    int  itemIndex;
} CMD_BlueToothServiceInfo;

typedef struct {
    //sBluetoothCmd cmd;
} CMD_Bluetooth;

typedef struct {

} CMD_Hciconfig;

typedef struct {
    char 	svrip[20];		//server IP. such as 192.168.8.33
    int		svrport;		//server port. should be 10001
    int		devType;		//device type.
    char	curver[128]; 	//current version. such as "T-SLM-Release-5.0.72"
} NU_DATA_DOWNLOAD;

enum {
    UP_DEVTYPE_CX100,
    UP_DEVTYPE_CX100P,
    UP_DEVTYPE_CX100D3,
    UP_DEVTYPE_CX100D3P,

    UP_DEVTYPE_CX350 = 100,
    UP_DEVTYPE_CX380
};




/************************************************************************/
/* New common structures must be added in this macro */
#define CMD_UNION_PUBLIC_PART      \
    ConnTestCMD   ConnTest;        \
    RTClock       SetClock;		\
    CMD_BackLight BackLight;	\
    CMD_LED	LED;			\
    CMD_Buzzer	 Buzzer;		\
    CMD_Udhcpreq UdhcpReq;      \
    CMD_Udhcpv6req Udhcpv6Req;         \
    CMD_Autov6req Autov6Req;         \
    CMD_Castlnetdhcpreq CastlnetdhcpReq; \
    CMD_Castlnetconfreq CastlnetconfReq; \
    CMD_PPPoEconfig PPPoEconfig; \
    CMD_Ifconfig	 Ifconfig;   \
    CMD_IfconfigIPv6 IfconfigIPv6;  \
    CMD_Route	 Route;   \
    CMD_DNS      DNS;     \
    CMD_Ipupdown Ipupdown;	\
    CMD_ArpRequest  ArpReq;	 \
    CMD_PingReq PingReq; \
    CMD_Traceroute	TraceRoute; \
    CMD_WebRequest	WebReq;	\
    CMD_FtpRequest	FtpReq; \
    CMD_TftpRequest	TftpReq; \
    CMD_CapturePkt  CapturePkt; \
    CMD_BlockReport BlockReq;	\
    CMD_VOIPReq VoipReq;	\
    CMD_VOIPpack Voipcmd; \
    CMD_IPTVReq IptvReq; \
    CMD_IPPortSwitch Port; \
    CMD_FANCTRL Fanctrl; \
    CMD_STANDBY	StandBy; \
    NetWiz_DoIPScanCmd IPScan; \
    CMD_FtpUpload FtpUpload; \
    CMD_ManPortCheck ManPortCheck; \
    CMD_ManPortSet ManPortSet;	\
    VOIP_SETUP *VoipSetup;	\
    CMD_WiFiConnReq WiFiConnReq; \
    CMD_USBTransfer USBTransfer; \
    CMD_USBUmount USBUmount; \
    CLONEFILESEL CloneFileSel; \
    CMD_SyncServerPort    SyncServerPort; \
    CMD_SyncUpload        SyncUpload; \
    CMD_SyncDeviceInfo    SyncDevInfo; \
    CMD_SyncDownProfiles  SyncProfiles; \
    CMD_DataGPSReq DataGPSReq; \
    CMD_SyncWorkForceStatus SyncWorkForceStatus; \
    CMD_SyncWorkForceTech	SyncWorkForceTech; \
    CMD_SyncGpsData			SyncGpsData; \
    CMD_SyncGpsStatus		SyncGpsStatus;\
    CMD_SyncJobsStatus SyncJobsStatus;\
    CMD_SyncJobsManageStatus SyncJobsManageStatus;\
    CMD_SyncClosedJob   SyncClosedJob;\
    CMD_SyncCurrentJob  SyncCurrentJob;\
    CMD_BlueToothScan   BlueToothScanReq;\
    CMD_BlueToothPair   BlueToothPairReq;\
    CMD_BlueToothTransfer   BlueToothTransferReq;\
    CMD_BlueToothService	BlueToothServiceReq;\
    CMD_Bluetooth	BlueToothReq;\
    CMD_Hciconfig   HciconfigReq;\
    NU_DATA_DOWNLOAD	netupdate;\
    CMD_Autodhcpreq    AUTODHCPReq;
/************************************************************************/

/* Command definition */

#define PUBCMD_HEADER_SIZE (8)


/************************************************************************/
/* Define the results processed by architecture.                        */
/************************************************************************/

typedef enum enumResult {
    eRetConnTest = 0,

    eRetBattery,
    eRetClock,

    eRetFPGAStart,      //Init start, no param
    eRetFPGAProg,	    //Init progress, has param int (download X%)
    eRetFPGAOK,			//Init OK, has param int (download error no)
    eRetFPGAErr,		//Download

    eRetInitOK,			//Back engine Init OK
    eRetResumeOK,

    /* More results can be added here */

    eSysEvent,		//System events

    eRetDhcp,            //dhcp result
    eRetAutoDhcp,        //auto dhcp result
    eRetDhcpv6,          //dhcpv6 result
    eRetAutov6,          //autov6 result
    eRetCastlnetUdhcpc,	//Castlnet MTA dhcp result

    eRetCastlnetConfig,	//Castlnet MTA configuation for SIP service

    eRetArpReq,        //Arp result
    eRetPingReq,       //ping result


    eRetTraceReq,	//traceroute

    eRetPcapReq,    // pcap response

    eRetIpupdownReq,

    eRetFtpReq,

    eRetTftpReq,

    eRetWebReq,

    eRetIfconfigReq,
    eRetIfconfigIPv6Req, //ifconfig ipv6 result

    eRetRouteReq,

    eRetDnsReq,
    eRetPPPoEReq,

    eRetDiskSpace,	//disk space
    eRetPubVCTFe,
    eRetCVTAdjustFe,
    eRetVoip,
    eRetIptv,
    eRetFtpUpload, //ftp upload
    eRetUSBTranMount,
    eRetUSBTranDetect,
    eRetUSBTranUmount,
    eRetUSBTranUmount_FileUpload,
    eRetManPort,
    eRetUSBTransfer,

    eRetNetWiz_IPScan,
    eSumNetWiz_IPScan,

    eRetRegStatus,
    eRetCallStatus,
    eRetInComingCall,
    eRetUnregFinished,

    eRetCloneRecv,
    eRetCloneStart,
    eRetCloneOver,

    eRetOhRegStatus,
    eRetOhCallStatus,
    eRetOhInComingCall,
    eRetOhUnregFinished,

    eRetWifiConnetivity,
    eRetWifiScan,
    eRetWifiConnectReq,
    eRetWifiMonitor,
    eRetWifiMsg,
    eRetWifiServiceStatus,
    eRetWifiExitReady,

    // Corresponding result structure is sync2server.h::st_SyncStatus
    // And refer to sync2server.h::enumSyncStatus
    eRetSyncStatus = 100,

    // Corresponding result structure is sync2server.h::st_SyncStatus.
    // Refer to sync2server.h::enumSyncStatus :
    // eSyncSucceed --> Successfully, others --> failed
    eRetSyncRegister,
    eRetSyncUnRegister,
    eRetSyncCheck,

    // Attached structure is sync2server.h::st_DeviceInfo.
    eRetSyncDevInfo,

    // Attached structure is sync2server.h::st_SyncServerInfo.
    eRetSyncServerInfo,

    // Attached structure is st_AdminSetResult
    eRetSyncAdminSet,

    eRetLockRegStatus,//st_LockRegStatus		SyncLockStatus;
    eRetDeviceInfoExt,//st_DeviceInfoExt          SyncDevInfoExt;
    eRetOrgChartStatus,//st_OrgChartStatus		SyncOrgStatus;
    eRetOrgChartInfo, //st_OrgChartInfo			SyncOrgInfo;

    // Corresponding result structure is sync2server.h::st_SyncStatus.
    // Refer to sync2server.h::enumSyncStatus :
    // eSyncSucceed --> Successfully, others --> failed
    eRetSyncDownload,

    // Get all profile info from server.
    // Corresponding result structure is sync2server.h::st_ProfileInfoArray.
    eRetGetAllProfInfo, //112

    // Corresponding result structure is sync2server.h::st_SoftwareVersion.
    eRetSyncSoftwareVer, //113

    eRetDataGPSDataSignal,
    eRetDataGPSGPSTracking,
    eRetDataGPSCardDetect,
    eRetDataGPsAutoConnectStatus,

    eRetWorkForceStatus,
    eRetSynGpsConfig,
    eRetWorkForceTech,	//120
    eRetSyncGpsData,
    eRetSyncGpsStatus,
    eRetJobsManageStatus,
    eRetJobsStatus,
    eRetJobsDetailsStart,
    eRetJobsDetails,
    eRetSwitchInterfaceOK,
    eRetJobs,
    eRetBluetoothInit,
    eRetBluetoothScan,	//130
    eRetBluetoothPair,
    eRetBluetoothTransfer,
    eRetBluetoothService,
    eRetBluetoothDialup,
    eRetBluetoothServiceInfo,
    eRetHciconfig,
    eRetBTAutoDialUpStatus,
    eRetBluetoothPairAuto,
    eRetBluetoothServiceAuto,

    eRetNetUpdater, //140
    eRetUSBUpdater,

    eRetIncUpgradeFailure,

    eRetPubMax = 999

} enumRET;

typedef struct ConnTestRet {
    unsigned int  uTestID;
    unsigned char Reserved[RESERVED_CHARS_COUNT];

} ConnTestRet;

typedef struct BattInfo {
    int nTemp;
    DWORD   BettTime;
    WORD   	BettPercent;
    BYTE	PowerSrc;
    BYTE	BettCheck;

} BattInfo;

typedef struct FPGADnProg {
    /* Progress in percent, example: 80 -> 80% */
    // on return code in eRetFPGAOK.
    DWORD nProgPercent;


} FPGADnProg;

// system event type define
typedef struct st_SysEvent {
    char time[8];			//test event happen time
    unsigned char event;	//test event, such as "CRC Errors", "IP Chksum"
    unsigned char type;		//test type, such as "BERT", "Throughput"
    int Count;				//event count, such as number of errros in the event
} SysEvent;

typedef enum {
    eARP_STATUS_UNKNOWN = 0,
    eARP_STATUS_PASS,
    eARP_STATUS_INPROGRESS,
    eARP_STATUS_FAILED,
    eARP_STATUS_TIMEOUT
} eArpStatus;



typedef enum {
    eIfconfig_STATUS_UNKNOWN = 0,
    eIfconfig_STATUS_INPROGRESS,
    eIfconfig_STATUS_UP,
    eIfconfig_STATUS_DOWN
} eIfconfigRetStatus;

typedef struct {
    char		Dev[IFNAME_MAX_STRLEN];
    char		LocalIpAddr[IPADDR_MAX_STRLEN];
    char		SubMaskAddr[IPADDR_MAX_STRLEN];
    eIfconfigRetStatus		status;
    BOOL	RWflag;

} IfconfigResult;

typedef struct {
    char		Dev[IFNAME_MAX_STRLEN];
    char		LocalIpAddr[IPADDR_MAX_STRLEN];
    char		SubMaskAddr[IPADDR_MAX_STRLEN];
    eIfconfigRetStatus		status;
    BOOL	RWflag;
} IfconfigIPv6Result;

typedef enum {
    ePPPoE_STATUS_UNKNOWN = 0,
    ePPPoE_STATUS_INPROGRESS,
    ePPPoE_STATUS_UP,
    ePPPoE_STATUS_DOWN
} ePPPoERetStatus;

typedef struct {
    char		Dev[IFNAME_MAX_STRLEN];
    char		LocalIPAddr[IPADDR_MAX_STRLEN];
    char		GateWayAddr[IPADDR_MAX_STRLEN];
    char        NameSRVAddr[IPADDR_MAX_STRLEN];

    ePPPoERetStatus		status;
    BOOL	RWflag;
} PPPoEResult;

typedef enum {
    eUDHCP_STATUS_UNKNOWN = 0,
    eUDHCP_STATUS_INPROGRESS,
    eUDHCP_STATUS_PASS,
    eUDHCP_STATUS_TIMEOUT,
    eUDHCP_STATUS_FAILED
} eUdhcpRetStatus;

typedef struct {
    eUdhcpRetStatus status;
    char		Dev[IFNAME_MAX_STRLEN];
    char        HostName[MAX_HOSTNAME_LEN];
    char		LocalIpAddr[IPADDR_MAX_STRLEN];
    char		SubMaskAddr[IPADDR_MAX_STRLEN];
    char		GateWayAddr[IPADDR_MAX_STRLEN];
    char        NameSRVAddr[IPADDR_MAX_STRLEN];
    char		DomainName[MAX_HOSTNAME_LEN * 2];
    char        LeaseTime[32];
    char		BootFile[128];
    char		TftpServer[MAX_HOSTNAME_LEN];
} UdhcpResult;

typedef struct {
    eUdhcpRetStatus status;
    char		Dev[IFNAME_MAX_STRLEN];
    char		LocalIpAddr[IPADDR_MAX_STRLEN];
} AutodhcpResult;

typedef enum {
    eUDHCPV6_STATUS_UNKNOWN = 0,
    eUDHCPV6_STATUS_INPROGRESS,
    eUDHCPV6_STATUS_PASS,
    eUDHCPV6_STATUS_TIMEOUT,
    eUDHCPV6_STATUS_FAILED
} eUdhcpv6RetStatus;

typedef struct {
    eUdhcpv6RetStatus status;
    char	Dev[IFNAME_MAX_STRLEN];
    char    HostName[MAX_HOSTNAME_LEN];
    char	LocalIpAddr[IPV6ADDR_MAX_STRLEN];
    char	SubMaskAddr[IPV6ADDR_MAX_STRLEN];
    char	GateWayAddr[IPV6ADDR_MAX_STRLEN];
    char	NameSRVAddr[IPV6ADDR_MAX_STRLEN];
    char	DomainName[MAX_HOSTNAME_LEN * 2];
    char	LeaseTime[32];
    int     cidr;
    BOOL	RWflag;
} Udhcpv6Result;

typedef enum {
    ePING_STATUS_UNKNOWN = 0,
    ePING_STATUS_INPROGRESS,
    ePING_STATUS_PASS,
    ePING_STATUS_FAILED
} ePingRetStatus;

typedef struct {
    ePingRetStatus status;
    char dst_str[128];
    int    sent;
    int    received;
    int    unreach;
    int    missing;
    int    rt_current;
    int    rt_average;
    int    rt_max;
    int    rt_min;
    int    dup;
    int    rt_avg;
} PingResult;

typedef enum {
    eCNCONF_STATUS_UNKNOWN = 0,
    eCNCONF_STATUS_INPROGRESS,
    eCNCONF_STATUS_PASS,
    eCNCONF_STATUS_FAILED
} eCastlnetConfRetStatus;

typedef struct {
    eCastlnetConfRetStatus status;
    char		Dev[IFNAME_MAX_STRLEN];
    char		SipProxyAddr[MAX_HOSTNAME_LEN];
    char		SipProxyPort[16];
    char		SipRegAddr[MAX_HOSTNAME_LEN];
    char		SipRegPort[16];
    char		SipUser1Name[MAX_USER_PASS_LEN];
    char		SipUser1Id[MAX_USER_PASS_LEN];
    char		SipUser1Pw[MAX_USER_PASS_LEN];
} CastlnetConfResult;

typedef enum {
    eIp_STATUS_UNKNOWN = 0,
    eIp_STATUS_PASS,
    eIp_STATUS_FAILED
} eIpupdownRetStatus;


typedef struct {
    eIpupdownRetStatus		status;


} IpupdownResult;

typedef enum {
    eRoute_STATUS_UNKNOWN = 0,
    eRoute_STATUS_INPROGRESS,
    eRoute_STATUS_PASS,
    eRoute_STATUS_FAILED,
} eRouteRetStatus;


typedef struct {
    char		Dev[IFNAME_MAX_STRLEN];
    char		GateWayAddr[IPADDR_MAX_STRLEN];
    eRouteRetStatus		status;
    BOOL	RWflag;

} RouteResult;

typedef enum {
    eDNS_STATUS_UNKNOWN = 0,
    eDNS_STATUS_INPROGRESS,
    eDNS_STATUS_PASS,
    eDNS_STATUS_FAILED
} eDNSRetStatus;

typedef struct {
    eDNSRetStatus status;
} DNSResult;

typedef enum {
    eTraceR_STATUS_UNKNOWN = 0,
    eTrace_STATUS_PASS,
    eTrace_STATUS_FAILED,
    eTrace_STATUS_TIMEOUT
} eTraceStatus;

typedef struct {
    eTraceStatus     status;
    char  ip[IPADDR_MAX_STRLEN];   /*ip address in ***.***.***.*** form*/
    int avgetime;/*ms*/
    int hopnum;
    char  tgt_ip[IPADDR_MAX_STRLEN]; /*destination ip address in ***.***.***.*** form*/
} TraceResult;

typedef struct {
    int space;
} DiskResult;

typedef enum {
    eFtp_STATUS_UNKNOWN = 0,
    eFtp_STATUS_INPROGRESS,
    eFtp_STATUS_PASS,
    eFtp_DNSERR,
    eFtp_INVALIDSITE,
    eFtp_INVALIDNAME,
    eFtp_INVALIDPATH,
    eFtp_TIMEOUT,
    eFtp_STATUS_CONNECTING,
    eFtp_STATUS_CONNECTED,
    eFtp_STATUS_DOWNLOADING,
    eFtp_STATUS_UPLOADING,
    eFtp_STATUS_RETR,
    eFtp_STATUS_DOWNFAIL,
    eFtp_STATUS_UPFAIL,
    eFtp_STATUS_Num,
} eFtpStatus;

typedef enum {
    eUSB_TRAN_MOUNT_SUCCESS = 0,
    eUSB_TRAN_MOUNT_FAIL
} eUSBTranMount;

typedef enum {
    eWeb_STATUS_UNKNOWN = 0,
    eWeb_STATUS_INPROGRESS,
    eWeb_PASS,
    eWeb_INVALIDSITE,
    eWeb_INVALIDNAME,
    eWeb_INVALIDPATH,
    eWeb_TIMEOUT
} eWebStatus;

typedef struct {
    eFtpStatus status;
    size_t     transdata;       /*transfered data size, in bytes*/
    int        averageput;      /*transfer speed, in bytes/sec*/
    int        logintime;       /*login to get|put time, in ms*/
    int        totaltime;       /*total transfer time, in ms*/
} FtpResult;

typedef enum {
    eTftp_STATUS_UNKNOWN = 0,
    eTftp_STATUS_INPROGRESS,
    eTftp_PASS,
    eTftp_FAIL,
    eTftp_NO_FILE,
    eTftp_VIOLATION,
    eTftp_TIMEOUT
} eTftpRetStatus;

typedef struct {
    eTftpRetStatus status;
} TftpResult;

typedef struct {
    eWebStatus status;
    int			size;//download data size(bytes)
    unsigned long speed;//download spped(bytes per second)
    unsigned long time;//download time(ms)
    unsigned long restime;//response time(ms)

} WebResult;

typedef enum {
    ePCAP_STATUS_UNKNOWN = 0,
    ePCAP_STATUS_INIT,
    ePCAP_STATUS_INPROGRESS,
    ePCAP_STATUS_PASS,
    ePCAP_STATUS_FAILED
} ePCapStatus;

typedef struct {
    ePCapStatus    status;
    int            pktno;
    char           time[8];
    char           src_ip[IPADDR_MAX_STRLEN];
    int            src_port;
    char           dst_ip[IPADDR_MAX_STRLEN];
    int            dst_port;
    char           src_mac[MACADDR_MAX_STRLEN];
    char           dst_mac[MACADDR_MAX_STRLEN];
    char           proto[32];
    int            packet_len;  /* length of portion present */
} PcapResult;


typedef struct {
    eUSBTranMount m_status;
} stUSBTransMountRet;

typedef enum {
    eMANPORT_STATUS_LINKDOWN = 0,
    eMANPORT_STATUS_LINKUP
} eMANPORT_STATUS;

typedef enum {
    eMANPORT_STATUS_10,
    eMANPORT_STATUS_100,
    eMANPORT_STATUS_1000,
} eMAMPORT_SPEED_TYPE;

typedef enum {
    eMANPORT_STATUS_HALF,
    eMANPORT_STATUS_FULL
} eMANPORT_DUPLEX_TYPE;

typedef struct {
    eMANPORT_STATUS m_status;
    eMAMPORT_SPEED_TYPE m_speedtype;
    eMANPORT_DUPLEX_TYPE m_duplextype;
} stManPortRet;

typedef enum {
    eStarted = 0,
    eInProc,
    eFinished
} enumIPScanStat;

typedef enum {
    eOK = 0,
    eFAIL,
    eUNKOWN
} enumScanPingStat;

typedef struct NetWiz_IPScanRet {
    enumIPScanStat status;

    int  nMachineIndex;
    char cIPArr[4];
    char cMacArr[6];
    char szGroupName[16];
    char szMachineName[16];
    char szAttribute[32];
    enumScanPingStat PingStat;

} NetWiz_IPScanRet;

typedef struct NetWiz_IPScanSum {
    unsigned long ulTxFrames;
    unsigned long ulRxFrames;
    unsigned long ulRxErrors;

    eMAMPORT_SPEED_TYPE  SpeedType;
    eMANPORT_DUPLEX_TYPE DuplexType;

    unsigned int  uDevicesFound;
    unsigned int  uNetworksFound;

} NetWiz_IPScanSum;

typedef struct incoming_call {
    char url[50];

} IN_COMING_CALL;

//WiFi
typedef struct {
    unsigned char numSSiD;
    unsigned char numChannel;
    unsigned char numinfrastructure;
    unsigned char numAdHoc;
} SWiFiScanRslt;

typedef struct {
    unsigned char cellID;
    char SSiD[40];
    unsigned char BSSiD[20];
    int           channel;
    unsigned char encryption;
    unsigned char type;
    unsigned char signal;
    unsigned char linkQuality;
    unsigned char txRate;
    int bitRates;
} SWiFiScanItem;


//Roman add end

#define MAX_WIFI_SCAN_ITEM	64

typedef struct {
    unsigned char scannedNum;
    SWiFiScanItem list[MAX_WIFI_SCAN_ITEM];
} SWiFiScanList;

typedef struct {
    SWiFiScanRslt result;
    SWiFiScanList APList;
} SWiFiScan;

typedef struct {
    unsigned char connStatus;
    char 			SSID[40];
    unsigned char BSSiD[20];
    unsigned char signal;
    unsigned char linkQuality;
    unsigned char txRate;
    char 			description[80];
} SWiFiConnStatus;

enum {
    eWifi_NO_USB_ADAPTER,
    eWifi_ADAPTER_DETECTED,
    eWifi_NO_SSID,
    eWifi_INVALID_KEY,
    eWifi_INVALID_SSID,
    eWifi_CONNECTION_LOST,
    eWifi_RECONNECTED,
    eWifi_USB_CONFIGURED,
    eWifi_USB_OFF_READY
};

typedef enum {
    eWifiRalink,
    eWifiZyDAS,
    eNoOfWifiDrivers
} eWifiDriver;

//Generic Ethernet Port Definition, Applicable to both base chassis and ethernet module

typedef struct {
    unsigned int dwType;
    void		 *pReserved;

    VCTRESULT    vct;
    stPortStatus port;

} EtherFeetResult;

#define VCT_CHECK_SUCCESS 	0
#define VCT_CHECK_FAIL   	-1
#define VCT_NULL_PTR_ERR 	-2

typedef struct st_SyncStatus {
    // Refer to sync2server.h -> enumSyncStatus
    int nSyncStatus;

} st_SyncStatus;


//Data card
typedef struct {
    int broadbandSig;
    unsigned char connStatus;
    unsigned char cardDetect;
} SDataCardRslt;

typedef enum {
    eCmdBLUETOOTH_UNKNOWN = 0,
    eCmdBLUETOOTH_SCANING,
    eCmdBLUETOOTH_SCANED,
    eCmdBLUETOOTH_PAIRING,
    eCmdBLUETOOTH_PAIRED,
} eBlueToothCmdStatus;

typedef enum {
    eBLUETOOTH_STATUS_UNKNOWN = 0,
    eBLUETOOTH_STATUS_INPROGRESS,
    eBLUETOOTH_STATUS_PASS,
    eBLUETOOTH_STATUS_TIMEOUT,
    eBLUETOOTH_STATUS_FAILED
} eBlueToothRetStatus;

typedef struct bluetooth_session {
    char Mac[64];
    char Name[64];
    int  Channel;
} bluetooth_session;

typedef struct {
    eBlueToothRetStatus status;
    int    itemNumber;
    int    isLink;
    bluetooth_session bt_session[20];
} BlueToothResult;

typedef struct {
    eBlueToothRetStatus status;
    int	itemNumber;

    struct {
        int UUID;
        int channel;
    } service[30];
} BlueToothService;

typedef struct {
    eBlueToothRetStatus status;
    char Mac[64];
    char Name[64];
    int  Channel;
    PPPoEResult ppp;
} BlueToothDialup;

typedef struct {
    eBlueToothRetStatus status;
    char Mac[64];
    char Name[64];
    int  Channel;

} BluetoothServiceInfo;

typedef enum {
    eHCICONFIG_UNKNOWN = 0,
    eHCICONFIG_INPROGRESS,
    eHCICONFIG_PASS,
} eHciconfigCmdStatus;

typedef struct {
    int  isLink;
    char type[64];
    char name[64];
    char mac [64];
    char manufac[128];
} HciconfigResult;

/************************************************************************/
/* New common structures must be added in this macro */
#define RET_UNION_PUBLIC_PART     \
    ConnTestRet  CONNTEST;        \
    BattInfo         BATT;            \
    RTClock         RTC;             \
    FPGADnProg   FPGAProg;        \
    SysEvent        SYSEvent;  \
    UdhcpResult     UdhcpRet;   \
    AutodhcpResult  AutodhcpRet;   \
    Udhcpv6Result     Udhcpv6Ret;   \
    CastlnetConfResult CastlnetConfRet;	\
    ArpResult        ArpRet;	\
    PingResult       PingRet;	\
    TraceResult	TraceRet;	\
    DiskResult	DiskRet;	\
    FtpResult		FtpRet;	\
    TftpResult		TftpRet;	\
    WebResult	WebRet;	\
    IfconfigResult	IfconfigRet;	\
    IfconfigIPv6Result	IfconfigIPv6Ret;	\
    RouteResult	RouteRet;	\
    DNSResult    DNSRet;     \
    PPPoEResult	PPPRet; \
    IpupdownResult	IpupdownRet;		\
    VCTRESULT fzRet;	\
    stVoipRet voipRet;	\
    struct iptv_report iptvRet; \
    EtherFeetResult  CommonVctRet; \
    NetWiz_IPScanRet  IPScanRet; \
    NetWiz_IPScanSum  IPScanSum; \
    PcapResult   PcapRet;    \
    stFtpUploadRet ftpuploadRet; \
    stManPortRet manportRet; \
    stUSBTransferRet usbTransferRet; \
    stUSBTransMountRet usbTransMountRet; \
    IN_COMING_CALL                  IncomingRet; \
    SIP_GLB_STA	GlbStatus; \
    SWiFiScan *wifiListRet;	\
    SWiFiConnStatus wifiConnRet; \
    unsigned char wifiMonRet[200]; \
    st_SyncStatus          SyncStatus;   \
    st_AdminSetResult      SyncAdminSet; \
    st_LockRegStatus		SyncLockStatus; \
    st_DeviceInfo          SyncDevInfo;  \
    st_DeviceInfoExt          SyncDevInfoExt;  \
    st_OrgChartStatus		SyncOrgStatus; \
    st_OrgChartInfo			SyncOrgInfo;  \
    st_SyncServerInfo      SyncServerInfo; \
    st_ProfileInfoArray    SyncProfInfoArray; \
    st_SoftwareVersion     SyncSoftwareVer;   \
    SDataCardRslt datacardRet; \
    st_WorkForceStatus  workforce; \
    st_JobsStatus jobsStatus;\
    st_JobsManageStatus jobsManageStatus;\
    SGPSLoc gpsRet; \
    st_SyncGpsConfig SyncGpsConfig; \
    st_JobsDetailsPtr jobsDetailsPtr; \
    BlueToothResult     BlueToothRet; \
    BlueToothService	BlueToothServ; \
    HciconfigResult     HciconfigRet; \
    unsigned char resv[64];

/************************************************************************/



// profile header structure

typedef struct {
    unsigned short	moduleType;		//0x0002 for ethernet.
    unsigned int	fileSize;
    char			fileName[8];

    unsigned short	type;
    char			typeName[8];

    unsigned int	lastFixedTime;
    unsigned char	isLocked;		// 0:non-lock, other locked
} stProfileHdr;


typedef struct {
    int item;
    int data;
} stTestConfig;

/************************************************************************/
/* New common structures must be added in this macro */
#define DATA_UNION_PUBLIC_PART     \
    stTestConfig  TEST_PROFILE;        \

    /************************************************************************/

    typedef struct {
        stProfileHdr	header;

        union {
            DATA_UNION_PUBLIC_PART;
        } DATA;
    } stProfile;

    typedef enum {
        eSingleAnalog = 1,
        eSingleDigital,
        eInschk,
        esysscan,
        efpingress,
        ereversepath,
        eCableModem,
        eTilt,
        eReverse,
        eCMPing,
        eCMWebftp,
        eCMIptv,
        eCMVoip,
        eCMTrace,
        eHip,
        eUpRange,
        eRPBalance,
        eHipat,
        eLeakageDetector,
        eHipCm,
        eOFDM,
        eISS,
        eCableModemD3,
        eHipD3,
        eHipatD3,

        eCommPing = 300,
        eCommWebftp,
        eCommIptv,
        eCommVoip,
        eCommTrace,
        eCommArp,
        eCommNetwizard,
        /* More result type should be added here */
    } enumSLMResultType;



    //defines for eRetNetUpdater
#define	NU_STAT_MASK		(0xFF000000)
#define	NU_STAT_ERR			(0x10000000)
#define	NU_STAT_DOWN		(0x20000000)
#define	NU_STAT_UPDATE		(0x40000000)
#define	NU_STAT_DONE		(0x50000000)


#define NU_ERR_TIMEOUT		(0x01)	//Timeout when receiving data
#define NU_ERR_VER			(0x02)	//version is not support
#define NU_ERR_MD5			(0x03)	//doanload failed, checksum error
#define NU_ERR_UNTAR		(0x04)	//uncompress failed
#define NU_ERR_INTERNAL		(0x05)	//Internal error
#define NU_ERR_SEND			(0x06)	//Send command failed
#define NU_ERR_RECV			(0x07)	//Receive data failed
#define NU_ERR_CURRENT		(0x08)	//Version already the newest.
#define NU_ERR_BUSY			(0x09)	//Server is busy, please try later
#define NU_ERR_WRITE		(0x0A)	//write to memory failed.
#define NU_ERR_NETWORK		(0x0B)	//Network error

    ///////


    typedef struct {
        float start;
        float stop;
    } stFreqMask;

    typedef struct {
        int	maskCount;
        stFreqMask freqMask[10];
    } stFreqMaskInfo;

    typedef struct {
        char channel[12][10];
    } stIC_AnalogInfo;

    typedef struct {
        char channel[12][10];
    } stIC_DigitalInfo;

    typedef struct {
        stIC_AnalogInfo analog;
        stIC_DigitalInfo digital;
    } stInstallationCheckInfo;

    enum HipMode {
        eHipModeBasic,
        eHipModeAutoTest,
        eHipModeComcast,
        eHipC_M,
        eHipInvalid,
        eHipModeCount,
    };
    typedef enum {
        eDefaultMac = 0,
        eManualMac
    } enum_MacMode;


#pragma pack(8)
    typedef struct {
        char sChannelTableName[20];
        char sTapThreshold[20];
        char sGroundBlockThreshold[20];
        char sEMTAThreshold[20];
        char sCableModemThreshold[20];
        char sDigitalBoxThreshold[20];
        char sVideoThreshold[20];
        char sDOCSISChannel[10];
        char sDOCSISUploadChannel[10];
        float fIngressNoiseLevel;
        stFreqMaskInfo stMask;
        stInstallationCheckInfo stIC;
        enum HipMode type;//
        char sAutoTable[20];

        enum_MacMode       MacMode_DTV;
        enum_MacMode       MacMode_ISP;
        enum_MacMode       MacMode_VOIP;
        char defaultMacAddress[20];// Local MAC, Get from Taget.
        char manualMacAddress_DTV[20];//
        char manualMacAddress_ISP[20];//
        char manualMacAddress_VOIP[20];//

        char DTV1_Ch[10];
        float DTV1_Freq;
        bool DTV1_MER_Enable;
        float DTV1_BerDuration;

        char DTV2_Ch[10];
        float DTV2_Freq;
        bool DTV2_MER_Enable;
        float DTV2_BerDuration;

        char DTV_CM_Ch[10];
        float DTV_CM_Freq;
        bool DTV_CM_MER_Enable;
        float DTV_CM_BerDuration;

        char ISP_CM_Ch[10];
        float ISP_CM_Freq;
        bool ISP_CM_MER_Enable;
        float ISP_CM_BerDuration;

        char VOIP_CM_Ch[10];
        float VOIP_CM_Freq;
        bool VOIP_CM_MER_Enable;
        float VOIP_CM_BerDuration;

        bool DocsicTestEnable;
        bool IngressTestEnable;
        bool VoipTestEnable;
        bool TwoWaySplitterEnable;
        int i2WaySplitterDb;
        bool StepByStepEnable;
        int Encoding;//0:G.711U, 1:G.711A, 2:G.723, 3:G.729
        int TestDuration;
        int JitterBuffer;

        /* for speed test */
        int ookla_providerindex;
        int ookla_svrindex;
        bool bSpeedTest;//on 1, off 0

        float f2WaySplitterDb;
    } stHipProfile;

    enum MODULATION {
        NTSC,
        QAM64,
        QAM256,
        PAL,
        SECAM,
        PALSECAM, // PAL/SECAM
        DOCSIS,
        OOB,
        QAM128,
        QAM16,
        QPSK,
        VSB8,
        OFDM,
        VOD64,
        VOD256,
        NULL_MOD,
    };

    struct channel_analog {
        unsigned int video;
        unsigned int audio1;
        unsigned int audio2;
        unsigned int freq;
        bool benable;
        bool bscramble;
    };

    struct channel_digital {
        unsigned int freq;
        unsigned int symbol;
        bool benable;
        bool binverted;
        float symbol_save;
    };

    typedef enum {
        eChannelNull,
        eChannelDefault,
        eChannelE,
        eChannelS,
        eChannelZ,
        eChannelDS,
        eChannelC,
        eChannelU,
    } eChannelType;

    typedef struct {
        char lable[12];
        int ChanelNum;
        eChannelType type;
        char ChannelName[8];
        bool bMode;   // analog == true and digital == false
        enum MODULATION modulation;
        struct channel_analog  analog;
        struct channel_digital digital;
        int nAnnexType;//0==AnnexA-8M, 1==AnnexB-6M, 2==AnnexC-8M;
    } standard_channel;

    typedef enum {
        eCHECKED,
        eID,
        eLABEL,
        eMODULATION,
        eVIDEO,
        eAUDIO1, //5
        eAUDIO2, //6
        eSCRAMBEL, //7
        eFREQUENCY,//8
        eSYMBOL_RATE,//9
        eINVERTED,//10
        eAnnex,//11 the last one. if you
        eMAX,
    } eChannelTableCol;


#define QAM64SYMBOL_USA		5057000
#define QAM256SYMBOL_USA	5361000
#define QPSK_SYMBOL			1544000

#define QAM64SYMBOL_EUROPE	6952000
#define QAM256SYMBOL_EUROPE	6952000

#define QAM64SYMBOL_CHINA		6875000
#define QAM256SYMBOL_CHINA	6875000

#define QAM64SYMBOL_AnnexC		5274000
#define QAM256SYMBOL_AnnexC	    5274000
#define QPSK_SYMBOL_AnnexC		1544000


    typedef enum enumScanChanType {
        eScanChanAnalog = 0,
        eScanChanDigital,

        eScanChanNull

    } enumScanChanType;


    typedef struct FastSysScanAnalogItem {
        /* 4 bytes equal to the desired analog channel video carrier frequency in Hz */
        unsigned int uAnalogVideoFreq;

        /* 4 bytes equal to the desired analog channel primary audio carrier frequency in Hz */
        unsigned int uAnalogPrimaryAudioFreq;

    } FastSysScanAnalogItem;

    typedef struct FastSysScanDigitalItem {
        /* 4 bytes equal to the desired QAM channel center frequency in Hz */
        unsigned int uQAMCenterFreq;

        /* 3 bytes equal to bandwidth (symbol rate) of QAM channel */
        unsigned int uQAMSymbolRate;

    } FastSysScanDigitalItem;


    typedef struct FastSysScanItem {
        int              nChannelNumber;
        int              type;
        int              ChannelIndex;
        enumScanChanType ScanChanType;

        union {
            FastSysScanAnalogItem     AnaScanItem;
            FastSysScanDigitalItem    DigScanItem;
        };
    } FastSysScanItem;

    typedef struct FastSystemScanConfig {
        /* 0: Do not report continuously, 1: Report continuously */
        char             cReportType;

        FastSysScanItem  ArrFastSysScanItem[6];

    } FastSystemScanConfig;
    typedef FastSystemScanConfig *P_FastSystemScanConfig;

    typedef struct iOS_FastSystemScanConf {
        /* Please refer to enumCMD in commif.h and enumCmdType */
        unsigned int dwType;
        void         *pReserved;

        union {
            FastSystemScanConfig         FastSystemScanConf;
        };
    } iOS_FastSystemScanConf;

    ////recive
    typedef struct FastScanAnaRetItem {
        /* 1 byte equal to the integer value of channel n video carrier level measurement */
        /* 1 byte equal to the fractional value of channel n video carrier level measurement */
        char cVideoLevel[2];

        /* 1 byte equal to the integer value of channel n primary audio carrier level measurement */
        /* 1 byte equal to the fractional value of channel n primary audio carrier level measurement */
        char cPriAudioLevel[2];

    } FastScanAnaRetItem;

    typedef struct FastScanDigRetItem {
        /* 1 byte equal to the integer value of channel n QAM level measurement */
        /* 1 byte equal to the fractional value of channel n QAM level measurement */
        char cQAMLevel[2];

    } FastScanDigRetItem;

    typedef struct FastSysScanRetItem {
        int              nChannelNumber;
        int              type;
        int              ChannelIndex;
        enumScanChanType ScanChanType;

        union {
            FastScanAnaRetItem    AnaScanRetItem;
            FastScanDigRetItem    DigScanRetItem;
        };
    } FastSysScanRetItem;

    typedef struct FastSysScanResult {
        FastSysScanRetItem  ArrFastSysScanRetItem[6];

    } FastSysScanResult;
    typedef FastSysScanResult  *P_FastSysScanResult;

    typedef struct DigInstChkRetItem {
        /* Channel Number */
        int nChannelNumber;

        /* Channel type */
        int type;

        /* index */
        int index;

        /* 2 bytes equal to the integer (1st byte) and fractional (2nd byte) values of the digital carrier average level in dBmV */
        char cArrDigAveLevel[2];

        /* 2 bytes equal to the integer (1st byte) and fractional (2nd byte) values of the max delta between the digital
         carrier level and the adjacent channel levels in dB */
        char cArrMaxDeltaBetwDigAdj[2];

        /* 1 byte equal to 00000000 if demod lock to QAM signal was unsuccessful or 00000001 if demod is lock to QAM signal was successful */
        char bQAMLockOK;

        /* 2 bytes equal to the integer (1st byte) and fractional (2nd byte) values of the MER in dB */
        char cArrMER[2];

        unsigned short wPreFECErrs;
        unsigned short wPostFECErrs;

        float fPreFECErrRate;
        float fPostFECErrRate;

    } DigInstChkRetItem;

    typedef struct DigInstCheckResult {
        DigInstChkRetItem  DigRetArr[6];

    } DigInstCheckResult;

    typedef struct iOS_AutoTestResult {
        /* Please refer to enumRET in commif.h and enumReadDatatType */
        unsigned int dwType;
        void         *pReserved;

        union {
            /* Fast System Scan Measurement Result */
            FastSysScanResult            FastSystemScanResult;

            /* Digital Installation Chect Result */
            DigInstCheckResult           DigitalInstChkResult;

            /* Analog Installation Chect Result */
        } ;

    } iOS_AutoTestResult;



    typedef enum enumCmdType {
        /*!!!!!!!!!!!!!!!!!!!!! Measurements starting commands !!!!!!!!!!!!!!!!!!!!!!!*/

        /* Command Byte 00000000: Start SLM Analog Channel Measurements */
        eAnaChanMeasCmd = eCmdPubMax + 1,

        /* Command Byte 00000001: Start SLM Digital Channel Measurements */
        eDigChanMeasCmd,

        /* Command Byte 00000010: Start Demod Digital Channel Measurements */
        eDemodDigChanMeasCmd,

        /* Command Byte 00000011: Set Spectrum Analyzer Attenuation */
        eSpecAnalyAttCmd,

        /* Command Byte 00000100: Start Reverse Path Spectrum Analyzer Measurement */
        eRevPathSpecAnalyMeasCmd,

        /* Command Byte 00000101: Start Forward Path Spectrum Analyzer Measurement */
        eForPathSpecAnalyMeasCmd,

        /* Command Byte 00001110: Fast System Scan Measurements */
        eFastSystemScanMeasCmd,

        /* Command Byte 00011001: Digital Installation Check */
        eDigInstCheckCmd,

        /* Command Byte 00011010: Analog Installation Check */
        eAnalogInstCheckCmd,

        /* Command Byte 00000110: Start Temperature Sensor */
        eTemperSensorCmd,

        /* No Command Byte : Read Firmware Revision */
        eReadFirmRevCmd,

        /* Corresponding structure is stRPbSetup */
        eRPBalanceCmd,

        /* Start Forward Path Sweep */
        eForwardSweepCmd,

        /* Command Byte 00010001: Set Low Power Mode */
        eSetLowPowerCmd,


        /*!!!!!!!!!!!!!!!!!!!!! Measurements stopping commands !!!!!!!!!!!!!!!!!!!!!!!*/

        /* Stop SLM Analog Channel Measurements */
        eStopAnaChanMeasCmd,

        /* Stop SLM Digital Channel Measurements */
        eStopDigChanMeasCmd,

        /* Stop Demod Digital Channel Measurements */
        eStopDemodDigChanMeasCmd,

        /* eStopSpecAnalyAttCmd, */

        /* Stop Reverse Path Spectrum Analyzer Measurement */
        eStopRevPathSpecAnalyMeasCmd,

        /* Stop Forward Path Spectrum Analyzer Measurement */
        eStopForPathSpecAnalyMeasCmd,

        /* Stop Fast System Scan Measurement */
        eStopFastSystemScanMeasCmd,

        /* Stop Digital Installation Check */
        eStopDigInstCheckCmd,

        /* Stop Analog Installation Check */
        eStopAnaInstCheckCmd,

        /* Stop Reading Temperature Sensor */
        eStopTemperSensorCmd,

        /* Stop Reading Firmware Revision */
        eStopReadFirmRevCmd,

        /* Stop RP Balance measurement */
        eStopRPBalanceCmd,

        /* Stop Forward Path Sweep command */
        eStopForwardSweep,

        /* Stop all measurements */
        eStopAllMeasCmd,


        /*!!!!!!!!!!!!!!! Other SLM commands for simple operations !!!!!!!!!!!!!!!*/

        /* Upgrade the SLM firmware */
        eUpgradeSLMFirmwareCmd,

        /* Power off and power on the SLM board */
        eRebootSLMBoardCmd,

        /* Clear the demod digital results */
        eCmdDigitalClear,

        /* Invalid cmd type definition */
        eCmdTypeInvalid,



        /*!!!!!!!!!!!!!!!!!!! CX150 Cabel Modem relative commands !!!!!!!!!!!!!!!!!!!!!*/

        /* V150 relative commands starting index */
        eV150MeasCmdStart = eCmdPubMax + 1000,

        /* New V150 commands must be added here */
        /* eXXXXXX = eV150MeasCmdStart + 1 */
        eV150Init,				/* Setup cable modem and return eV150RetInit */
        eV150SerialInit,
        eV150Connect,			/* Try to online with stConnectConfig and return eV150RetConnect */
        eV150Disconnect,
        eV150Unplug,
        eV150Reset,
        eV150RenewClient,		/* Return eV150RetRenewClient */
        eV150PassThroughEn,     /* Enable Pass Through mode */
        eV150PassThroughDis,    /* Disable Pass Through mode */
        eV150Close,
        eV150TurnSQ,			//return eV150RetSQ;
        eV150TurnUS,			//return eV150RetUS
        eV150TurnDS,			//return eV150RetDS
        eV150TestQAMSetup,      //send st_CMTestQAMSetup
        eV150GenUpstream,       //upstream singal generator
        eV150StopUpstream,      //stop upstream singal generator
        eV150TdrStart,		    //start TDR test
        eV150TdrStop,		    //stop TDR test
        eV150Test,
        eV150TurnSQstart,
        eV150Linkstatus,
        eV150Linkstatus4HIP,
        eV150TurnSQstop,
        eV150UpgradeCM,

        eTdrInit,
        eTdrSerialInit,

        eUSG_FEC_Min,
        eUSG_FEC_Init,
        eUSG_FEC_Start,
        eUSG_FEC_TEST_CW2,
        eUSG_FEC_Stop,
        eUSG_FEC_Close,
        eUSG_FEC_CW2,
        eUSG_FEC_PowerOn,      // USG-FEC and SLM Power on at the same time.
        eUSG_FEC_PowerOff,      // USG-FEC Power off, SLM' Power still on
        eCmdUSG_FEC_Sweep_Start,
        eCmdUSG_FEC_Sweep_Stop,
        eUSG_FEC_Max,

        eV150StopConnect,
        eV150ReConnect,


        eCM_RPB_Init,
        eCM_RPB_Close,

        eLeakage_Min,
        eLeakage_Init,
        eLeakage_Start,
        eLeakage_Stop,
        eLeakage_Close,
        eLeakage_Max,

        /* Invalid V150 cmd type definition */
        eV150CmdTypeInvalid,

        /* Following command types are used by OFDM measurements */
        eOFDMCmdBegin = eCmdPubMax + 1500,

        eOFDMCmdInit,          /* No command structure attached */
        eOFDMCmdUpgrade,       /* No command structure attached */
        eOFDMCmdStart,         /* Attach st_OFDMStartCmd as the command structure */
        eOFDMCmdStop,          /* No command structure attached */
        eOFDMCmdLayer,         /* Attach st_OFDMLayerCmd as the command structure */
        eOFDMCmdClose,         /* No command structure attached */

        eOFDMCmdInvalid,

        eCmdIGM_BEGIN = eOFDMCmdInvalid + 100,

        eCmdIGM_Connect, 			//Attach the st_IGMConnect structure, return IGM_Status
        eCmdIGM_Disconnect,			 /* No command structure attached */

        eCmdIGM_DemandQAMStart,        /* Attach the st_DemodQAMConfig structure */
        eCmdIGM_DemandQAMStop,        /* Attach st_DemandQAMStop structure */

        eCmdIGM_DemandLevelStart,      /* Attach the st_LevelQAMConfig structure */
        eCmdIGM_DemandLevelStop,      /* Attach an "int" to indicate the port number(0-9) */

        eCmdIGM_DemandSetAtten,           /* Attach the st_SpectrumAttConfig structure */

        eCmdIGM_DemandRevPathStart,    /* Attach the st_DemandRevPathConfig structure */
        eCmdIGM_DemandRevPathStop,    /*  Attach the st_DemandRevPathStop structure */

        eDemandReverseSweepCmd,    /* Attach the st_ReturnPathSweepConfig */
        eDemandSweepStopCmd,    /* Attach st_ReturnPathSweepStop */

        eCX300ReverseSweepCmd,    /* Attach the st_CX300ReverseSweepConfig */
        eCX300SweepStopCmd,    /* attach no command structure */
        eCX300SetLowPowerCmd, /* No command structure attached */

        eCmdIGM_END,

    } enumCmdType;


    typedef enum enumReadDatatType {
        /* Measurement Status */
        eMeasStatus = eRetPubMax + 1, //1000

        /* SLM Analog Channel Measurements */
        eAnalogChanMeas,

        /* SLM Digital Channel Measurements */
        eDigitalChanMeas,

        /* Demod Digital Channel Measurements */
        eDemodDigitalChanMeas,

        /* Reverse Path Spectrum Analyzer Data */
        eRevPathSpecAnalyData,

        /* Forward Path Spectrum Analyzer Data */
        eForPathSpecAnalyData,

        /* Fast System Scan Result Type */
        eFastSysScanResult,

        /* Digital Installation Check Result Type */
        eDigInstCheckResult,

        /* Analog Installation Check Result Type */
        eAnaInstCheckResult,

        /* Temperature Sensor */
        eTemperSensor,

        /* Firmware Revision */
        eFirmRevision,

        /* Split Data Type */
        eSplitDataBlock,

        /* Upgrading firmware status */
        eUpgradeFWStatus,

        /* CX SLM board firmware revision number */
        eSLMBoardFWVerNum,

        /* RP Balancing result. Return stRPBalancingResult structure */
        eRPBalanceMeas,
        eRPBalanceFinished,

        /* Forward Path Sweep result type */
        eForwardSweepResult,

        /* Invalid data type definition */
        eReadTypeInvalid,


        /* V150 relative results starting index */
        eV150MeasRetStart = eRetPubMax + 1000,

        /* New V150 results index must be added here */
        /* eXXXXXX = eV150MeasRetStart + 1 */
        eV150RetInit,     //2000          // Return stRetInit struct.
        eTdrRetInit,
        eTdrRetStart,
        eUsgRetInit,
        eV150RetConnect,		    // Return stRetEvent one or more times.
        eV150RetClose,
        eV150RetTurnUp,				// ReturnstRetTurnUp struct.
        eV150RetRenewClient,	    // Return stRetRenewClient struct.
        eV150RetSQ,				    //return stRetSQ;
        eCMRetLinkDS = eV150RetSQ, //
        eV150RetUS,				    //return stRetUS
        eV150RetDS,					//return stRetDS
        eCMRetLinkUS,               //return st_ArrUpstreamInfo
        eV150RetCMStatus,			//return stCMStatus
        eV150RetTestQAM,            //return st_CMTestQAMResult
        eV150RetOption,				//return option
        eV150RetUpgradeCM,
        eV150RetStopConn,
        eV150RetResetOver,
        eV150RetBPIDocsis,

        eUsgRetPowerOn,
        eRET_USG_FEC_Close,

        eLeakage_Init_Ret,
        eLeakage_Measure_Ret,
        eLeakage_Start_Ret,
        eLeakage_Stop_Ret,
        eLeakage_Close_Ret,

        /* Invalid V150 result type definition */
        eV150RetTypeInvalid,

        /* Following result types are used by OFDM measurements */
        eOFDMRetBegin = eRetPubMax + 1500,

        eOFDMRetInit,                /* FWUpgradeStatus is attached */
        eOFDMRetResult,              /* st_OFDMResult is attached as the result structure */
        eOFDMRetClose,               /* No result structure is attached */

        eOFDMRetInvalid,

        eRetIGM_Begin = eOFDMRetInvalid + 50,

        eRetIGM_Connect,			//Attach the st_IGMConnectStatus
        eRetIGM_DemandQAM,        /* Attach the st_DemandQAMResultHeader structure followed by a constellation buffer */
        eRetIGM_DemandLevel,      /* Attach the st_DemandQAMLevelResult structure */
        eRetIGM_DemandRevPath,    /* Attach the st_DemandRevPathResultHeader structure followed by a spectrum data buffer */

        eRetIGM_DemandReverseSweep,    /* Attach the st_SweepRetItem structure */
        eRetIGM_DemandReverseSweepNew,    /* Attach the st_SweepRetTable+st_SweepRetItem structure */
        eRetCX300_ReverseSweep,    /* Attach the st_CX300SweepRetItem structure */

        eRetIGM_Invalid,


    } enumReadDatatType;


    typedef struct st_DeviceInfo {
        // Refer to enumDeviceType
        unsigned char cDeviceType;

        char szSerialNum[16];
        // Base board mac address
        unsigned char Mac[6];
        // Cable Modem mac address
        unsigned char CMMac[6];

        // Version number
        char szVersion[128];

        char szTechID[16];
        char TechnicianName[16];
        char szCompany[20];
    } st_DeviceInfo;

    typedef struct st_DeviceInfo_300 {
        // Refer to enumDeviceType
        unsigned char cDeviceType;

        char szSerialNum[32];
        // Base board mac address
        unsigned char Mac[6];
        // Cable Modem mac address
        unsigned char CMMac[6];

        // Version number
        char szVersion[128];

        char szTechID[16];
        //int  TechnicianID;
        char TechnicianName[16];
        char szCompany[20];

    } st_DeviceInfo_300;


    typedef struct {
        char szServerAddr[16];

        // Device information
        st_DeviceInfo   DeviceInfo;

    } CMD_SyncJobsStatus;

    typedef struct {
        int Status;//0-In training, 1-En route, 2-Break, 3-Lunch, 4-warehouse, 5-customer
        int GlobleStatus;//0-On, 1-Off
    } st_SyncWorkForceTech;

    typedef struct {
        char szServerAddr[16];

        // Device information
        st_DeviceInfo   DeviceInfo;

        st_SyncWorkForceTech tech;
        st_SyncCurrentJob job;
    } SyncWorkForceTech;

    typedef struct {
        char szServerAddr[16];

        // Device information
        st_DeviceInfo   DeviceInfo;

    } SyncWorkForceStatus;

    //typedef struct iOS_Workforce
    //{
    /* Please refer to enumRET in commif.h and enumReadDatatType */
    //	unsigned int dwType;
    //	void         *pReserved;

    //	union
    //	{
    //		SyncWorkForceStatus       SyncWorkForceStatus;
    //	} ;

    //} iOS_Workforce;

    typedef struct st_RserverDeviceInfo {
        // Refer to enumDeviceType
        unsigned char cDeviceType;

        char szSerialNum[16];
        // Base board mac address
        unsigned char Mac[6];
        // Cable Modem mac address
        unsigned char CMMac[6];

        // Version number
        char szVersion[128];

        char szTechID[16];
        //int  TechnicianID;
        char TechnicianName[16];
        char szCompany[20];

    } st_RserverDeviceInfo;

    typedef struct st_RserverDeviceInfoExt {
        // Refer to enumDeviceType
        unsigned char cDeviceType;

        char szSerialNum[16];
        // Base board mac address
        unsigned char Mac[6];
        // Cable Modem mac address
        unsigned char CMMac[6];

        // Version number
        char szVersion[128];

        char szTechID[16];
        //int  TechnicianID;
        char TechnicianName[16];
        char szCompany[20];
        char szSupervisorID[32];

    } st_RserverDeviceInfoExt;

    typedef struct st_RserverDeviceInfoExtLongId {
        // Refer to enumDeviceType
        unsigned char cDeviceType;

        char szSerialNum[16];
        // Base board mac address
        unsigned char Mac[6];
        // Cable Modem mac address
        unsigned char CMMac[6];

        // Version number
        char szVersion[256];

        char szTechID[64];
        //int  TechnicianID;
        char TechnicianName[64];
        char szCompany[64];
        char szSupervisorID[32];

    } st_RserverDeviceInfoExtLongId;

    typedef struct st_RserverCoreProfileInfo {
        char szName[32];
        int  nType;       //enum_ProfileType;
        int  nSize;
        int  nArrDate[6]; //year, month, day, hour, minute, second
    } st_RserverCoreProfileInfo;

    typedef struct st_RserverUpdateProfInfo {
        /* 0: Do not need to be updated, 1: Need to be updated */
        char                		bNeedToUpdate;
        st_RserverCoreProfileInfo   ProfFileInfo;

    } st_RserverUpdateProfInfo;

    typedef struct st_RserverProfileInfoArray {
        int                 		nTotalCount;
        st_RserverUpdateProfInfo   *pInfoArray;

    } st_RserverProfileInfoArray;

    typedef struct st_TestsetStatus {
        char serialNum[16];
        int testsetStatus;
    } st_TestsetStatus;

    typedef enum enum_ProfileType {
        // MX 300
        eProfBert_1GC1 = 300,
        eProfBert_1GC2,
        eProfBert_1GF1,
        eProfBert_1GF2,
        eProfBert_10G,
        eProfRfc2544_1GC1,
        eProfRfc2544_1GC2,
        eProfRfc2544_1GF1,
        eProfRfc2544_1GF2,
        eProfRfc2544_10G,
        eProfThrpt_1GC1,
        eProfThrpt_1GC2,
        eProfThrpt_1GF1,
        eProfThrpt_1GF2,
        eProfThrpt_10G,
        eProfScan_1GC1,
        eProfScan_1GC2,
        eProfScan_1GF1,
        eProfScan_1GF2,
        eProfScan_10G,
        eProfVsam_1GC1,
        eProfVsam_1GC2,
        eProfVsam_1GF1,
        eProfVsam_1GF2,
        eProfVsam_10G,

        eProfBert_10G2,
        eProfRfc2544_10G2,
        eProfThrpt_10G2,
        eProfScan_10G2,
        eProfVsam_10G2,

        eProfVperf_1GC1 = 330,
        eProfVperf_1GC2,
        eProfVperf_1GF1,
        eProfVperf_1GF2,
        eProfVperf_10G,
        eProfVperf_10G2,

        //FC
        eProfFcBert_1GF1 = 400,
        eProfFcBert_1GF2,
        eProfFcBert_10G,
        eProfFcBert_10G2,
        eProfFcRfc2544_1GF1,
        eProfFcRfc2544_1GF2,
        eProfFcRfc2544_10G,
        eProfFcRfc2544_10G2,
        eProfFcThrpt_1GF1,
        eProfFcThrpt_1GF2,
        eProfFcThrpt_10G,
        eProfFcThrpt_10G2,

        eProfEthernetIP_1GC1 = 450,
        eProfPPPoE_1GC1,
        eProfEthernetPort_1GC1,
        eProfPing_1GC1,
        eProfTraceRoute_1GC1,
        eProfARPWiz_1GC1,
        eProfFTPDownload_1GC1,
        eProfFTPUpload_1GC1,
        eProfWebTest_1GC1,
        eProfWebBrowser_1GC1,
        eProfIPPhone_1GC1,
        eProfVoIPClient_1GC1,
        eProfVoIPCheck_1GC1,
        eProfEthernetIP_1GC2,
        eProfPPPoE_1GC2,
        eProfEthernetPort_1GC2,
        eProfPing_1GC2,
        eProfTraceRoute_1GC2,
        eProfARPWiz_1GC2,
        eProfFTPDownload_1GC2,
        eProfFTPUpload_1GC2,
        eProfWebTest_1GC2,
        eProfWebBrowser_1GC2,
        eProfIPPhone_1GC2,
        eProfVoIPClient_1GC2,
        eProfVoIPCheck_1GC2,
        eProfEthernetIP_1GF1,
        eProfPPPoE_1GF1,
        eProfEthernetPort_1GF1,
        eProfPing_1GF1,
        eProfTraceRoute_1GF1,
        eProfARPWiz_1GF1,
        eProfFTPDownload_1GF1,
        eProfFTPUpload_1GF1,
        eProfWebTest_1GF1,
        eProfWebBrowser_1GF1,
        eProfIPPhone_1GF1,
        eProfVoIPClient_1GF1,
        eProfVoIPCheck_1GF1,
        eProfEthernetIP_1GF2,
        eProfPPPoE_1GF2,
        eProfEthernetPort_1GF2,
        eProfPing_1GF2,
        eProfTraceRoute_1GF2,
        eProfARPWiz_1GF2,
        eProfFTPDownload_1GF2,
        eProfFTPUpload_1GF2,
        eProfWebTest_1GF2,
        eProfWebBrowser_1GF2,
        eProfIPPhone_1GF2,
        eProfVoIPClient_1GF2,
        eProfVoIPCheck_1GF2,
        eProfEthernetIP_10G,
        eProfPPPoE_10G,
        eProfEthernetPort_10G,
        eProfPing_10G,
        eProfTraceRoute_10G,
        eProfARPWiz_10G,
        eProfFTPDownload_10G,
        eProfFTPUpload_10G,
        eProfWebTest_10G,
        eProfWebBrowser_10G,
        eProfIPPhone_10G,
        eProfVoIPClient_10G,
        eProfVoIPCheck_10G,
        eProfEthernetIP_10G2,
        eProfPPPoE_10G2,
        eProfEthernetPort_10G2,
        eProfPing_10G2,
        eProfTraceRoute_10G2,
        eProfARPWiz_10G2,
        eProfFTPDownload_10G2,
        eProfFTPUpload_10G2,
        eProfWebTest_10G2,
        eProfWebBrowser_10G2,
        eProfIPPhone_10G2,
        eProfVoIPClient_10G2,
        eProfVoIPCheck_10G2,

        // TX 600

        // Common 1000
        eProfLocator = 1000,
        eProfType,
        eProfIPTVChannelTable,
        eProfEthernetIP,
        eProfPPPoE,
        eProfEthernetPort,
        eProfPing,
        eProfTraceRoute,
        eProfARPWiz,
        eProfFTPDownload,
        eProfFTPUpload,
        eProfWebTest,
        eProfWebBrowser,
        eProfIPPhone,
        eProfVoIPClient,
        eProfVoIPCheck,

    } enum_ProfileType;

    typedef struct st_OrgChartStatus {
        char status;	//0: disable, other: enable
    } st_OrgChartStatus;

    typedef struct st_OrgChartInfo {
        char szRegion[128];
        char szDistrict[128];
        char szSystem[128];

    } st_OrgChartInfo;

    typedef struct st_SyncServerInfo {
        char szServerName[50];
        char szServerLocation[50];

        // year, month, day, hour, minute, second
        int nArrDate[6];

    } st_SyncServerInfo;

#endif  /* #ifndef V100_BEUI_COMMUNICATION_INTERFACE_JHT3425TRER423FSD_EYTT54TRHG_H_ */


