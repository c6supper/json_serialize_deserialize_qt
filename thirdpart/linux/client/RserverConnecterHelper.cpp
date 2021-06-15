#include "RserverConnecterHelper.h"
#include "RServerClient.h"
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <utime.h>

namespace rserver
{
    RserverConnecter::RserverConnecter(CRServerClient *pbase)
    {
        if (pbase == 0) {
            m_psender = new CRServerClient();
        } else {
            m_psender = pbase;
        }
    }

    RserverConnecter::~RserverConnecter()
    {
        if (m_psender != 0) {
            delete m_psender;
            m_psender = 0;
        }
    }

    bool RserverConnecter::ConnectCmd(const char *IPAddress, unsigned short port,
                                      const char *device, unsigned int type)
    {
        return m_psender->ConnectCmd(IPAddress, port, device, type);
    }

    void RserverConnecter::registerMessageCallback(void *obj,
            rserverMessageCallback callback)
    {
        m_psender->registerMessageCallback(obj, callback);
    }

    bool RserverConnecter::disconnect()
    {
        return m_psender->Disconnect();
    }

    bool RserverConnecter::sendCmd(unsigned int type, const unsigned char *data,
                                   unsigned int length)
    {
        return m_psender->SendCmd(type, data, length);
    }

    int RserverConnecter::TestsetUpgrade(char *CurrVer, int Type)
    {
        int nRet = RC_FAIL;
        st_TestsetSWVersioninfo cmd;
        memset(&cmd, 0, sizeof(st_TestsetSWVersioninfo));
        memcpy(cmd.CurrentVersion, CurrVer, sizeof(cmd.CurrentVersion));
        cmd.Type = Type;
        cout << "\nTestsetUpgrade CurrentVersion/Type: " << cmd.CurrentVersion << "," <<
             cmd.Type << endl;
        nRet = m_psender->SendCmd(eTestsetSWverionInfo, (const unsigned char *)&cmd,
                                  sizeof(cmd));
        return nRet;
    }

    int RserverConnecter::StopTestsetUpgrade()
    {
        int nRet = RC_FAIL;
        nRet = m_psender->SendCmd(eTestsetSWStopSend);
        return nRet;
    }

    int RserverConnecter::GetFileCountInFolder(const char *pFolderPath)
    {
        int nFileCount = 0;
        DIR *pDir = opendir(pFolderPath);

        if (NULL == pDir) {
            return 0;
        }

        struct dirent *pEntity = NULL;

        while (NULL != (pEntity = readdir(pDir))) {
            if (8 == pEntity->d_type) {
                nFileCount++;
            }
        }

        if (NULL != pDir) {
            closedir(pDir);
        }

        return nFileCount;
    }

    bool RserverConnecter::FileExists(char *filePath)
    {
        FILE *pstream = fopen(filePath, "rb");

        if (pstream == NULL) {
            printf("\n The file(path:%s) does not exist!", filePath);
            return false;
        } else {
            fclose(pstream);
            pstream = NULL;
            return true;
        }
    }

    bool RserverConnecter::UploadProfiles2Server(char *path, const char *name,
            const int type)
    {
        char xmlFileName[32];
        char xmlFilePath[128];
        bool ret = false;
        strncpy(xmlFileName, name, 32);
        strncpy(xmlFilePath, path, 128);
        strcat(xmlFilePath, xmlFileName);
        st_ProfileFile xmlFile;
        memset(&xmlFile, 0, sizeof(st_ResultFile));
        xmlFile.nType = type;
        memcpy(xmlFile.szName, name, 32);
        ret = SendProfileSingleFile(&xmlFile, xmlFilePath);
        time_t time_now;
        struct utimbuf uid_time;
        time_now = time(NULL);
        uid_time.actime  = time_now + 60;	 /* Access time.  */
        uid_time.modtime = time_now + 60;	 /* Modification time.  */
        utime(xmlFilePath, &uid_time);
        return ret;
    }

