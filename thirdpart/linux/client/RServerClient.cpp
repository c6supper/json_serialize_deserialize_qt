

#include "RServerClient.h"
#include <unistd.h>

using namespace std;


namespace rserver
{

#define TIMEOUT SEND_TIMEOUT_SECONDS


    CRServerClient::CRServerClient()
    {
        m_profFileStream = NULL;
        m_packageFileStream = NULL;
        messageCallback = NULL;
        SWTotalSize = 0;
        SoftwareDownloadProgress = 0;
        stream = NULL;
        m_nProfStCurrentCount = 0;
        ProfileInfoResult = NULL;
        m_nCheckRes = -1;
        m_bCheckResReady = false;
        m_Length = -1;
        m_nRegistRes = -1;
        m_nUnRegistRes = -1;
        m_nProfInfoCount = -1;
        m_nProfStCount = -1;
        m_nProfFileCount = -1;
        ProfileFilePath = NULL;
        OrgStatus = -1;
        m_nErrorCode = -1;
        pServerinfo = NULL;
        pDeviceInfo_300 = NULL;
        pAdminSetting = NULL;
        pCurrentVersion = NULL;
        pDeviceinfo = NULL;
        pOrgChartInfo = NULL;
        this->m_nArryLen = -1;
        m_bDevInfoReady = false;
        m_bGetVersionReady = false;
        /*        ResetUpgradeParam();
                ResetDownloadParam();*/
        sem_init(&m_hSem, 0, 1);
    }

    CRServerClient::~CRServerClient()
    {
        sem_destroy(&m_hSem);
    }

    void *ConnectThread(void *para)
    {
        /*
           int ret;
           stConnectCmd *pConnectCmd = (stConnectCmd *)para;
           ret = pthread_detach(pthread_self());

           if (0 != ret) {
               printf("Detach thread failed, error: %d.\n", ret);
           }

           if (!((CRServerClient *)pConnectCmd->rServerClint)->getCurrentConnectStatus()) {
               ret = ((CRServerClient *)pConnectCmd->rServerClint)->Connect(
                         pConnectCmd->ipaddr, pConnectCmd->port, pConnectCmd->device);

               if (true != ret) {
                   printf("Send command failed, error: %d.\n", ret);
                   ((CRServerClient *)pConnectCmd->rServerClint)->ConstructMsg(eConnectFail,
                           0, 0);
                   free(para);
                   return NULL;
               }
           } else {
               sleep(1);
           }

           ((CRServerClient *)pConnectCmd->rServerClint)->ConstructMsg(pConnectCmd->type,
                   0, 0);
           free(para);
           return NULL;
        */
    }

    bool CRServerClient::ConnectCmd(const char *IPAddress, unsigned short port,
                                    const char *device, unsigned int type)
    {
        /*stConnectCmd *pConnectCmd = (stConnectCmd *)malloc(sizeof(stConnectCmd));

        if (NULL == pConnectCmd) {
            printf("\nmalloc memory failed.");
            ConstructMsg(type, 0, 0);
            return  false;
        }

        memset(pConnectCmd, 0, sizeof(stConnectCmd));
        pConnectCmd->rServerClint = (void *)this;
        pConnectCmd->port = port;
        pConnectCmd->type = type;
        strncpy(pConnectCmd->ipaddr, IPAddress, sizeof(pConnectCmd->ipaddr));
        strncpy(pConnectCmd->device, device, sizeof(pConnectCmd->device));
        pConnectCmd->ipaddr[sizeof(pConnectCmd->ipaddr) - 1] = '\0';
        pConnectCmd->device[sizeof(pConnectCmd->device) - 1] = '\0';
        pthread_t l_ConnectThread;
        int ret = pthread_create(&l_ConnectThread, 0, ConnectThread,
                                 (void *)pConnectCmd);

        if (0 != ret) {
            printf("Create thread failed, error: %d.\n", ret);
            ConstructMsg(type, 0, 0);
            l_ConnectThread = 0;
            free(pConnectCmd);
            pConnectCmd = NULL;
            return false;
        }

        return true;*/
        bool ret = true;

        if (!getCurrentConnectStatus()) {
            ret = Connect(IPAddress, port, device);

            if (!ret) {
                printf("connect failed, error: %d.\n", ret);
                ConstructMsg(eConnectFail, 0, 0);
                return ret;
            }
        }

        ConstructMsg(type, 0, 0);
        return ret;
    }

