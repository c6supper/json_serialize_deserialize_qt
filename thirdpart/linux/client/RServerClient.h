#ifndef Myclient_class
#define Myclient_class

#include "clientbase.h"
#include <iostream>
#include <time.h>
#include <stdio.h>
#include <bitset>
#include <string.h>
#include <stdlib.h>
#include <map>
#include "commif.h"
#include <semaphore.h>
#include <sys/stat.h>
#include <utime.h>
using namespace std;


namespace rserver
{

#define SERVER_SYNC_MAX_BLOCKSIZE      (60 * 1024)
#define SW_BLK_LENGTH   (30720)
#define OPTION_NUM   (10)
#define OPTIONID_LENGTH (16)

    typedef enum enumTestsetType {
        CX100Type = 0,
        CX120Type = 1,
        CX150Type = 2,
        CX180Type = 3,
        CX110Type = 4,
        CX350Type =	5,
        CX380Type = 6,
        CX150D3Type = 7,
        CX180D3Type = 8,
        TX300SType = 11, //for veexpress
        RXT1200Type = 12,

        MX100Type = 100,
        MX120Type = 101,
        MX300Type = 102,
        RXT1200_Rserver_Type = 160,

        BX100AType = 200,
        BX100BType = 201,
        BX100TIType = 202,
        TX300S_Rserver_Type = 205, //for R-Server
        BX100VDSLType = 203,

        TX130Type = 300,
        TX150Type = 301,
        TX300Type = 302,
        TX300SIMG = 301,
        RXT1200SIMG = 303         //rxt1200
    } enumTestsetType;

    typedef enum enumRServerTestsetType {
        RServer_PLATFORM_Type = 160,
        RServer_COMBO_Type = 161,
        RServer_OSA_Type = 162,
        RServer_OTDR_Type = 163,
        RServer_SLM_Type = 164,
        RServer_CFP2_Type = 165,
        RServer_3900_Type = 166,
        RServer_6000E_Type = 167,
        RServer_6000PLUS_Type = 168,
        //	RServer_MTT5_Type=169,
        RServer_MTT6B_Type = 170,
        RServer_MTT8_Type = 171,
        RServer_MTT9_Type = 172,
        RServer_MTT14B_Type = 173,
        //	RServer_MTT14C_Type=174,
        RServer_MTT16A_Type = 175,
        RServer_MTT19A_Type = 176,
        RServer_MTT24_Type = 177,
        RServer_MTT27_Type = 178,
        RServer_MTT38_Type = 179,
        RServer_MTT45_Type = 180,
        RServer_MTT48_Type = 181,
        RServer_MTT50_Type = 182,
        RServer_MTT51_Type = 183,
        RServer_CARRIER_Type = 184,
        RServer_SHDSL_Type = 185,
        RServer_6200_Type = 186,
        RServer_3400_Type = 187,

        RServer_Platform_Image_Type = 340,
        RServer_Combo_Image_Type = 341,
        RServer_Osa_Image_Type = 342,
        RServer_Otdr_Image_Type = 343,
        RServer_Slm_Image_Type = 344,
        RServer_Cfp2_Image_Type = 345,
        RServer_3900_Image_Type = 346,
        RServer_6000e_Image_Type = 347,
        RServer_6000plus_Image_Type = 348,
        //	RServer_MTT5_Image_Type=349,
        RServer_MTT6B_Image_Type = 350,
        RServer_MTT8_Image_Type = 351,
        RServer_MTT9_Image_Type = 352,
        RServer_MTT14B_Image_Type = 353,
        //	RServer_MTT14C_Image_Type=354,
        RServer_MTT16A_Image_Type = 355,
        RServer_MTT19A_Image_Type = 356,
        RServer_MTT24_Image_Type = 357,
        RServer_MTT27_Image_Type = 358,
        RServer_MTT38_Image_Type = 359,
        RServer_MTT45_Image_Type = 360,
        RServer_MTT48_Image_Type = 361,
        RServer_MTT50_Image_Type = 362,
        RServer_MTT51_Image_Type = 363,
        RServer_CARRIER_Image_Type = 364,
        RServer_SHDSL_Image_Type = 365,
        RServer_6200_Image_Type = 366,
        RServer_3400_Image_Type = 367,
    } enumRServerTestsetType;