    bool RserverConnecter::SendProfileSingleFile(st_ProfileFile *pProfileFile,
            char *path)
    {
        bool ret = false;
        //	printf("\nReady to upload,FilePath: %s.", m_ResultPath);
        Upstream = fopen(path, "rb");

        if (Upstream == NULL) {
            printf("\nerror occurred when reading file...\n");
            return ret;
        }

        int size = getlength(Upstream);
        printf("\nfile path = %s, file size = %d.", path, size);

        while (size > SERVER_SYNC_MAX_BLOCKSIZE) {
            ret = SendProfileFileBlock(pProfileFile);

            if (true != ret)
                return ret;

            size -= SERVER_SYNC_MAX_BLOCKSIZE;
        }

        if (size != 0) {
            pProfileFile->nLength = size;
            ret = SendProfileFileFinish(pProfileFile);
            size = 0;
            printf("upload successed!");
        } else {
            fclose(Upstream);
            Upstream = NULL;
            printf("upload successed!");
        }

        return ret;
    }

    bool RserverConnecter::SendProfileFileBlock(st_ProfileFile *fp)
    {
        bool result = false;
        fp->nLength = SERVER_SYNC_MAX_BLOCKSIZE;
        unsigned char sendbuffer[fp->nLength + PROFILEFILE_LEN];
        memcpy(sendbuffer, fp, PROFILEFILE_LEN);
        int ret = fread(sendbuffer + PROFILEFILE_LEN, fp->nLength, 1, Upstream);

        if (ret != 1) {
            printf("error occurred when writing data to buffer...\n");
            return result;
        }

        result = m_psender->Send(eUploadProfileBlock,
                                 (const unsigned char *)&sendbuffer, sizeof(sendbuffer));
        fp->nIndex++;
        return result;
    }

    bool RserverConnecter::SendProfileFileFinish(st_ProfileFile *fp)
    {
        bool result = false;
        unsigned char sendbuffer[fp->nLength + PROFILEFILE_LEN];
        memcpy(sendbuffer, fp, PROFILEFILE_LEN);
        int ret = fread(sendbuffer + PROFILEFILE_LEN, fp->nLength, 1, Upstream);

        if (ret != 1) {
            printf("error occurred when writing data to buffer...\n");
            return result;
        }

        result = m_psender->Send(eUploadProfileFinish,
                                 (const unsigned char *)&sendbuffer, sizeof(sendbuffer));
        fclose(Upstream);
        Upstream = NULL;
        return result;
    }

