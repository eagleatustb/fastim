#ifndef OPTIONS_H
#define OPTIONS_H

#include "Ini_File.h"

struct Ini_Option_t
{
	char	log_dir[128];
	char	bind_file[128];

	char	log_prename[32];
	char	msglog_prename[32];
	short	log_priority;
	int		log_size;

	key_t	send_sem_key;
	key_t	recv_sem_key;
	int	shm_queue_length;
	
	//fdinfo
	key_t	fdinfo_sem_key;
	key_t	fdinfo_shm_key;
	int		fdinfo_capbility;
	
	//userinfo
	key_t	userinfo_sem_key;
	key_t	userinfo_shm_key;
	int		userinfo_capbility;

	//sessioninfo
	key_t	sessioninfo_sem_key;
	key_t	sessioninfo_shm_key;
	int		sessioninfo_capbility;	
	int		sessioninfo_length;	

	//msginfo
	key_t	msg_shm_key;
	
	//fifo
	char	fifo[128];

	int	socket_timeout;
	int	session_timeout;
	int	socket_bufsize;
	int	backlog;
	int	worker_proc_num;

	int	monitor_time;
	int check_timeout;
	
	int server_mode;

	//press_test
	bool press_test;

	//cpu id
	int cpu_id;
	
	//limit
	key_t	limit_sem_key;
	key_t	limit_shm_key;
	int		limit_capbility;
	int		limit_internal;
	int		limit_maxcount;
	int		limit_idle;

	char	userip[24];
	int		userport;

	char	infoip[24];
	char	apiip[24];
	int		infoport;
};

#define SERVER_MODE_LONG_POLL	1
#define SERVER_MODE_SERVER_PUSH	2

struct Bind_Option_t
{
	char 	ip[16];
	u_short port;
	unsigned char		type;
	unsigned char		protocol;
};

class Option
{
public:
	Option();
	~Option();

	int init(const char* config_file);
	void print();
	
	int bind_count;
protected:
	int	parse_network(const char* config_file);	
	int	parse_ini();

	IniFile ini_file;
};

extern Bind_Option_t bind_port[16];

extern Ini_Option_t ini;
extern Option option;
#endif