    typedef enum enumUpgradeMsgType {
        CLIENT_ID = 0x3001,

        MSG_STARTDOWNLOAD = 0x100,
        MSG_GETDATA = 0x101,
        MSG_DATA = 0x200,
    } enumUpgradeMsgType;

    typedef enum enumSWpakeageStatus {
        FinetoUpgrade,

        VersionCantFind,

        VersionIsnewest,

        TooMuchConnection,
    } enumSWpakeageStatus;


    typedef enum enumTestsetUpgradeType {
        CX1xx,
        CX1xxP,
        CX1xxD3,
        CX1xxD3P,

        CX350 = 100,
        CX380,
    } enumTestsetUpgradeType;

    typedef enum enumCheckResult {
        NOT_REGISTER = 0,
        REGISTER_AUTHORIZE = 1,
        REGISTER_NOT_AUTHORIZE = 2,
        REGISTER_RMA = 3,
        CR_UNKNOW,
    } enumCheckResult;


    typedef enum enumClientMsgType {
        eCheckSecurePort = 0x03,

        eTestsetSWverionInfo = 0x11,

        eTestsetSWinfo = 0x12,

        eTestsetSWBLK = 0x13,

        eTestsetSWEND = 0x14,

        eTestsetSWStopSend = 0x15,

        eTestsetSWVersionAndURL = 0x103F0,

        // Echo message type is eVerifyPass == 0x25
        eDeviceInfo = 0x20,

        eFolderInfo = 0x21,

        // Block size <= 60K bytes
        eFileBlock  = 0x22,
        // Indicate the file whose size <= 60K bytes or the
        // last block of the file whose size > 60K bytes
        eFileFinish = 0x23,

        eForce2Disc = 0x24,

        // Result data is 1 byte:
        // 0: the target is not registered
        // 1: the target is registered and authorize succeeded
        // 2: the target is registered but authorize failed
        eVerifyResult = 0x25,

        eAdminSetting = 0x29,

        // Echo message type is eRegistResult == 0x2D
        eRegisterTarget = 0x2A,
        // Echo messaage type is eUnRegistResult == 0x2E
        eUnRegistTarget = 0x2B,

        // Echo message type is eVerifyResult == 0x25
        eCheckTarget = 0x2C,

        // Result data is 1 byte: 0 - succeeded, others - error
        eRegistResult = 0x2D,

        // Result data is 1 byte: 0 - succeeded, others - error
        eUnRegistResult = 0x2E,

        // Command: Get profile info, no corresponding command structure
        eGetProfileInfo = 0x31,
        // Result: Get profile info unfinished, structure is st_ProfileInfo
        eProfileInfo    = 0x33,
        // Result: Get profile info finished, structure is st_ProfileInfo
        eProfileInfoEnd = 0x34,
        // Result: Upload profile info finished, the data is an integer to send the number of profile info uploaded to the server
        eUploadProfileInfoEnd = 0x2038,
        eUploadProfileInfoAck = 0x2039,

        // Command: Get profile file, structure is st_GetProfileFile
        eGetProfileFile = 0x32,
        // Result: Get profile file block, structure is st_ProfileFile
        eProfileFileBlk = 0x35,
        // Result: Get profile file end, structure is st_ProfileFile
        eProfileFileEnd = 0x36,

        // Command: Download all profile from server, no corresponding command structure
        eGetAllProfile = 0x37,

        // Command: Get current device information from server, structure is st_DeviceInfo
        eGetDeviceInfo = 0x38,

        // Command and result: Server info message
        eGetServerInfo = 0x39,

        // Command
        eGetSoftwareVer = 0x3E,

        // Result: Software version information
        eSoftwareVersion = 0x3D,

        eGetSWfile = 0x3F,

        eSWfile = 0x41,

        eSWfileEnd = 0x42,

        // MSG_GET_WF_STATUS = 0x75;
        eGetWorkForceStatus = 0x75,

        //const int MSG_UPDATE_WF_TECH = 0x76;
        eUpdateWorkForceTech = 0x76,

        //MSG_GPS_CONFIG = 0x79
        eGpsConfig = 0x79,

        //�������ʱ��������MSG_GPS_DATA = 0x7A��server������ṹΪSGPSLoc
        eUpdateGPSData = 0x7A,

        //public const int MSG_GPS_ACTIVE = 0x7C; ��GPS��ʼ����ʱ������MSG_GPS_ACTIVE��Server
        eGPSActive = 0x7C,

        //public const int MSG_GPS_INACTIVE = 0x7D; ��GPSֹͣ����ʱ������MSG_GPS_INACTIVE��Server
        eGPSInactive = 0x7D,

        eGetJobsStatus = 0x80,

        eGetJobsCount = 0x81,


        eGetJobsBegin = 0x82,
        eGetJobsEnd = 0x83,


        eGetJobsManageStatus = 0x84,

        eSetCurrentJob = 0x85,

        eReptClosedJob = 0x86,

        eGetJobsDetailsStart = 0x82 + 100, //( Obsolete )
        eGetJobsDetails = 0x83 + 100,    //( Obsolete ) Job Details End

        eGetAdminSettingExt = 0x100,	//lock reg

        /* change the Folder name length and File name length to 256 */
        eFolderInfoEx = 0x1021,
        eFileBlockEx  = 0x1022,
        eFileFinishEx = 0x1023,
        eUploadResultAck = 0x1024,
        UpdateVersionInfo = 0x1038,
        eUploadProfileBlock  = 0x1039,
        eUploadProfileFinish = 0x103A,
        eUploadProfileBlockLongName = 0x103B,
        eUploadProfileFinishLongName = 0x103C,
        eUploadProfileAck = 0x103D,
        eGetSoftwareByType = 0x103F,
        eCheckAll = 0x1002C,
        eCheckAllExt = 0x1002D,
        eCheckAllExt2 = 0x1002E,
        eCheckAllExt3 = 0x1003E,


        eRegisterTargetExt = 0x1002F,	// Org Chart
        eUpdateDeviceInfo = 0x10030,
        eGetOrgChartInfo = 0x10039,
        eGetDevInfoExt = 0x1003A,
        eGetOrgChartStatus = 0x1003B,

        // for support long tech ID
        eRegisterTargetExt2 = 0x1012F,
        eUpdateDeviceInfo2 = 0x10130,
        eCheckAll2 = 0x1012C,
        eUnRegistTarget2 = 0x12B,
        eDeviceInfo2 = 0x120,

        eServerVersion = 0x12000,

        eTestsetStatus = 0x121,

        eSupportVideo = 0x2000,

        eDeviceBondInfo = 0x3456,

        //external commands
        eServerClosed = 0xEE00,
        eSendCmdFail = 0xEE01,
        eUpgradeFail = 0xEE02,
        eConnectReg = 0xEE03,
        eConnectUnReg = 0xEE04,
        eConnectChk = 0xEE05,
        eConnectPkg = 0xEE06,
        eConnectPkgUp = 0xEE07,

        //for R-Server
        eConnect2Server = 0xEE10,
        eDownloadFail   = 0xEE11,
        eConnectFail   = 0xEE12,

        //update VConnect information to VConnect server.
        eUpdateVConnectInfo   = 0xEE20,

        //send commands
        eGetSoftwareVerExt = 0xEF01,

        //received commands
        eSoftwareVersionExt = 0xFE01,

        eSendCmdSucceed = 0xFFFE,

        //update tech name
        eUpdateTechName = 0x10131
    } enumClientMsgType;