    bool RserverConnecter::UploadResults2Server(char *resultPath,
            const char *dirName, const int resultType)
    {
        char xmlFileName[32];
        char xmlFilePath[128];
        char htmlFileName[32];
        char htmlFilePath[128];
        char htmlImageFolderPath[128];
        int xmlFileCount = 0;
        int htmlFileCOunt = 0;
        int htmlFolderFileCnt = 0;
        bool ret = false;
        strncpy(xmlFileName, dirName, 32);
        //	strcat(xmlFileName, ".xml");
        strncpy(xmlFilePath, resultPath, 128);
        strcat(xmlFilePath, xmlFileName);
        strcat(xmlFileName, ".xml");

        if (FileExists(xmlFilePath))
            xmlFileCount = 1;

        //	printf("\n xmlFilePath:%s",xmlFilePath);
        strncpy(htmlFileName, dirName, 32);
        strcat(htmlFileName, ".html");
        strncpy(htmlFilePath, resultPath, 128);
        strcat(htmlFilePath, htmlFileName);

        if (FileExists(htmlFilePath))
            htmlFileCOunt = 1;

        //	printf("\n htmlFilePath:%s",htmlFilePath);
        strncpy(htmlImageFolderPath, resultPath, 128);
        strcat(htmlImageFolderPath, dirName);
        strcat(htmlImageFolderPath, "_files/");
        htmlFolderFileCnt = GetFileCountInFolder(htmlImageFolderPath);
        //	printf("\n htmlImageFolderPath:%s",htmlImageFolderPath);
        st_ResultDirectory _ResultDirectory;
        memset(&_ResultDirectory, 0, sizeof(st_ResultDirectory));
        memcpy(_ResultDirectory.szDirName, dirName, sizeof(_ResultDirectory.szDirName));
        _ResultDirectory.nFileCount = xmlFileCount + htmlFileCOunt + htmlFolderFileCnt;
        _ResultDirectory.nResultType = resultType;
        //	printf("\n DirName:%s,%d,%d.",_ResultDirectory.szDirName, _ResultDirectory.nFileCount, _ResultDirectory.nResultType);

        if (_ResultDirectory.nFileCount <= 0) {
            printf("\n there is no result file!");
            return ret;
        }

        // get file saved time
        char filePath[128];

        if (0 < xmlFileCount)
            memcpy(filePath, xmlFilePath, 128);
        else if (0 < htmlFileCOunt)
            memcpy(filePath, htmlFilePath, 128);

        if ((0 < xmlFileCount) || (0 < htmlFileCOunt)) {
            FILE *fp;
            int fd;
            struct stat buf;
            fp = fopen(filePath, "r");
            fd = fileno(fp);
            fstat(fd, &buf);
            //            time_t mdTime = buf.st_mtime;
            /*            struct tm *pStDate;
                        pStDate = localtime(&mdTime);*/
            //		printf("\n filePath:%s, mdTime:%d, stDate:%d-%d-%d %d:%d:%d", filePath, mdTime,pStDate->tm_year+1900,pStDate->tm_mon+1,pStDate->tm_mday,pStDate->tm_hour,pStDate->tm_min,pStDate->tm_sec);
            fclose(fp);
#if 0
            _ResultDirectory.TestDate[0] = pStDate->tm_year + 1900;
            _ResultDirectory.TestDate[1] = pStDate->tm_mon + 1;
            _ResultDirectory.TestDate[2] = pStDate->tm_mday;
            _ResultDirectory.TestDate[3] = pStDate->tm_hour;
            _ResultDirectory.TestDate[4] = pStDate->tm_min;
            _ResultDirectory.TestDate[5] = pStDate->tm_sec;
#endif
        }

        ret = m_psender->Send(eFolderInfo, (const unsigned char *)&_ResultDirectory,
                              sizeof(st_ResultDirectory));

        if (true != ret)
            return ret;

        printf("\n xmlFileCount:%d, htmlFileCount:%d, htmlFolderFileCnt:%d",
               xmlFileCount, htmlFileCOunt, htmlFolderFileCnt);

        if (0 < xmlFileCount) {
            st_ResultFile xmlResultFile;
            memset(&xmlResultFile, 0, sizeof(st_ResultFile));
            memcpy(xmlResultFile.szFolderName, dirName, 32);
            memcpy(xmlResultFile.szFileName, xmlFileName, 32);
            ret = SendSingleFile(&xmlResultFile, xmlFilePath);

            if (true != ret)
                return ret;
        }

        if (0 < htmlFileCOunt) {
            st_ResultFile htmlResultFile;
            memset(&htmlResultFile, 0, sizeof(st_ResultFile));
            memcpy(htmlResultFile.szFolderName, dirName, 32);
            memcpy(htmlResultFile.szFileName, htmlFileName, 32);
            ret = SendSingleFile(&htmlResultFile, htmlFilePath);

            if (true != ret)
                return ret;
        }

        if (0 < htmlFolderFileCnt)
            ret = SendFiles2SyncServer(htmlImageFolderPath, dirName);

        return ret;
    }

    bool RserverConnecter::SendSingleFile(st_ResultFile *pResultFile,
                                          char *m_ResultPath)
    {
        bool ret = false;
        //	printf("\nReady to upload,FilePath: %s.", m_ResultPath);
        Upstream = fopen(m_ResultPath, "rb");

        if (Upstream == NULL) {
            printf("\nerror occurred when reading file...\n");
            return ret;
        }

        int size = getlength(Upstream);
        printf("\nm_ResultPath = %s, file size = %d.", m_ResultPath, size);

        while (size > SERVER_SYNC_MAX_BLOCKSIZE) {
            ret = SendFileBlock(pResultFile);

            if (true != ret)
                return ret;

            size -= SERVER_SYNC_MAX_BLOCKSIZE;
        }

        if (size != 0) {
            pResultFile->nBlockLength = size;
            ret = SendFileFinish(pResultFile);
            size = 0;
            printf("upload successed!");
        } else {
            fclose(Upstream);
            Upstream = NULL;
            printf("upload successed!");
        }

        return ret;
    }

