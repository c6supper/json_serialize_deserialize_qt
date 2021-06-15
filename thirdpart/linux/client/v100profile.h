#if !defined(__V100_SYSTEM_PROFILE_H__)
#define __V100_SYSTEM_PROFILE_H__



#include "commif.h"

#define FILE_NAME_PREFIX_PATH "/usr/app/prefix"
#define SYSPROFILEPATH "/usr/app/sysprofile"
#define SYNCPROFILEPATH "/usr/app/syncserverprofile"
#define TOAFILEPATH "/usr/lib/v100/res/toasetup.txt"
#define TOAINFOPATH "/usr/lib/v100/res/toainfo.txt"
#define TOASETTINGPATH "/usr/app/toasetting"
#define IPCONFIGSHELLPATH	"/usr/app/scripts/ipconfig.sh"
#define IP_ADDR_PATH         "/usr/storage/common/profile/address/ipaddr"
#define USR_ADDR_PATH        "/usr/storage/common/profile/address/usraddr"
#define IPV6_ADDR_PATH       "/usr/storage/common/profile/address/ipv6addr"
#define IPV6_USR_ADDR_PATH   "/usr/storage/common/profile/address/ipv6usraddr"
#define SIP_ADDR_PATH        "/usr/storage/common/profile/address/sipaddr"
#define ADDR_DIR_PATH             "/usr/storage/common/profile/address"
#define IPTV_CFG_PATH        "/usr/storage/common/profile/address"
#define IPTV_IPV6_CFG_PATH        "/usr/storage/common/profile/address/ipv6"
#define CDEF_IPTOOLS_FILE_PATH "/usr/storage/common/profile/iptools"
#define SAVE_LOCATOR_FILE_PATH "/usr/storage/common/profile/savelocator.txt"
#define SAVE_TYPE_FILE_PATH 	"/usr/storage/common/profile/savetype.txt"
#define STORAGE_SETUP_FILE "/usr/app/storage_setup.txt"
#define JOBSDETAILSFILEPATH "/usr/app/jobsdetail"

// Ethernet system storage dir define
#define CSTORAGE_PATH		           "/usr/storage/common"
#define CSTORAGE_STP_PATH	        "/usr/storage/common/profile"

#define V100_PROFILE_NAME_DEFAULT        ("Default")

#define V100_IP_MANSETUP_PROFILE_PATH      ("/usr/storage/common/profile/iptools0")
#define V100_IP_WIFISETUP_PROFILE_PATH     ("/usr/storage/common/profile/iptools1")
#define V100_IP_TESTSETUP_PROFILE_PATH     ("/usr/storage/common/profile/iptools2")
#define V100_IP_SECONDSETUP_PROFILE_PATH   ("/usr/storage/common/profile/iptools3")
#define V100_IP_FTPWEB_PROFILE_PATH        ("/usr/storage/common/profile/webftp")
#define V100_IP_VOIP_PROFILE_PATH          ("/usr/storage/common/profile/voip")



#define V100_VLAN_DEFAULT_PRIORITY       (0)
#define V100_VLAN_DEFAULT_VLANID         (0)
#define V100_IP_DHCP_DEFAULT_VENDORCLASS ("")
#define V100_IP_DHCP_DEFAULT_USERCLASS   ("")
#define V100_IP_DHCP_DEFAULT_VENDORINFO  ("")
#define V100_IP_DHCP_DEFAULT_HOSTNAME    ("")
#define V100_IP_STATIC_DEFAULT_IPADDR    ("192.168.0.10")
#define V100_IP_STATIC_DEFAULT_NETMASK   ("255.255.255.0")
#define V100_IP_STATIC_DEFAULT_GATEWAY   ("192.168.0.1")
#define V100_IP_STATIC_DEFAULT_DNSIP     ("192.168.0.1")
#define V100_IP_STATIC_DEFAULT_DOMAIN    ("vetronicsltd.com.cn")

#define V100_IP_FTP_DEFAULT_HOSTNAME    ("www.kernel.org")
#define V100_IP_FTP_DEFAULT_FILEPATH    ("/pub/README")
#define V100_IP_FTP_DEFAULT_USERNAME    ("anonymous")
#define V100_IP_FTP_DEFAULT_PASSWORD    ("anonymous")
#define V100_IP_FTP_DEFAULT_UPLOADSIZE  (500*1024)