    void *SendThread(void *para)
    {
        /*
           int ret;
           stSendCmd *pSendCmd = (stSendCmd *)para;
           ret = pthread_detach(pthread_self());

           if (0 != ret) {
               printf("Detach thread failed, error: %d.\n", ret);
           }

           sem_wait(&((CRServerClient *)pSendCmd->rServerClint)->m_hSem);
           ret = ((CRServerClient *)pSendCmd->rServerClint)->Send(pSendCmd->type,
                   pSendCmd->data, pSendCmd->length, TIMEOUT);
           sem_post(&((CRServerClient *)pSendCmd->rServerClint)->m_hSem);

           if (true != ret) {
               printf("Send command failed, error: %d.\n", ret);
               ((CRServerClient *)pSendCmd->rServerClint)->ConstructMsg(eSendCmdFail, 0, 0);
           } else {
               ((CRServerClient *)pSendCmd->rServerClint)->ConstructMsg(eSendCmdSucceed,
                       (unsigned char *)&pSendCmd->type, sizeof(pSendCmd->type));
           }

           free(para);
           return NULL;
        */
    }

    bool CRServerClient::SendCmd(unsigned int type, const unsigned char *data,
                                 unsigned int length)
    {
        /*       stSendCmd *pSendCmd = (stSendCmd *)malloc(sizeof(stSendCmd) + length - sizeof(
                                         char));

               if (NULL == pSendCmd) {
                   printf("\nmalloc memory failed.");
                   ConstructMsg(eSendCmdFail, 0, 0);
                   return false;
               }

               memset(pSendCmd, 0, sizeof(stSendCmd) + length - sizeof(char));
               pSendCmd->rServerClint = (void *)this;
               pSendCmd->type = type;
               pSendCmd->length = length;
               memcpy((void *)pSendCmd->data, data, length);
               pthread_t l_sendThread;
               int ret = pthread_create(&l_sendThread, 0, SendThread, (void *)pSendCmd);

               if (0 != ret) {
                   printf("Create thread failed, error: %d.\n", ret);
                   ConstructMsg(eSendCmdFail, 0, 0);
                   l_sendThread = 0;
                   free(pSendCmd);
                   pSendCmd = NULL;
                   return false;
               }*/
        bool ret = Send(type, data, length, TIMEOUT);

        if (true != ret) {
            printf("Send command failed, error: %d.\n", ret);
            ConstructMsg(eSendCmdFail, 0, 0);
        } else {
            ConstructMsg(eSendCmdSucceed, (const unsigned char *)&type, sizeof(type));
        }

        return ret;
    }