    bool RserverConnecter::SendFiles2SyncServer(const char *pFolderPath,
            const char *folderName)
    {
        bool ret = false;
        //    printf( "\nSendFiles2SyncServer,Folder path:%s\n", pFolderPath );
        DIR *pDir = opendir(pFolderPath);

        if (NULL == pDir) {
            return ret;
        }

        struct dirent *pEntity = NULL;

        while (NULL != (pEntity = readdir(pDir))) {
            if (8 == pEntity->d_type) {
                st_ResultFile resultFile;
                memset(&resultFile, 0, sizeof(st_ResultFile));
                memcpy(resultFile.szFolderName, folderName, 32);
                memcpy(resultFile.szFileName, pEntity->d_name, 32);
                //			resultFile.nBlockIndex = 0;
                //			resultFile.nBlockLength = 0;
                PathLength = strlen(pFolderPath) + strlen(pEntity->d_name);
                FilePath = (char *)malloc(sizeof(char) * (PathLength + 1));
                FilePath[PathLength] = '\0';
                memcpy(FilePath, pFolderPath, strlen(pFolderPath));
                memcpy(FilePath + strlen(pFolderPath), pEntity->d_name,
                       strlen(pEntity->d_name));
                //			printf("Ready to upload,FilePath:%s\n", FilePath);
                Upstream = fopen(FilePath, "rb");

                if (Upstream == NULL) {
                    printf("error occurred when reading file...\n");
                    return false;
                }

                int size = getlength(Upstream);
                printf("\nm_ResultPath = %s, file size = %d.", pFolderPath, size);

                while (size > SERVER_SYNC_MAX_BLOCKSIZE) {
                    ret = SendFileBlock(&resultFile);

                    if (true != ret)
                        return ret;

                    size -= SERVER_SYNC_MAX_BLOCKSIZE;
                }

                if (size != 0) {
                    resultFile.nBlockLength = size;
                    ret = SendFileFinish(&resultFile);

                    if (true != ret)
                        return ret;

                    size = 0;
                    printf("upload successed!");
                } else {
                    fclose(Upstream);
                    Upstream = NULL;
                    printf("upload successed!");
                }
            }
        }

        if (NULL != pDir) {
            closedir(pDir);
        }

        return true;
    }

    bool RserverConnecter::SendFileFinish(st_ResultFile *fp)
    {
        bool result = false;
        unsigned char sendbuffer[fp->nBlockLength + RESULTFILE_LEN];
        memcpy(sendbuffer, fp, RESULTFILE_LEN);
        int ret = fread(sendbuffer + RESULTFILE_LEN, fp->nBlockLength, 1, Upstream);

        if (ret != 1) {
            printf("error occurred when writing data to buffer...\n");
            return result;
        }

        result = m_psender->Send(eFileFinish, (const unsigned char *)&sendbuffer,
                                 sizeof(sendbuffer));
        fclose(Upstream);
        Upstream = NULL;
        return result;
    }

    bool RserverConnecter::SendFileBlock(st_ResultFile *fp)
    {
        bool result = false;
        fp->nBlockLength = SERVER_SYNC_MAX_BLOCKSIZE;
        unsigned char sendbuffer[fp->nBlockLength + RESULTFILE_LEN];
        memcpy(sendbuffer, fp, RESULTFILE_LEN);
        int ret = fread(sendbuffer + RESULTFILE_LEN, fp->nBlockLength, 1, Upstream);

        if (ret != 1) {
            printf("error occurred when writing data to buffer...\n");
            return result;
        }

        //	memcpy(sendbuffer+sizeof(st_ResultFile),Upstream,fp.nBlockLength);
        result = m_psender->Send(eFileBlock, (const unsigned char *)&sendbuffer,
                                 sizeof(sendbuffer));
        fp->nBlockIndex++;
        return result;
    }

