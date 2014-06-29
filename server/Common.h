#ifndef COMMON_H
#define COMMON_H

#include <time.h>
#include <string.h>

#include <inttypes.h>
#include <sys/time.h>
#include <queue>
using namespace std;
#define ERROR_MSG_MAX		512
#define POLL_WAIT_TIME		100
#define POLL_SEND_MAX		100
#define Q_SLEEP_TIME		100

#pragma pack(1)

#ifdef REPORTSHM
typedef struct
{
	//机器状态
	uint32_t iServerId;
	uint32_t iReq;
	uint32_t iDelay;
	uint32_t iMaxDelay;
	uint32_t lLastAccessTime;		//最后访问时间
} ServerState;
typedef struct
{
	uint32_t iServerId;
	int iNowConfIndex;	//指向最新更新的配置共享内存的指针
	ServerState astStateShm[2];
} LocalStateShm;
#endif

enum
{
    UDP_REQUEST = 0,
    TCP_REQUEST,
    UDP_RESPONSE,
    TCP_RESPONSE,

    INT_REQUEST,
    INT_RESPONSE,

    PAD_REQUEST,
    NET_SOCKET_CLOSE
};

struct shm_block
{
	int length;
	// 32(ip)+16(port)+16(socketfd)
	int64_t id;	//cid
	unsigned char protocol;	//协议类型
	/*
	* 0: extern call request
	* 1: extern call response
	* 2: intern call request
	* 3: intern call response
	* 4: fill bytes, discard
	*/
	int64_t sockinfo;
	char type;
	char flag;  //note here close flag :0 mean send and close fd, 1 mean only send,3 mean send and close but svc no clean
	struct timeval stTv;
	char* data;
};
#pragma pack()

#define MB_HEAD_LENGTH	((int)(sizeof (shm_block) - sizeof (char*)))
#define MB_DATA_LENGTH(mb)	(mb->length - MB_HEAD_LENGTH)

//pipe
enum
{
	UDP_LISTENING = 0,
	TCP_LISTENING,
	PIPE_STREAM,
	TCP_STREAM,
};


//HTTP_PROCESS_RESULT
enum
{
	HTTP_SUCCESS = 0,
	HTTP_LONGCONN_SUCCESS,
	HTTP_AUTH_ERROR,
	HTTP_USERBIND_ERROR,
	HTTP_SESSION_ERROR,
	HTTP_MSGDB_ERROR,
};

typedef unsigned int uint;
typedef int64_t lint;


typedef struct _tagUser_Session
{
	uint	uin;	// login uin
	int		ip;		// login ip
	time_t	create;	// session create time
	time_t	stamp;	// session op time
	lint 	cid;	// connect session id
	char 	sKey[52];	//Key
	_tagUser_Session()
	{
		uin = 0;
		ip = 0;
		create = 0;
		stamp = 0;
		cid = 0;
		memset(sKey, 0, sizeof(sKey));
	}
} User_Session;


struct Connect_Session_t
{
	int64_t id;
	char	type;
	unsigned char protocol;
	struct timeval stTv;
	char* recv_mb;
	int recv_len;
	char* send_mb;
	int send_len;   // byte need be send

	int send_pos;   // send pos ... 
	
	char flag;		//note here close flag 0 mean send and close fd 1 mean only send

	queue<shm_block*> recvqueue;
	queue<shm_block*> sendqueue;
	
	Connect_Session_t (int64_t key, char t,int p);
	~Connect_Session_t ();
};

extern int64_t CONNECTION_ID (int ip, unsigned short port, int fd); 
extern int load_dll(const char* dll_name);
extern void init_fdlimit ();
extern void daemon_start ();

#define CONNECTION_FD(x)	(x&0xFFFFFFFF)
#define CONNECTION_IP(x)	((x>>32)&0xFFFFFFFF)

typedef int (*init_factory)(int,char**,int, bool);
typedef void (*fini_factory)(int);
typedef void (*stat_factory)(int,int,int);

typedef int (*handle_input_factory)(const char*,int,int64_t ,int);
typedef int (*handle_close_factory)(int64_t);
typedef int (*handle_process_factory)(char*,int&,int64_t,int);

extern bool stopped;
extern init_factory app_init;
extern fini_factory app_fini;
extern stat_factory app_stat;
//worker
extern handle_process_factory app_process;
//network
extern handle_close_factory app_close;
extern handle_input_factory app_input;

#endif