    void CRServerClient::OnReceived(unsigned int type, const unsigned char *data,
                                    unsigned int length)
    {
        //        printf("\n CRServerClient.OnReceived, type = 0x%x, length = %d.", type, length);
        m_Length = length;

        switch (type) {
        default:
            CatchError(eUnknowReply);
            break;
            /*
                    case eVerifyResult: {
                        m_nCheckRes = data[0];
                        m_bCheckResReady = true;
                        break;
                    }

                    case eRegistResult: {
                        switch (data[0]) {
                        case 0:
                            if (m_nCheckRes == NOT_REGISTER) {
                                m_nCheckRes = REGISTER_NOT_AUTHORIZE;
                            }

                            break;

                        default:
                            break;
                        }

                        m_nRegistRes = data[0];
                        break;
                    }

                    case eUnRegistResult: {
                        switch (data[0]) {
                        case 0:
                            m_nCheckRes = NOT_REGISTER;
                            break;

                        default:
                            break;
                        }

                        m_nUnRegistRes = data[0];
                        break;
                    }

                    case eGetProfileInfo: {
                        if (sizeof(int) != length) {
                            CatchError(eInvalidDataLength);
                            return;
                        }

                        if (ProfileInfoResult != NULL) {
                            ResetProfileInfoResult();
                        }

                        memcpy(&m_nProfInfoCount, data, sizeof(int));
                        m_nProfStCount = m_nProfInfoCount / 100;
                        printf("m_nProfInfoCount = %d, m_nProfStCount = %d.\n", m_nProfInfoCount,
                               m_nProfStCount);
                        break;
                    }

                    case eProfileInfo: {
                        if (length < sizeof(st_ProfileInfo)) {
                            CatchError(eInvalidDataLength);
                            return;
                        }

                        printf("Get profile info unfinished\n");

                        if (ProfileInfoResult == NULL) {
                            ProfileInfoResult = (st_ProfileInfo *)malloc(sizeof(st_ProfileInfo) *
                                                (m_nProfStCount + 1));
                        }

                        memset(ProfileInfoResult, 0, sizeof(st_ProfileInfo) * (m_nProfStCount + 1));
                        memcpy(&ProfileInfoResult[m_nProfStCurrentCount], (st_ProfileInfo *)data,
                               sizeof(st_ProfileInfo));
                        m_nProfStCurrentCount++;
                        break;
                    }

                    case eProfileInfoEnd: {
                        if (length < sizeof(st_ProfileInfo)) {
                            CatchError(eInvalidDataLength);
                            return;
                        }

                        if (ProfileInfoResult == NULL) {
                            ProfileInfoResult = (st_ProfileInfo *)malloc(sizeof(st_ProfileInfo) *
                                                (m_nProfStCount + 1));
                            memset(ProfileInfoResult, 0, sizeof(st_ProfileInfo) * (m_nProfStCount + 1));
                        }

                        memcpy(&ProfileInfoResult[m_nProfStCurrentCount], (st_ProfileInfo *)data,
                               sizeof(st_ProfileInfo));
                        m_nProfStCurrentCount = 0;
                        printf("\nGet profile info finished\n");
                        break;
                    }

                    case eGetServerInfo: {
                        if (length != sizeof(st_SyncServerInfo)) {
                            CatchError(eInvalidDataLength);
                            return;
                        }

                        if (pServerinfo == NULL) {
                            pServerinfo = (st_SyncServerInfo *)malloc(sizeof(st_SyncServerInfo));
                        }

                        memset(pServerinfo, 0, sizeof(st_SyncServerInfo));
                        memcpy(pServerinfo, (st_SyncServerInfo *)data, sizeof(st_SyncServerInfo));
                        printf("Got server information\n");
                        break;
                    }

                    case eGetDeviceInfo: {
                        if (length != sizeof(st_DeviceInfo_300)) {
                            CatchError(eInvalidDataLength);
                            return;
                        }

                        if (pDeviceInfo_300 == NULL) {
                            pDeviceInfo_300 = (st_DeviceInfo_300 *)malloc(sizeof(st_DeviceInfo_300));
                        }

                        memset(pDeviceInfo_300, 0, sizeof(st_DeviceInfo_300));
                        memcpy(pDeviceInfo_300, (st_DeviceInfo_300 *)data, sizeof(st_DeviceInfo_300));
                        printf("Got Device information\n");
                        break;
                    }

                    case eAdminSetting: {
                        if (length != sizeof(st_AdminSetting)) {
                            CatchError(eInvalidDataLength);
                            return;
                        }

                        if (pAdminSetting == NULL) {
                            pAdminSetting = (st_AdminSetting *)malloc(sizeof(st_AdminSetting));
                        }

                        memset(pAdminSetting, 0, sizeof(st_AdminSetting));
                        memcpy(pAdminSetting, (st_AdminSetting *)data, sizeof(st_AdminSetting));
                        printf("Got AdminSetting\n");
                        break;
                    }

                    case eSoftwareVersion:
                        break;

                    case eDeviceInfo: {
                        if (length != sizeof(st_DeviceInfo_300)) {
                            CatchError(eInvalidDataLength);
                            return;
                        }

                        if (pDeviceinfo == NULL) {
                            pDeviceinfo = (st_DeviceInfo_300 *)malloc(sizeof(st_DeviceInfo_300));
                        }

                        memset(pDeviceinfo, 0, sizeof(st_DeviceInfo_300));
                        memcpy(pDeviceinfo, (st_DeviceInfo_300 *)data, sizeof(st_DeviceInfo_300));
                        printf("Got DeviceInfo\n");
                        m_bDevInfoReady = true;
                        break;
                    }

                    case eGetAdminSettingExt: {
                        //not yet completed
                        char *LockRegistration = (char *)data;
                        BYTE *a;
                        a = (BYTE *)"abc";
                        printf("Got LockRegistration\n");
                        cout << LockRegistration << (char *)a << endl;
                        break;
                    }

                    case eGetOrgChartStatus: {
                        if (length != sizeof(BYTE)) {
                            CatchError(eInvalidDataLength);
                            return;
                        }

                        OrgStatus = *(BYTE *)data;
                        cout << "OrgChart Management Status is" << OrgStatus << endl;
                        break;
                    }

                    case eGetOrgChartInfo: {
                        if (length != sizeof(st_OrgChartInfo)) {
                            CatchError(eInvalidDataLength);
                            return;
                        }

                        if (pOrgChartInfo == NULL) {
                            pOrgChartInfo = (st_OrgChartInfo *)malloc(sizeof(st_OrgChartInfo));
                        }

                        memset(pOrgChartInfo, 0, sizeof(st_OrgChartInfo));
                        memcpy(pOrgChartInfo, (st_OrgChartInfo *)data, sizeof(st_OrgChartInfo));
                        printf("Got OrgCharInfo\n");
                        break;
                    }

                    case eGetProfileFile: {
                        printf("\n *********eGetProfileFile********** \n");
                        break;
                    }

                    case eGetAllProfile: {
                        if (m_profFileStream != NULL || m_receiveSize != 0
                            || m_nReceivedProfFileCount != 0)
                            return;

                        ResetDownloadParam();

                        if (length != sizeof(int)) {
                            CatchError(eInvalidDataLength);
                            ConstructMsg(eDownloadFail, NULL, 0);
                            return;
                        }

                        memcpy(&m_nProfFileCount, data, sizeof(int));
                        break;
                    }

                    case eProfileFileBlk: {
                        printf("\neProfileFileBlk.");

                        if (length < sizeof(st_ProfileFile)) {
                            CatchError(eInvalidDataLength);
                            ConstructMsg(eDownloadFail, NULL, 0);
                            return;
                        }

                        if (false == profileDownloadAndVerify((const void *)data, length))
                            return;

                        ConstructMsg(type, NULL, 0);
                        return;
                    }

                    case eProfileFileEnd: {
                        printf("\neProfileFileEnd.");

                        if (length < sizeof(st_ProfileFile)) {
                            CatchError(eInvalidDataLength);
                            ConstructMsg(eDownloadFail, NULL, 0);
                            return;
                        }

                        st_ProfileFile *pFileHeader = (st_ProfileFile *)data;

                        if (pFileHeader->nLength - m_receiveSize != (int)(length - sizeof(
                                    st_ProfileFile))) {
                            CatchError(eInvalidDataLength);
                            ConstructMsg(eDownloadFail, NULL, 0);
                            return;
                        }

                        if (false == profileDownloadAndVerify((const void *)data, length))
                            return;

                         Modify mtime
                        char szFilePath[128];
                        getAbsolutePath(szFilePath, pFileHeader->nType, pFileHeader->szName);

                        for (int i = 0; i < (m_nProfStCount + 1); i++) {
                            for (int j = 0; j < ProfileInfoResult[i].nCount; j++) {
                                if ((strncmp(ProfileInfoResult[i].ArrInfo[j].szName, pFileHeader->szName,
                                             strlen(pFileHeader->szName)) == 0)
                                    && (ProfileInfoResult[i].ArrInfo[j].nType == pFileHeader->nType)) {
                                    time_t ModTime = convertTime(ProfileInfoResult[i].ArrInfo[j].nArrDate);
                                    struct utimbuf uid_time;
                                    uid_time.actime  = ModTime;
                                    uid_time.modtime = ModTime;
                                    utime(szFilePath, &uid_time);
                                }
                            }
                        }

                        if (++m_nReceivedProfFileCount == m_nProfFileCount) {
                            ResetDownloadParam();
                        } else {
                            m_receiveSize = 0;

                            if (NULL != m_profFileStream) {
                                fclose(m_profFileStream);
                                m_profFileStream = NULL;
                            }
                        }

                        ConstructMsg(type, NULL, 0);
                        return;
                    }

                    case eTestsetSWinfo: {
                        if (0 < m_packageTotalByte || m_packageFileStream != NULL)
                            return ;

                        ResetUpgradeParam();

                        if (length != sizeof(st_TestsetSWinfo)) {
                            printf("\neTestsetSWinfo.data length(%d) error.", length);
                            CatchError(eInvalidDataLength);
                            ConstructMsg(eUpgradeFail, NULL, 0);
                            return;
                        }

                        st_TestsetSWinfo *ptestsetswinfo = (st_TestsetSWinfo *)data;

                        if (ptestsetswinfo->Status != FinetoUpgrade) {
                            printf("\nError! Testset software status = %d.", ptestsetswinfo->Status);
                            CatchError(eNotFineToUpgrade);
                            ConstructMsg(type, data, length);
                            return;
                        }

                        if (0 >= (m_packageTotalByte = ptestsetswinfo->Length)) {
                            CatchError(eInvalidDataLength);
                            ConstructMsg(eUpgradeFail, NULL, 0);
                            ResetUpgradeParam();
                            return;
                        }

                        printf("\nGot testset software image information success!m_packageTotalByte = %d",
                               m_packageTotalByte);

                        if ((m_packageFileStream = createPackageFile(m_FilePath)) == NULL) {
                            CatchError(eInvalidDataLength);
                            ConstructMsg(eUpgradeFail, NULL, 0);
                            ResetUpgradeParam();
                        }

                        break;
                    }

                    case eTestsetSWBLK: {
                        if (0 >= m_packageTotalByte || m_packageFileStream == NULL)
                            return ;

                        if (length != SW_BLK_LENGTH) {
                            printf("\neTestsetSWBLK.data length(%d) error.", length);
                            CatchError(eInvalidDataLength);
                            ConstructMsg(eUpgradeFail, NULL, 0);
                            ResetUpgradeParam();
                            return;
                        }

                        if (!packageDownloadAndVerify((const void *)data, SW_BLK_LENGTH)) {
                            printf("\neTestsetSWBLK.write data failed.");
                            ConstructMsg(eUpgradeFail, NULL, 0);
                            ResetUpgradeParam();
                            return;
                        }

                        break;
                    }

                    case eTestsetSWEND: {
                        if (0 >= m_packageTotalByte || m_packageFileStream == NULL)
                            return ;

                        if (length != (m_packageTotalByte - m_packageDownloadByte)) {
                            printf("\eTestsetSWEND.last block length(%d) error, total length(%d), load length(%d).",
                                   length, m_packageTotalByte, m_packageDownloadByte);
                            CatchError(eInvalidDataLength);
                            ConstructMsg(eUpgradeFail, NULL, 0);
                            return;
                        }

                        if (!packageDownloadAndVerify(data, length)) {
                            printf("\neTestsetSWBLK.write data failed.");
                            ConstructMsg(eUpgradeFail, NULL, 0);
                            return;
                        }

                        ResetUpgradeParam();
                        break;
                    }
                    break;*/
        }

        ConstructMsg(type, data, length);
    }