#define V100_IP_WEB_DEFAULT_URL         ("http://www.google.com")
#define V100_IP_WEB_DEFAULT_REPEATCOUNT (5)


#if defined(__cplusplus)
extern "C" {
#endif/*defined(__cplusplus)*/

typedef enum {
    FILES_FTP_UPLOAD,
    TOOLS_IP_FTP_UPLOAD
} FTP_UPLOAD_ID;

typedef enum {
    FTP,
    USB,
    BLUETOOTH
} TRANSFER_TYPE;

typedef enum {
    LIST_10SEC_ID = 1,
    LIST_30SEC_ID ,
    LIST_1MIN_ID ,
    LIST_2MIN_ID ,
    LIST_3MIN_ID ,
    LIST_4MIN_ID ,
    LIST_5MIN_ID ,
    LIST_6MIN_ID ,
    LIST_7MIN_ID ,
    LIST_8MIN_ID ,
    LIST_9MIN_ID ,
    LIST_10MIN_ID ,
    LIST_30MIN_ID ,
    LIST_60MIN_ID

} TIME_LIST_ID;

typedef	enum {
    LIST_ENGLISH_ID = 0,
    LIST_FRANCE_ID =  1,
    LIST_ESPANOL_ID = 2,
    LIST_CHINESE_ID = 3
} LIST_LANGUAGE_ID;

typedef enum {
    LIST_SAVE_MODE_AUTO_ID,
    LIST_SAVE_MODE_ADVANCED_ID,
} LIST_SAVE_MODE;

typedef enum {
    LIST_AUTO_SAVE_ON,
    LIST_AUTO_SAVE_OFF,
} LIST_AUTO_SAVE_SWITCH;

typedef enum {
    UNIT_ENGLISH_ID = 0,
    UNIT_METRICH_ID = 1
} LIST_UNIT_ID;

typedef struct {
    int	 	BetteryLev;
    int 	ACLev;
    TIME_LIST_ID	BetteryTime;
    TIME_LIST_ID 	ACTime;
    unsigned int  	CheckBettery;
    unsigned int  	CheckAC;
} BACKLIGHTSETTING;

typedef struct {
    unsigned int  	Theme;
    unsigned int  		xFactor;
    unsigned int 		yFactor;
    unsigned int 		xOffset;
    unsigned int 		yOffset;
} SCREENSETTING;

typedef struct {
    TIME_LIST_ID	PowerTime;
    unsigned int 			CheckPower;


} POWERSETTING;


typedef enum {
    eAudibleAlarm_On,
    eAudibleAlarm_Off

} eAudibleAlarm;

typedef enum {
    eStartupMode_Full,
    eStartupMode_Quick

} eStartupMode;

#define MAX_LEN_FILE_NAME_PREFIX  8
#define MAX_LEN_TECH_ID  10
#define MAX_LEN_TECH_NAME  16
#define MAX_LEN_COMPANY  20
#define MAX_LEN_LOCATOR 30
#define MAX_LEN_TYPE 30

typedef struct {
    LIST_LANGUAGE_ID	Language;
    LIST_UNIT_ID	Uint;
    eAudibleAlarm Alarm;
    char FNPrefix[MAX_LEN_FILE_NAME_PREFIX + 1];
    int bShowPsw;
    int managerPort;
    LIST_SAVE_MODE saveMode;
    LIST_AUTO_SAVE_SWITCH autoSaveSwitch;
    char TechID[MAX_LEN_TECH_ID];
    char TechName[MAX_LEN_TECH_NAME];
    char Company[MAX_LEN_COMPANY];
    char szSupervisorID[32];
    eStartupMode	startup;
    int syncServerPort;
} REGIONALSETTING;

typedef struct {
    unsigned int 	CheckAdjust;
    unsigned int    CheckTime;
    unsigned int  Timezone;
    unsigned char   rev[4];
} CLOCKDATESETTING;


typedef enum {
    eST_IPTYPE_DHCP,
    eST_IPTYPE_STATIC,
} eStIpAddrType;

typedef enum {
    eST_DNS_OFF,
    eST_DNS_MANUAL,
#if 0
#ifndef CX_BUILD
#warning "should remove this item, dhcp have no DNS setting"
#endif
#endif/*0*/
    eST_DNS_AUTO,
} eStDNSType;

typedef enum {
    eST_VLAN_OFF,
    eST_VLAN_ON
} eStVlanStatus;

typedef enum {
    eST_DHCP_OPT_OFF,
    eST_DHCP_OPT_BOTH,
    eST_DHCP_OPT_VENDORCLASS,
    eST_DHCP_OPT_USERCLASS,
    eST_DHCP_OPT_HOSTNAME,
    eST_DHCP_OPT_VENDORINFO
} eStDHCPOPT;

// Added by zyhong for IPv6
typedef enum {
    eST_IPTYPE_IPV4,
    eST_IPTYPE_IPV6,
} eStIpVersion;

typedef struct {
    unsigned short    priority;
    unsigned short    vlan_id;
} stStVlanInfo;

// IP configure.
typedef struct {
    //VLAN
    eStVlanStatus    vlan_opt;
    //IP configure.
    eStIpAddrType    ip_type;					//dynamic or static.
    //DHCP options
    eStDHCPOPT       dhcp_opt;
    //DNS.
    eStDNSType       dns_type;

    stStVlanInfo   vlaninfo;

    char           vendorclass[32];
    char           userclass[32];

    char	       ipaddr[16];
    char	       netmask[16];
    char	       gateway[16];

    char	       dnsip[16];
    char		   domain[64];
    char           leasetime[32];

    int            ip_probe_fail;  /* non-zero to mark ip confliction detected */
    int            gw_probe_fail;  /* non-zero to mark gateway have not detected */
    int            dns_probe_fail; /* non-zero to mark DNS have not detected */

    //IP type
    eStIpVersion     ip_version;               // ipv4 or ipv6

    char           ipv6_ipaddr[64];
    int            ipv6_cidr;
    char           ipv6_gateway[64];
    char           ipv6_dnsip[64];

    char           hostname[50];
    unsigned char  vendorinfo[256];

} stStIPConfig;

typedef enum {
    eST_MAC_DEFAULT,
    eST_MAC_MANUAL
} eStMacType;

typedef struct {
    eAutoNego         auto_neg;
    eLineRate         speed;
    eDuplexMode       dupmode;
    eControl          flowctl;

    eStMacType        mactype;
    char	          default_MAC[32];
    char	          manual_MAC[32];

    eLineRate         speed_setup;    /* only be used for auto_neg == eAN_DISABLE */
    eDuplexMode       dupmode_setup;  /* only be used for auto_neg == eAN_DISABLE */
} stStPortConfig;

typedef enum {
    eIP_STATUS_IPDOWN,
    eIP_STATUS_IPUP,
    eIP_STATUS_IPFAIL,     /*IP conflict or other failure*/
    eIP_STATUS_INPROGRESS
} eIpStatus;

typedef struct {
    int dur_time;
    char VoipLastIpAddr[16];
    char SIPLastURLAddr[128];
} VOIPSETTING;

typedef struct {
    unsigned int table_id;
    unsigned int chan1_id;
    unsigned int chan2_id;
    unsigned int chan3_id;
} IPTVSETTING;

typedef struct {
    char upload_address[32];
    char upload_file_path[128];
    char upload_username[32];
    char upload_pwd[32];
    int  upload_type;
    char upload_usb_file_path[128];
    char upload_bt_mac[64];
    char upload_bt_name[64];
    int  upload_bt_channel;
    int  upload_bt_status;
} FILEUPLOADSETTING;

typedef enum {
    eNetMODE_other,
    eMODE_PPPOE,
    eMODE_ETH,
    eMODE_BT_DIALUP,
    eMODE_DATACARD,
    eMODE_WIFI,
    eMODE_HDLC_PPP,
    eMODE_HDLC_ETH
} eNetMode;

#define  PPP_USER_ID_LEN              60
#define  PPP_PASSWD_LEN               20

//this structure is obsolete from compatibility reasons
typedef struct {
    ePPP_Auth    Auth;
    char             userid[20];
    char             passwd[20];
} stDummyReserved;

typedef struct {
    ePPP_Auth    Auth;
    char             userid[PPP_USER_ID_LEN];
    char             passwd[PPP_PASSWD_LEN];
} stPPPoECfg;

typedef enum {
    ePTYPE_TESTPORT,
    ePTYPE_10_100T_CH,   /*10-100 Mbps ether*/
    ePTYPE_WIFI,
} ePType;

typedef enum {
    eSTATUS_LINKDOWN,
    eSTATUS_LINKUP,
} eLinkStatus;

/*
typedef struct
{
	unsigned int Mobileprovider;
	char Username[60];
	char Password[32];
	char APN[64];
} stDialUpCfg;
*/
#define V100P_VLAN_TAGNUM   (3)


typedef struct {
    char	PingUrlAddr[40];
    char	PingIPAddr[16];
    int PingLength;
    int PingNumber;
    int PingPerSec;
    int PingLastFileType;
    //	int PingLastURLAddrId;
    //	int PingLastIpAddrId;
    char PingLastURLAddr[40];
    char PingLastIPAddr[16];
    int PingTimeout;
} stPingProf;

typedef struct {
    char Arp_Start[16];
    char Arp_End[16];
    char Arp_Subnet[16];
} stArpProf;

typedef struct {
    char	TracerouteAddr[40];
    //	int TraceLastURLAddrId;
    char TraceLastURLAddr[40];
} stTraceProf;


typedef struct {
    char manipProfile[32];   /* management port IP profile name */
    char wifiipProfile[32];  /* WIFI port IP profile name */
    char testipProfile[32];  /* test port IP profile name */
    char secondipProfile[32];/* second test port IP profile name */
    char ftpProfile[32];     /* ftp/web test profile name */
    char hipProfile[20];
} MODULEPROFILE;

//WiFi
typedef enum {
    eWifi_ENCRYP_WEP,
    eWifi_ENCRYP_WPA,
    eWifi_ENCRYP_WPA2,
    eWifi_ENCRYP_DISABLE
} eWifiEncryption;

typedef enum {
    eWifi_TYPE_INFRASTRUCTURE,
    eWifi_TYPE_AD_HOC
} eWifiType;

#define MAX_SSID_CHAR	40
typedef struct {
    char SSiD[MAX_SSID_CHAR];
    unsigned char BSSiD[20];
    unsigned char channel;  //1-11
    eWifiEncryption encryp;
    eWifiType type;
} WifiManualSetting;

typedef struct {
    WifiManualSetting manual;
} WIFISETTING;

typedef struct {
    char ip_address[16];
} FILERSERVERUPLOAD;

typedef enum {
    eLocatorTap,
    eLocatorGroundBlock,
    eLocatorMasterBedRoom,
    eLocatorLivingRoom,
} eLocator;

typedef enum {
    eTypeSTB,
    eTypeDVR,
    eTypeCableModem,
    eTypeDrop,
} eTestType;


typedef struct {
    char ip_address[16];
    char port[20];
    bool bSubmitted;
    int  status;
} REG_SETTING;


enum {
    BT_SERVICE_FILE_UPLOAD,
    BT_SERVICE_DIALUP,
    BT_SERVICE_MAX
};

typedef enum {
    eMobile_SP_ATT,
    eMobile_SP_Verizon,
    eMobile_SP_TMobile,
    eMobile_SP_O2,
    eMobile_SP_TRUE,
    eMobile_SP_Sprint,
    eMobile_SP_UserDefined
} eMobileServProvider;


typedef struct {
    bool isNeedDownChannelTables;
    bool isNeedDownLocations;
    bool isNeedDownSoftware;
    bool isNeedDownHipProfile;
    bool isNetUpdateSoftware;
} StNeedDownd;

// for toa setup
#define LEN_TOA_ADDR        (128)
#define LEN_TOA_COMPANY     (64)
#define LEN_TOA_USERNAME    (64)
#define LEN_TOA_PASSWORD    (64)
typedef struct {
    char address[LEN_TOA_ADDR];
    char company[LEN_TOA_COMPANY];
    char username[LEN_TOA_USERNAME];
    char password[LEN_TOA_PASSWORD];
} stToaSetup;


typedef struct {
    unsigned long SoftwareOption;
    BYTE SoftwareVer[32];
    BYTE HardWareVer[32];
    BYTE	MacAddr[8];
    BYTE **ModuleOptionStr;
    int	ModuleOptionNum;
    BYTE	MouduleBoardMac[6];
    BYTE	MouduleBoardMac2[6];
    unsigned char ServiceNum;
    unsigned long SoftwareOptionAdd1;
} VERSIONCTRL;

const int MAX_LEN_TestResult = 10;

typedef struct {
    unsigned int nFolderIndex;

} StroageStruct;

extern char SoftVersionNumer[];
extern VERSIONCTRL *g_pSysVersion;
extern stToaSetup toasetup;
#if defined(__cplusplus)
}
#endif/*defined(__cplusplus)*/

#endif/*!defined(__V100_SYSTEM_PROFILE_H__)*/