    /*struct stRserverEvent2UI {
        uint32_t  messageType;
        uint32_t	messageLenth;
        void	*mesage[2048];
    };*/


    typedef enum enumErrorType {
        eDefaultValue = -1,
        eInvalidDataLength = 0x01,
        eUnknowReply = 0x02,
        eNotFineToUpgrade = 0x03,
        eCantGetDLProgress = 0x04,

    } enumErrorType;

    typedef struct st_CoreProfileInfo {
        char szName[32];
        int  nType;//enum_ProfileType;
        int  nSize;
        int  nArrDate[6]; //year, month, day, hour, minute, second
    } st_CoreProfileInfo;


    typedef struct st_ProfileInfo {
        int 				nCount;
        st_CoreProfileInfo	ArrInfo[100];
    } st_ProfileInfo;

    typedef struct st_AdminSetting {
        char bEnable;
        char bLockDownEnable;
        char TimeLockEnable;

        char TimeBombEnable;
        char strMessage[200];
        int  nDaysCount;

    } st_AdminSetting;


    struct stVerNode {
        char module[16];
        char newVer[16];
        char fileName[128];
    };

    typedef struct st_CurrentVersion {
        char CurrentVersion[128];

    } st_CurrentVerison;
#if 0
    typedef struct st_OrgChartStatus {
        char status;	//0: disable, other: enable
    } st_OrgChartStatus;