    FILE *CRServerClient::createPackageFile(const char *filepath)
    {
        return fopen(filepath, "wb+");
    }

    bool CRServerClient::packageDownloadAndVerify(const void *data, uint32_t length)
    {
        if (fwrite((const void *)(data), length, 1, m_packageFileStream) != 1) {
            printf("\npackageDownloadAndVerify-fwrite fail.");
            return false;
        } else {
            fflush(m_packageFileStream);
            m_packageDownloadByte += length;
            printf("\npackageDownloadAndVerify(%d).",
                   m_packageDownloadByte * 100 / m_packageTotalByte);

            if (m_packageDownloadByte > m_packageTotalByte) {
                printf("\npackageDownloadAndVerify-m_packageDownloadByte(%d), m_packageTotalByte(%d).",
                       m_packageDownloadByte, m_packageTotalByte);
                return false;
            }
        }

        return true;
    }

    void CRServerClient::ResetDownloadParam()
    {
        printf("\n ResetDownloadParam.\n");
        m_nProfFileCount = 0;
        m_nReceivedProfFileCount = 0;
        m_receiveSize = 0;

        if (NULL != m_profFileStream) {
            fclose(m_profFileStream);
            m_profFileStream = NULL;
        }
    }

    bool CRServerClient::profileDownloadAndVerify(const void *data, uint32_t length)
    {
        st_ProfileFile *pFileHeader = (st_ProfileFile *)data;

        if (NULL == m_profFileStream) {
            char szFilePath[128];
            getAbsolutePath(szFilePath, pFileHeader->nType, pFileHeader->szName);

            if ((m_profFileStream = createPackageFile(szFilePath)) == NULL) {
                CatchError(eInvalidDataLength);
                ConstructMsg(eDownloadFail, NULL, 0);
                ResetDownloadParam();
                return false;
            }
        }

        if (pFileHeader->nLength == 0)
            return true;

        if (fwrite((const void *)((char *)pFileHeader + sizeof(st_ProfileFile)),
                   length - sizeof(st_ProfileFile), 1, m_profFileStream) != 1) {
            CatchError(eInvalidDataLength);
            ConstructMsg(eDownloadFail, NULL, 0);
            ResetDownloadParam();
            return false;
        }

        fflush(m_profFileStream);
        printf("\n m_receiveSize:%d, length - sizeof(st_ProfileFile):%d, pFileHeader->nLength:%d.",
               m_receiveSize, length - sizeof(st_ProfileFile), pFileHeader->nLength);
        m_receiveSize += length - sizeof(st_ProfileFile);

        if (m_receiveSize > pFileHeader->nLength)
            return false;

        return true;
    }