    int RserverConnecter::GetSoftwareVersion()
    {
        return m_psender->SendCmd(eGetSoftwareVer);
    }

    int RserverConnecter::getlength(FILE *fp)
    {
        int cur_pos;
        int len;
        cur_pos = ftell(fp);
        fseek(fp, 0, SEEK_END);
        len = ftell(fp);
        fseek(fp, cur_pos, SEEK_SET);
        return len;
    }

    int RserverConnecter::RServerRegister(char DeviceType, char *SNum, char *Mac,
                                          char *CMMacm, char *Version, char *TechID, char *TechnicianName, char *Company,
                                          char *supervisorid)
    {
        int nRet = RC_FAIL;
        st_RserverDeviceInfoExt cmd;
        memset(&cmd, 0, sizeof(st_RserverDeviceInfoExt));
        cmd.cDeviceType = DeviceType;
        memcpy(cmd.szSerialNum, SNum, sizeof(cmd.szSerialNum));
        memcpy(cmd.Mac, Mac, sizeof(cmd.Mac));
        memcpy(cmd.CMMac, CMMacm, sizeof(cmd.CMMac));
        memcpy(cmd.szVersion, Version, sizeof(cmd.szVersion));
        memcpy(cmd.szTechID, TechID, sizeof(cmd.szTechID));
        memcpy(cmd.TechnicianName, TechnicianName, sizeof(cmd.TechnicianName));
        memcpy(cmd.szCompany, Company, sizeof(cmd.szCompany));
        memcpy(cmd.szSupervisorID, supervisorid, sizeof(cmd.szSupervisorID));
        nRet = m_psender->SendCmd(eRegisterTargetExt, (const unsigned char *)&cmd,
                                  sizeof(cmd));
        printf("\nRServerRegister,SN = %s, version = %s, devicetype = %d.",
               cmd.szSerialNum, cmd.szVersion, cmd.cDeviceType);
        return nRet;
    }
    int RserverConnecter::RServerUnRegister(char DeviceType, char *SNum, char *Mac,
                                            char *CMMacm, char *Version, char *TechID, char *TechnicianName, char *Company)
    {
        int nRet = RC_FAIL;
        st_RserverDeviceInfo cmd;
        memset(&cmd, 0, sizeof(st_RserverDeviceInfo));
        cmd.cDeviceType = DeviceType;
        memcpy(cmd.szSerialNum, SNum, sizeof(cmd.szSerialNum));
        memcpy(cmd.Mac, Mac, sizeof(cmd.Mac));
        memcpy(cmd.CMMac, CMMacm, sizeof(cmd.CMMac));
        memcpy(cmd.szVersion, Version, sizeof(cmd.szVersion));
        memcpy(cmd.szTechID, TechID, sizeof(cmd.szTechID));
        memcpy(cmd.TechnicianName, TechnicianName, sizeof(cmd.TechnicianName));
        memcpy(cmd.szCompany, Company, sizeof(cmd.szCompany));
        nRet = m_psender->SendCmd(eUnRegistTarget, (const unsigned char *)&cmd,
                                  sizeof(cmd));
        printf("\nRServerUnRegister,SN = %s, version = %s.", cmd.szSerialNum,
               cmd.szVersion);
        return nRet;
    }