    typedef struct st_OrgChartInfo {
        char szRegion[128];
        char szDistrict[128];
        char szSystem[128];

    } st_OrgChartInfo;
#endif
    typedef struct st_SupervisorID {

        char supervisorId[32];

    } st_SupervisorID;

    typedef struct st_GetProfileFile {
        char Name[32];
        int Type;
    } st_GetProfileFile;

    /* Upload/Download Profile, if more blocks, block length = 30k, last block length <= 30k, but "nLength" is total length. */
    typedef struct st_ProfileFile {
        char szName[32];
        int nType;
        int nIndex;
        int nLength; /* total length */
    } st_ProfileFile;

    const static int PROFILEFILE_LEN = sizeof(st_ProfileFile);

    typedef struct st_ProfileFileLongName {
        char szName[256];
        int nType;
        int nIndex;
        int nLength; /* total length */
    } st_ProfileFileLongName;

    typedef struct st_TestsetProfileAck {
        char szProfileName[256];
        int nProfileType;
        int nProfileStatus;	//0 means ok, 1 means invalid encrypted file, others means failed
    } st_TestsetProfileAck;


    typedef struct st_SWFile {
        char szName[128];
        //		int nType;
        int nIndex;
        int nLength;
    } st_SWFile;

    typedef struct st_ResultDirectory {
        char szDirName[32];
        int nResultType;
        int nFileCount;
        //		int TestDate[6];
    } st_ResultDirectory;

    typedef struct st_ResultDirectoryEx {
        char szDirName[256];
        int nResultType;
        int nFileCount;
        //		int TestDate[6];
    } st_ResultDirectoryEx;

    typedef struct st_ResultFile {
        char szFolderName[32];
        char szFileName[32];
        int nBlockIndex;
        int nBlockLength; /* block length, <=60k */
    } st_ResultFile;

    typedef struct st_ResultFileEx {
        char szFolderName[256];
        char szFileName[256];
        int nBlockIndex;
        int nBlockLength; /* block length, <=60k */
    } st_ResultFileEx;

    typedef struct st_ResultAck {
        char szResultName[256];
        int nResultType;
        int nResultStatus;
    } st_ResultAck;

    typedef struct st_UpgradeDemondFileInfo {
        int ID;
        char Version[256];
        char Name[256];
        int StartLength;
    } st_UpgradeDemondFileInfo;

    typedef struct st_UpgradeDemondFileData {
        float Progress;
        int Length;
        BYTE Data[30720];
    } st_UpgradeDemondFileData;

    typedef struct st_TestsetSWinfo {
        int Status;
        int Time;
        int Length;
        BYTE MD5[16];
    } st_TestsetSWinfo;

    typedef struct st_TestsetSWVersioninfo {
        int Type;
        char CurrentVersion[128];
        //		char sFormat[8];//FULL or DELTA
    } st_TestsetSWVersioninfo;


    typedef struct st_RServerSWVersionExt {
        char SoftwareVersion[64];
        char DeltaVersion[64];
        char BasicVersion[64];
        int fullPkgSize;
        int deltaPkgSize;
    } st_RServerSWVersionExt;

    typedef struct st_PlatformSnInfo {
        int type;
        char platformSn[16];
        int moduleCount;
    } st_PlatformSnInfo;

    typedef struct st_ModuleSnInfo {
        int type;
        char moduleSn[16];
    } st_ModuleSnInfo;

    typedef struct st_TechNameInfo {
        char SerialNumber[16];
        char TechName[64];
    } st_TechNameInfo;

    typedef struct st_TestsetSWVersionAndURL {
        int Type;
        char Version[128];
        char FullImageURL[1024];
        char DeltaImageURL[1024];
        char UpgradeFromVersion[128];
        char FullImageChecksum[40];
        char DeltaImageChecksum[40];
    } st_TestsetSWVersionAndURL;

    const static int RESULTFILE_LEN = sizeof(st_ResultFile);

#pragma pack(8)

