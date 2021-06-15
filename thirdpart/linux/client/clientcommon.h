/*****************************************************************************************
** Client.h: Common data structure.
**
** Author: Leo.Tang
** Copyright (C) 2007 VeTronics(BeiJing) Ltd.
**
** Create date: 08-15-2007
*****************************************************************************************/
#ifndef clientcommon_class
#define clientcommon_class

#define	IN
#define	OUT
//const
//#define	null						0
//IDLE is changed to 5 for on-demand on 2016-06-15
#define	KEEPALIVE_IDLE				(5) //(2) //1
#define	KEEPALIVE_INTERVAL			5
#define	KEEPALIVE_COUNT				3
#define	MAX_SIZE_OF_PACKET			32768	//4096
#define	RECEIVE_BUFFER_SIZE			65536
#define	SEND_BUFFER_SIZE			65536
#define	APP_FLAG					"CX180"

#ifndef TRACE_ERROR
#ifdef DEBUG
#define TRACE_ERROR do {printf("### ERROR: FILE:%s\t Func:%s\t%d\n", __FILE__, __FUNCTION__, __LINE__); } while(0)
#else
#ifndef TRACE_ERROR
#define TRACE_ERROR do{} while(0)
#endif
#endif
#endif

enum eRetCode {
    RC_SUCCESS = 0,
    RC_INVPARAM,
    RC_FAIL,
    RC_NOTFOUND,
    RC_DUPLICATE,
    RC_NOTIMPL,
    RC_NOTPERMIT,
    RC_RCF_EXCEPT,
    RC_NOTINIT,
    RC_CKSUMERR,
    RC_SOCKCLOSE,
    RC_RESUSED,
    RC_CLOSECONN,
    RC_DISABLED,
    RC_INPROGRESS,
    RC_TIMOUT,
};

#define SAFE_DESTORY_POINTER(ptr) 					\
    do	{										\
        if (ptr) {								\
            delete ptr; 						\
            ptr = NULL; 						\
        }										\
    } while (0)


#define SAFE_DESTORY_SOCKET(sock) 					\
    do	{							\
        if (sock > 0) {				\
            close(sock); 			\
            sock = -1; 				\
        }							\
    } while (0)

#ifndef INVALID_SOCKET
#define	INVALID_SOCKET				(-1)
#endif

#define	SOCKET_ERROR				(-1)
#define	CONECT_TIMEOUT_SECONDS		5
#define	DISCONECT_TIMEOUT_SECONDS	5
#define	SEND_TIMEOUT_SECONDS		10
#define	MAX_EVENTS_IN_QUEUE			256
#define	EXECUTE_WAIT_SECONDS		5

#define	DISCONNECT_FAILED			0x01
#define	CREATE_THREAD_FAILED		0x02
#define	SET_SOCKET_OPT_FAILED		0x03

//message
#define	MSG_DATA_CORRECT	        0x00
#define	MSG_DATA_ERROR	            0x01
#define	MSG_CONNECT	            0x02

//port
#define CX180R_PORT             19000
//network
typedef	int Socket;

//sizeof(FrameHeader) = 32bytes
//|Hash(16bytes)|AppFlag(8bytes)|DataLen(4bytes)|Type(2bytes)|
typedef struct st_FrameHeader {
    unsigned char	MD5Hash[16];
    char			AppFlag[8];
    unsigned int	DataLength;
    unsigned int	Type;

    st_FrameHeader()
    {
        for (int i = 0; i < 16; i++) {
            MD5Hash[i] = 0;
        }

        for (int i = 0; i < 8; i++) {
            AppFlag[i] = 0;
        }
    }

} FrameHeader;

typedef struct st_ReceivedData {
    unsigned int		Type;
    unsigned char	    *Data;
    unsigned int		Length;

    st_ReceivedData()
    {
        Data = 0;
    }

    ~st_ReceivedData()
    {
        if (0 != Data) {
            delete []Data;
            Data = 0;
        }
    }

} ReceivedData;

#endif  // common_class