    int RserverConnecter::RServerCheck(char DeviceType, char *SNum, char *Mac,
                                       char *CMMacm, char *Version, char *TechID, char *TechnicianName, char *Company)
    {
        int nRet = RC_FAIL;
        st_RserverDeviceInfo cmd;
        memset(&cmd, 0, sizeof(st_RserverDeviceInfo));
        cmd.cDeviceType = DeviceType;
        memcpy(cmd.szSerialNum, SNum, sizeof(cmd.szSerialNum));
        memcpy(cmd.Mac, Mac, sizeof(cmd.Mac));
        memcpy(cmd.CMMac, CMMacm, sizeof(cmd.CMMac));
        memcpy(cmd.szVersion, Version, sizeof(cmd.szVersion));
        memcpy(cmd.szTechID, TechID, sizeof(cmd.szTechID));
        memcpy(cmd.TechnicianName, TechnicianName, sizeof(cmd.TechnicianName));
        memcpy(cmd.szCompany, Company, sizeof(cmd.szCompany));
        nRet = m_psender->SendCmd(eCheckTarget, (const unsigned char *)&cmd,
                                  sizeof(cmd));
        printf("\nRServerCheck,Type = %d, SN = %s, version = %s.", cmd.cDeviceType,
               cmd.szSerialNum, cmd.szVersion);
        return nRet;
    }

    int RserverConnecter::RServerGetDeviceInfo(char DeviceType, char *SNum,
            char *Mac, char *CMMacm, char *Version, char *TechID, char *TechnicianName,
            char *Company, char *supervisorid)
    {
        int nRet = RC_FAIL;
        st_RserverDeviceInfoExt cmd;
        memset(&cmd, 0, sizeof(st_RserverDeviceInfoExt));
        cmd.cDeviceType = DeviceType;
        memcpy(cmd.szSerialNum, SNum, sizeof(cmd.szSerialNum));
        memcpy(cmd.Mac, Mac, sizeof(cmd.Mac));
        memcpy(cmd.CMMac, CMMacm, sizeof(cmd.CMMac));
        memcpy(cmd.szVersion, Version, sizeof(cmd.szVersion));
        memcpy(cmd.szTechID, TechID, sizeof(cmd.szTechID));
        memcpy(cmd.TechnicianName, TechnicianName, sizeof(cmd.TechnicianName));
        memcpy(cmd.szCompany, Company, sizeof(cmd.szCompany));
        memcpy(cmd.szSupervisorID, supervisorid, sizeof(cmd.szSupervisorID));
        nRet = m_psender->SendCmd(eGetDevInfoExt, (const unsigned char *)&cmd,
                                  sizeof(cmd));
        printf("\nRServerGetDeviceInfo,SN = %s, version = %s.", cmd.szSerialNum,
               cmd.szVersion);
        return nRet;
    }

    int RserverConnecter::RServerGetProfileInfo()
    {
        return m_psender->SendCmd(eGetProfileInfo);
    }

    int RserverConnecter::RServerGetProfileInfo(st_RserverProfileInfoArray
            *pInfoArray, int size)
    {
        return m_psender->SendCmd(eGetProfileInfo, (const unsigned char *)pInfoArray,
                                  size);
    }

    int RserverConnecter::RServerDownloadProfile(char *ProfileName, int type)
    {
        st_GetProfileFile cmd;
        memset(&cmd, 0, sizeof(st_GetProfileFile));
        memcpy(cmd.Name, ProfileName, sizeof(cmd.Name));
        cmd.Type = type;
        return m_psender->SendCmd(eGetProfileFile, (const unsigned char *)&cmd,
                                  sizeof(cmd));
    }

    int RserverConnecter::RServerDownloadAllProfile()
    {
        return m_psender->SendCmd(eGetAllProfile);
    }

    int RserverConnecter::RServerGetOrgChartStatus()
    {
        return m_psender->SendCmd(eGetOrgChartStatus);
    }

    int RserverConnecter::RServerGetOrgChartInfo(char *supervisorId)
    {
        st_SupervisorID cmd;
        memset(&cmd, 0, sizeof(st_SupervisorID));
        memcpy(cmd.supervisorId, supervisorId, sizeof(cmd.supervisorId));
        return m_psender->SendCmd(eGetOrgChartInfo, (const unsigned char *)&cmd,
                                  sizeof(cmd));
    }

    int RserverConnecter:: RServerGetServerInfo()
    {
        return m_psender->SendCmd(eGetServerInfo);
    }
}