    typedef struct st_TestsetAllCheckInfo {
        char RegisterStatus;//0x2C
        st_SyncServerInfo ServerInfo;//0x39
        st_RserverDeviceInfoExt DeviceInfo;//0x1003A
        st_AdminSetting AdminMode;//0x29
        char LockRegistration;//0x100
        st_CurrentVerison SWVersion;//0x3E
        st_OrgChartStatus OrgChartStatus;//0x1003B
        st_OrgChartInfo OrgChartInfo;//0x10039
    } st_TestsetAllCheckInfo;

    typedef struct st_TestsetAllCheckInfoLongId {
        char RegisterStatus;
        st_SyncServerInfo ServerInfo;//0x39
        st_RserverDeviceInfoExtLongId DeviceInfo;
        st_AdminSetting AdminMode;//0x29
        char LockRegistration;//0x100
        st_CurrentVerison SWVersion;//0x3E
        st_OrgChartStatus OrgChartStatus;//0x1003B
        st_OrgChartInfo OrgChartInfo;//0x10039
    } st_TestsetAllCheckInfoLongId;

    typedef void (*rserverMessageCallback)(void *obj, unsigned int type,
                                           const unsigned char *data, unsigned int length);

#pragma pack(1)

    typedef struct st_AdminExt {
        char registrationTimeBomb;
        char lockTechId;
        char maintenance;
    } st_AdminExt;

    typedef struct st_TestsetSWVersionInfo {
        int Type;
        char CurrentVersion[128];
    } st_TestsetSWVersionInfo;

    typedef struct st_UpdateVersionInfo {
        char serialNum[16];
        char version[256];
    } st_UpdateVersionInfo;

    struct stConnectCmd {
        void *rServerClint;
        unsigned short port;
        unsigned int type;
        char ipaddr[128];
        char device[64];
    };

    struct stSendCmd {
        void *rServerClint;
        unsigned int type;
        unsigned int length;
        unsigned char data[1];
    };

#pragma pack()

    class CRServerClient: public ClientBase
    {
        public:
            CRServerClient();
            virtual ~CRServerClient();

            void registerMessageCallback(void *obj, rserverMessageCallback callback)
            {
                objSender = obj;
                messageCallback = callback;
            }

            int getCheckResult()
            {
                int m = m_nCheckRes;
                ResetCheckResult();
                return m;
            }

            void ResetCheckResult()
            {
                m_nCheckRes = -1;
            }

            bool &CheckResReady()
            {
                return m_bCheckResReady;
            }

            int getResultLength()
            {
                int m = m_Length;
                ResetResultLength();
                return m;
            }

            void ResetResultLength()
            {
                m_Length = -1;
            }

            int getRegisterResult()
            {
                int m = m_nRegistRes;
                ResetRegisterResult();
                return m;
            }

            void ResetRegisterResult()
            {
                m_nRegistRes = -1;
            }

            int getUnRegisterResult()
            {
                int m = m_nUnRegistRes;
                ResetUnRegisterResult();
                return m;
            }

            void ResetUnRegisterResult()
            {
                m_nUnRegistRes = -1;
            }

            int getProfileInfoCount()
            {
                int m = m_nProfInfoCount;
                ResetProfileInfoCount();
                return m;
            }

            void ResetProfileInfoCount()
            {
                m_nProfInfoCount = -1;
            }

            int getProfileStCount()
            {
                int m = m_nProfStCount;
                ResetProfileStCount();
                return m;
            }

            void ResetProfileStCount()
            {
                m_nProfStCount = -1;
            }

            int getProfFileCount()
            {
                int m = m_nProfFileCount;
                ResetProfFileCount();
                return m;
            }

            void ResetProfFileCount()
            {
                m_nProfFileCount = -1;
            }

            int getErrorCode()
            {
                int m = m_nErrorCode;
                ResetErrorCode();
                return m;
            }

            void ResetErrorCode()
            {
                m_nErrorCode = -1;
            }

            st_ProfileInfo *getProfileInfoResult()
            {
                return ProfileInfoResult;
            }

            void ResetProfileInfoResult()
            {
                free(ProfileInfoResult);
                ProfileInfoResult = NULL;
            }

            st_SyncServerInfo *getServerInfoResult()
            {
                return pServerinfo;
            }

            void ResetServerInfoResult()
            {
                free(pServerinfo);
                pServerinfo = NULL;
            }

            bool &getDevInfoReady()
            {
                return m_bDevInfoReady;
            }