    void CRServerClient::getAbsolutePath(char *pPathBuf, int nType,
                                         const char *pFileName)
    {
        getAbsoluteFolderPath(pPathBuf, nType);
        createMulvPath(pPathBuf);
        strcat(pPathBuf, pFileName);
        return;
    }

    void CRServerClient::getAbsoluteFolderPath(char *pPathBuf, int nType)
    {
        pPathBuf[0] = 0;

        switch (nType) {
        case eProfBert_1GC1:
            strcpy(pPathBuf, "/home/1ge/mx300-prof/1gc1port/bert-setup/");
            break;

        case eProfBert_1GC2:
            strcpy(pPathBuf, "/home/1ge/mx300-prof/1gc2port/bert-setup/");
            break;

        case eProfBert_1GF1:
            strcpy(pPathBuf, "/home/1ge/mx300-prof/1gf1port/bert-setup/");
            break;

        case eProfBert_1GF2:
            strcpy(pPathBuf, "/home/1ge/mx300-prof/1gf2port/bert-setup/");
            break;

        case eProfBert_10G:
            strcpy(pPathBuf, "/home/10g/mx300-prof/10gport/bert-setup/");
            break;

        case eProfBert_10G2:
            strcpy(pPathBuf, "/home/10g/mx300-prof/10gport2/bert-setup/");
            break;

        case eProfRfc2544_1GC1:
            strcpy(pPathBuf, "/home/1ge/mx300-prof/1gc1port/rfc2544-setup/");
            break;

        case eProfRfc2544_1GC2:
            strcpy(pPathBuf, "/home/1ge/mx300-prof/1gc2port/rfc2544-setup/");
            break;

        case eProfRfc2544_1GF1:
            strcpy(pPathBuf, "/home/1ge/mx300-prof/1gf1port/rfc2544-setup/");
            break;

        case eProfRfc2544_1GF2:
            strcpy(pPathBuf, "/home/1ge/mx300-prof/1gf2port/rfc2544-setup/");
            break;

        case eProfRfc2544_10G:
            strcpy(pPathBuf, "/home/10g/mx300-prof/10gport/rfc2544-setup/");
            break;

        case eProfRfc2544_10G2:
            strcpy(pPathBuf, "/home/10g/mx300-prof/10gport2/rfc2544-setup/");
            break;

        case eProfThrpt_1GC1:
            strcpy(pPathBuf, "/home/1ge/mx300-prof/1gc1port/thrpt-setup/");
            break;

        case eProfThrpt_1GC2:
            strcpy(pPathBuf, "/home/1ge/mx300-prof/1gc2port/thrpt-setup/");
            break;

        case eProfThrpt_1GF1:
            strcpy(pPathBuf, "/home/1ge/mx300-prof/1gf1port/thrpt-setup/");
            break;

        case eProfThrpt_1GF2:
            strcpy(pPathBuf, "/home/1ge/mx300-prof/1gf2port/thrpt-setup/");
            break;

        case eProfThrpt_10G:
            strcpy(pPathBuf, "/home/10g/mx300-prof/10gport/thrpt-setup/");
            break;

        case eProfThrpt_10G2:
            strcpy(pPathBuf, "/home/10g/mx300-prof/10gport2/thrpt-setup/");
            break;

        case eProfVsam_1GC1:
            strcpy(pPathBuf, "/home/1ge/mx300-prof/1gc1port/y1564-setup/");
            break;

        case eProfVsam_1GC2:
            strcpy(pPathBuf, "/home/1ge/mx300-prof/1gc2port/y1564-setup/");
            break;

        case eProfVsam_1GF1:
            strcpy(pPathBuf, "/home/1ge/mx300-prof/1gf1port/y1564-setup/");
            break;

        case eProfVsam_1GF2:
            strcpy(pPathBuf, "/home/1ge/mx300-prof/1gf2port/y1564-setup/");
            break;

        case eProfVsam_10G:
            strcpy(pPathBuf, "/home/10g/mx300-prof/10gport/y1564-setup/");
            break;

        case eProfVsam_10G2:
            strcpy(pPathBuf, "/home/10g/mx300-prof/10gport2/y1564-setup/");
            break;

        case eProfScan_1GC1:
            strcpy(pPathBuf, "/home/1ge/mx300-prof/1gc1port/scan-setup/");
            break;

        case eProfScan_1GC2:
            strcpy(pPathBuf, "/home/1ge/mx300-prof/1gc2port/scan-setup/");
            break;

        case eProfScan_1GF1:
            strcpy(pPathBuf, "/home/1ge/mx300-prof/1gf1port/scan-setup/");
            break;

        case eProfScan_1GF2:
            strcpy(pPathBuf, "/home/1ge/mx300-prof/1gf2port/scan-setup/");
            break;

        case eProfScan_10G:
            strcpy(pPathBuf, "/home/10g/mx300-prof/10gport/scan-setup/");
            break;

        case eProfScan_10G2:
            strcpy(pPathBuf, "/home/10g/mx300-prof/10gport2/scan-setup/");
            break;

        case eProfVperf_1GC1:
            strcpy(pPathBuf, "/home/1ge/mx300-prof/1gc1port/vperf-setup/");
            break;

        case eProfVperf_1GC2:
            strcpy(pPathBuf, "/home/1ge/mx300-prof/1gc2port/vperf-setup/");
            break;

        case eProfVperf_1GF1:
            strcpy(pPathBuf, "/home/1ge/mx300-prof/1gf1port/vperf-setup/");
            break;

        case eProfVperf_1GF2:
            strcpy(pPathBuf, "/home/1ge/mx300-prof/1gf2port/vperf-setup/");
            break;

        case eProfVperf_10G:
            strcpy(pPathBuf, "/home/10g/mx300-prof/10gport/vperf-setup/");
            break;

        case eProfVperf_10G2:
            strcpy(pPathBuf, "/home/10g/mx300-prof/10gport2/vperf-setup/");
            break;

        case eProfFTPDownload_1GC1:
            strcpy(pPathBuf, "/home/1ge/mx300-prof/1gc1port/ftpget-setup/");
            break;

        case eProfFTPDownload_1GC2:
            strcpy(pPathBuf, "/home/1ge/mx300-prof/1gc2port/ftpget-setup/");
            break;

        case eProfFTPDownload_1GF1:
            strcpy(pPathBuf, "/home/1ge/mx300-prof/1gf1port/ftpget-setup/");
            break;

        case eProfFTPDownload_1GF2:
            strcpy(pPathBuf, "/home/1ge/mx300-prof/1gf2port/ftpget-setup/");
            break;

        case eProfFTPDownload_10G:
            strcpy(pPathBuf, "/home/10g/mx300-prof/10gport/ftpget-setup/");
            break;

        case eProfFTPDownload_10G2:
            strcpy(pPathBuf, "/home/10g/mx300-prof/10gport2/ftpget-setup/");
            break;

        case eProfFTPUpload_1GC1:
            strcpy(pPathBuf, "/home/1ge/mx300-prof/1gc1port/ftpput-setup/");
            break;

        case eProfFTPUpload_1GC2:
            strcpy(pPathBuf, "/home/1ge/mx300-prof/1gc2port/ftpput-setup/");
            break;

        case eProfFTPUpload_1GF1:
            strcpy(pPathBuf, "/home/1ge/mx300-prof/1gf1port/ftpput-setup/");
            break;

        case eProfFTPUpload_1GF2:
            strcpy(pPathBuf, "/home/1ge/mx300-prof/1gf2port/ftpput-setup/");
            break;

        case eProfFTPUpload_10G:
            strcpy(pPathBuf, "/home/10g/mx300-prof/10gport/ftpput-setup/");
            break;

        case eProfFTPUpload_10G2:
            strcpy(pPathBuf, "/home/10g/mx300-prof/10gport2/ftpput-setup/");
            break;

        case eProfFcBert_1GF1:
            strcpy(pPathBuf, "/home/1ge/fx300-prof/1gf1port/fbert-setup/");
            break;

        case eProfFcBert_1GF2:
            strcpy(pPathBuf, "/home/1ge/fx300-prof/1gf2port/fbert-setup/");
            break;

        case eProfFcBert_10G:
            strcpy(pPathBuf, "/home/10g/fx300-prof/10gport/fbert-setup/");
            break;

        case eProfFcBert_10G2:
            strcpy(pPathBuf, "/home/10g/fx300-prof/10gport2/fbert-setup/");
            break;

        case eProfFcRfc2544_1GF1:
            strcpy(pPathBuf, "/home/1ge/fx300-prof/1gf1port/frfc2544-setup/");
            break;

        case eProfFcRfc2544_1GF2:
            strcpy(pPathBuf, "/home/1ge/fx300-prof/1gf2port/frfc2544-setup/");
            break;

        case eProfFcRfc2544_10G:
            strcpy(pPathBuf, "/home/10g/fx300-prof/10gport/frfc2544-setup/");
            break;

        case eProfFcRfc2544_10G2:
            strcpy(pPathBuf, "/home/10g/fx300-prof/10gport2/frfc2544-setup/");
            break;

        case eProfFcThrpt_1GF1:
            strcpy(pPathBuf, "/home/1ge/fx300-prof/1gf1port/fthrpt-setup/");
            break;

        case eProfFcThrpt_1GF2:
            strcpy(pPathBuf, "/home/1ge/fx300-prof/1gf2port/fthrpt-setup/");
            break;

        case eProfFcThrpt_10G:
            strcpy(pPathBuf, "/home/10g/fx300-prof/10gport/fthrpt-setup/");
            break;

        case eProfFcThrpt_10G2:
            strcpy(pPathBuf, "/home/10g/fx300-prof/10gport2/fthrpt-setup/");
            break;

        default:
            strcpy(pPathBuf, "/home/root/");
            break;
        }

        return;
    }

