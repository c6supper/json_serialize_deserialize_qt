#ifndef RserverConnecterHelper_class
#define RserverConnecterHelper_class

#include "RServerClient.h"

namespace rserver
{
    typedef struct st_DeviceInfoExt {
        // Refer to enumDeviceType
        char cDeviceType;

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
    } st_DeviceInfoExt;

    class RserverConnecter
    {
        public:

            RserverConnecter(CRServerClient *pbase = 0);
            ~RserverConnecter();

            bool Disconnect();
            bool disconnect();

            bool sendCmd(unsigned int type, const unsigned char *data, unsigned int length);

            bool ConnectCmd(const char *IPAddress, unsigned short port, const char *device,
                            unsigned int type);

            void registerMessageCallback(void *obj, rserverMessageCallback callback);

            int TestsetUpgrade(char *CurrVer, int Type);

            int GetSoftwareVersion();
            int GetSoftwareVersionExt(char *ver);

            bool FileExists(char *filePath);
            bool SendSingleFile(st_ResultFile *pResultFile, char *m_ResultPath);
            bool SendFiles2SyncServer(const char *pFolderPath, const char *folderName);

            int GetFileCountInFolder(const char *pFolderPath);
            bool UploadResults2Server(char *resultPath, const char *dirName,
                                      const int resultType);
            bool UploadProfiles2Server(char *resultPath, const char *dirName,
                                       const int resultType);
            bool SendProfileSingleFile(st_ProfileFile *pProfileFile, char *path);
            bool SendProfileFileBlock(st_ProfileFile *fp);
            bool SendProfileFileFinish(st_ProfileFile *fp);
            int StopTestsetUpgrade();
            int UpdateModuleVer(const char *sn, const char *version);

            //for R-Server
            int RServerRegister(char DeviceType, char *SNum, char *Mac, char *CMMacm,
                                char *Version, char *TechID, char *TechnicianName, char *Company,
                                char *supervisorid);
            int RServerUnRegister(char DeviceType, char *SNum, char *Mac, char *CMMacm,
                                  char *Version, char *TechID, char *TechnicianName, char *Company);
            int RServerCheck(char DeviceType, char *SNum, char *Mac, char *CMMacm,
                             char *Version, char *TechID, char *TechnicianName, char *Company);
            int RServerGetDeviceInfo(char DeviceType, char *SNum, char *Mac, char *CMMacm,
                                     char *Version, char *TechID, char *TechnicianName, char *Company,
                                     char *supervisorid);
            int RServerGetProfileInfo();
            int RServerGetProfileInfo(st_RserverProfileInfoArray *pInfoArray, int size);
            int RServerDownloadProfile(char *ProfileName, int type);
            int RServerDownloadAllProfile();
            int RServerGetOrgChartStatus();
            int RServerGetOrgChartInfo(char *supervisorId);
            int RServerGetServerInfo();

        private:
            CRServerClient  *m_psender;

            FILE *Upstream;

            char *m_resultPath;

            char *ResultPath;

            char *FilePath;

            size_t PathLength;

            int getlength(FILE *fp);
            bool SendFileBlock(st_ResultFile *fp);
            bool SendFileFinish(st_ResultFile *fp);
    };
}

#endif