            st_DeviceInfo_300 *getDeviceInfoResult()
            {
                return pDeviceinfo;
            }

            void ResetDeviceInfoResult()
            {
                free(pDeviceinfo);
                pDeviceinfo = NULL;
            }

            st_AdminSetting *getAdminSettingResult()
            {
                return pAdminSetting;
            }

            void ResetAdminSettingResult()
            {
                free(pAdminSetting);
                pAdminSetting = NULL;
            }

            st_CurrentVersion *getCurrentVersionResult()
            {
                return pCurrentVersion;
            }

            void ResetCurrentVersionResult()
            {
                free(pCurrentVersion);
                pCurrentVersion = NULL;
            }

            BYTE getOrgStatusResult()
            {
                //BYTE m = OrgStatus;
                BYTE OrgStatus = -1;
                return OrgStatus;
            }

            st_OrgChartInfo *getOrgChartInfoResult()
            {
                return pOrgChartInfo;
            }

            void ResetOrgChartInfoResult()
            {
                free(pOrgChartInfo);
                pOrgChartInfo = NULL;
            }

            int getSWDownloadProgress()
            {
                return SoftwareDownloadProgress;
            }

            void setSoftwareFilePath(char *filepath)
            {
                strncpy(m_FilePath, filepath, sizeof(m_FilePath) - 1);
                m_FilePath[sizeof(m_FilePath) - 1] = '\0';
                printf("\n setSoftwareFilePath = %s", m_FilePath);
            }

            void getVerArry(struct stVerNode *&arry, int &nArryLen)
            {
                arry = m_arry;
                nArryLen = m_nArryLen;
                cout << "nArryLen " << nArryLen << endl;
                this->m_nArryLen = -1;
            }

            bool &GetVersionReady()
            {
                return m_bGetVersionReady;
            }

            bool ConnectCmd(const char *IPAddress, unsigned short port, const char *device,
                            unsigned int type);
            bool SendCmd(unsigned int type, const unsigned char *data = 0,
                         unsigned int length = 0);
            virtual void OnReceived(unsigned int type, const unsigned char *data,
                                    unsigned int length);
            virtual void OnServerClose();
            virtual void OnSocketError();
            void ConstructMsg(unsigned int type, const unsigned char *data,
                              unsigned int length);
            void ResetUpgradeParam();

            int LoadSize;

            int SWTotalSize;

            char m_FilePath[256];
            uint32_t m_packageDownloadByte;
            uint32_t m_packageTotalByte;

            sem_t m_hSem;

        private:

            void *objSender;
            rserverMessageCallback messageCallback;

            struct stVerNode m_arry[50];
            int m_nArryLen;
            bool m_bGetVersionReady;


            char *ProfileFilePath;

            FILE *stream;
            FILE *m_packageFileStream;

            int m_nCheckRes;
            bool m_bCheckResReady;

            int m_Length;

            int m_nRegistRes;

            int m_nUnRegistRes;

            int m_nProfInfoCount;

            int m_nProfStCount;

            int m_nProfStCurrentCount;

            //		int m_nProfFileCount;

            int m_nErrorCode;

            st_ProfileInfo *ProfileInfoResult;

            st_SyncServerInfo *pServerinfo;

            st_DeviceInfo_300 *pDeviceInfo_300;
            bool m_bDevInfoReady;

            st_AdminSetting *pAdminSetting;

            st_CurrentVersion *pCurrentVersion;

            st_DeviceInfo_300 *pDeviceinfo;

            BYTE OrgStatus;

            st_OrgChartInfo *pOrgChartInfo;

            int SoftwareDownloadProgress;

            void CatchError(int errorcode);

            void SetMultiErrorCode();

            void SetErrorCode(int errorcode);

            FILE *createPackageFile(const char *filepath);
            bool packageDownloadAndVerify(const void *data, uint32_t length);

            int		m_nProfFileCount;
            int 	m_nReceivedProfFileCount;
            int		m_receiveSize;
            FILE	*m_profFileStream;

            void ResetDownloadParam();
            bool profileDownloadAndVerify(const void *data, uint32_t length);
            void getAbsolutePath(char *pPathBuf, int nType, const char *pFileName);
            void getAbsoluteFolderPath(char *pPathBuf, int nType);
            void createMulvPath(char *muldir);
            time_t convertTime(int *pArrDate);

        public:
            int GetProgress();
    };

}

#endif