    void CRServerClient::createMulvPath(char *muldir)
    {
        int  i, len;
        char str[128];
        strncpy(str, muldir, 128);
        len = strlen(str);

        for (i = 0; i < len; i++) {
            if ('/' == str[i]) {
                str[i] = '\0';

                if (0 != access(str, F_OK)) {
                    mkdir(str, 0755);
                }

                str[i] = '/';
            }
        }

        if (len > 0 && 0 != access(str, F_OK)) {
            mkdir(str, 0755);
        }

        return;
    }

    time_t CRServerClient::convertTime(int *pArrDate)
    {
        struct tm  tmFile;
        tmFile.tm_sec  = pArrDate[5];
        tmFile.tm_min  = pArrDate[4];
        tmFile.tm_hour = pArrDate[3];
        tmFile.tm_mday = pArrDate[2];
        tmFile.tm_mon  = pArrDate[1] - 1;
        tmFile.tm_year = pArrDate[0] - 1900;
        tmFile.tm_wday  = 0;
        tmFile.tm_yday  = 0;
        tmFile.tm_isdst = 0;
        time_t newTime = mktime(&tmFile);
        return newTime;
    }

    void CRServerClient::OnServerClose()
    {
        ConstructMsg(eServerClosed, 0, 0);
        ClientBase::OnServerClose();
    }

    void CRServerClient::OnSocketError()
    {
        ConstructMsg(eServerClosed, 0, 0);
        ClientBase::OnSocketError();
    }

    void CRServerClient::ConstructMsg(unsigned int type, const unsigned char *data,
                                      unsigned int length)
    {
        if (messageCallback != NULL) {
            messageCallback(objSender, type, data, length);
        }
    }

    void CRServerClient::ResetUpgradeParam()
    {
        printf("\n ResetUpgradeParam.\n");
        m_packageDownloadByte = 0;
        m_packageTotalByte = 0;

        if (NULL != m_packageFileStream) {
            fclose(m_packageFileStream);
            m_packageFileStream = NULL;
        }
    }

    int CRServerClient::GetProgress()
    {
        if (m_packageTotalByte != 0) {
            float Progress = ((float)m_packageDownloadByte / m_packageTotalByte) * 100;
            return static_cast<int>(Progress);
        } else {
            CatchError(eCantGetDLProgress);
            return 0;
        }
    }

    void CRServerClient::CatchError(int errorcode)
    {
        if (m_nErrorCode >= 0) {
            SetMultiErrorCode();
        } else {
            SetErrorCode(errorcode);
        }
    }

    void CRServerClient::SetMultiErrorCode()
    {
        m_nErrorCode = 100;
    }

    void CRServerClient::SetErrorCode(int errorcode)
    {
        m_nErrorCode = errorcode;
    }

}

