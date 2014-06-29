#include "Options.h"
#include <string>
#include <string.h>
#include <stdio.h>
#include<sys/sysinfo.h>

using namespace std;

Bind_Option_t bind_port[16];
Ini_Option_t ini;
Option::Option()
{
	memset (&ini, 0x0, sizeof (ini));
	memset (&bind_port, 0x0, sizeof (bind_port));
	bind_count = 0;
}

Option::~Option()
{
}

int Option::init(const char* config_file)
{
	if (!ini_file.open (config_file))
	{
		printf ("open config file:%s failed\n", config_file);
		return -1;
	}

	if (parse_ini () != 0 || parse_network ((const char*)ini.bind_file) != 0)
		return -1;

	return 0;
}

int	Option::parse_ini()
{
	string tmp;
	//apiip
	tmp = ini_file.read ("apiip", "BASE_IP");
	if (tmp.empty ())
		return -1;
	else
		strncpy (ini.apiip, tmp.c_str(), sizeof ini.apiip);
	//
	tmp = ini_file.read ("ipc", "SEND_SEM_KEY");
	if (tmp.empty ())
		return -1;
	ini.send_sem_key = atoi (tmp.c_str());

	tmp = ini_file.read ("ipc", "RECV_SEM_KEY");
	if (tmp.empty ())
		return -1;
	ini.recv_sem_key = atoi (tmp.c_str());

	tmp = ini_file.read ("ipc", "SHM_QUEUE_LENGTH");
	if (tmp.empty ())
		return -1;
	ini.shm_queue_length = atoi (tmp.c_str());


	//fdinfo
	tmp = ini_file.read ("fdinfo", "FDINFO_SEM_KEY");
	if (tmp.empty ())
		return -1;
	ini.fdinfo_sem_key = atoi (tmp.c_str());

	tmp = ini_file.read ("fdinfo", "FDINFO_SHM_KEY");
	if (tmp.empty ())
		return -1;
	ini.fdinfo_shm_key = atoi (tmp.c_str());

	tmp = ini_file.read ("fdinfo", "FDINFO_CAPBILITY");
	if (tmp.empty ())
		return -1;
	ini.fdinfo_capbility = atoi (tmp.c_str());

	//userinfo
	tmp = ini_file.read ("userinfo", "USERINFO_SEM_KEY");
	if (tmp.empty ())
		return -1;
	ini.userinfo_sem_key = atoi (tmp.c_str());

	tmp = ini_file.read ("userinfo", "USERINFO_SHM_KEY");
	if (tmp.empty ())
		return -1;
	ini.userinfo_shm_key = atoi (tmp.c_str());

	tmp = ini_file.read ("userinfo", "USERINFO_CAPBILITY");
	if (tmp.empty ())
		return -1;
	ini.userinfo_capbility = atoi (tmp.c_str());

	//sessionrinfo
	tmp = ini_file.read ("sessioninfo", "SESSIONINFO_SEM_KEY");
	if (tmp.empty ())
		return -1;
	ini.sessioninfo_sem_key = atoi (tmp.c_str());

	tmp = ini_file.read ("sessioninfo", "SESSIONINFO_SHM_KEY");
	if (tmp.empty ())
		return -1;
	ini.sessioninfo_shm_key = atoi (tmp.c_str());

	tmp = ini_file.read ("sessioninfo", "SESSIONINFO_CAPBILITY");
	if (tmp.empty ())
		return -1;
	ini.sessioninfo_capbility = atoi (tmp.c_str());

	tmp = ini_file.read ("sessioninfo", "SESSIONINFO_LENGTH");
	if (tmp.empty ())
		return -1;
	ini.sessioninfo_length = atoi (tmp.c_str());

	//msginfo
	tmp = ini_file.read ("msginfo", "MSG_SHM_KEY");
	if (tmp.empty ())
		return -1;
	ini.msg_shm_key = atoi (tmp.c_str());

	//iplimit
	tmp = ini_file.read ("limit", "LIMIT_SEM_KEY");
	if (tmp.empty ())
		return -1;
	ini.limit_sem_key = atoi (tmp.c_str());

	tmp = ini_file.read ("limit", "LIMIT_SHM_KEY");
	if (tmp.empty ())
		return -1;
	ini.limit_shm_key = atoi (tmp.c_str());

	tmp = ini_file.read ("limit", "LIMIT_CAPBILITY");
	if (tmp.empty ())
		return -1;
	ini.limit_capbility = atoi (tmp.c_str());

	tmp = ini_file.read ("limit", "LIMIT_INTERNAL");
	if (tmp.empty ())
		return -1;
	ini.limit_internal = atoi (tmp.c_str());

	tmp = ini_file.read ("limit", "LIMIT_MAXCOUNT");
	if (tmp.empty ())
		return -1;
	ini.limit_maxcount = atoi (tmp.c_str());

	tmp = ini_file.read ("limit", "LIMIT_IDLE");
	if (tmp.empty ())
		return -1;
	ini.limit_idle = atoi (tmp.c_str());

	//fifo
	tmp = ini_file.read ("ipc", "FIFO_PATH");
	if (tmp.empty ())
		memset (ini.fifo, 0x0, sizeof ini.fifo);
	else
		strncpy (ini.fifo, tmp.c_str(), sizeof ini.fifo);

	tmp = ini_file.read ("logger", "LOG_PRENAME");
	if (tmp.empty ())
		memset (ini.log_prename, 0x0, sizeof ini.log_prename);
	else
		strncpy (ini.log_prename, tmp.c_str(), sizeof ini.log_prename);

	tmp = ini_file.read ("logger", "MSGLOG_PRENAME");
	if (tmp.empty ())
		memset (ini.msglog_prename, 0x0, sizeof ini.msglog_prename);
	else
		strncpy (ini.msglog_prename, tmp.c_str(), sizeof ini.msglog_prename);

	tmp = ini_file.read ("logger", "LOG_PRIORITY");
	if (tmp.empty ())
		return -1;
	ini.log_priority = atoi (tmp.c_str());

	tmp = ini_file.read ("logger", "LOG_SIZE");
	if (tmp.empty ())
		return -1;
	ini.log_size = atoi (tmp.c_str());

	tmp = ini_file.read ("misc", "SOCKET_TIMEOUT");
	if (tmp.empty ())
		return -1;
	ini.socket_timeout = atoi (tmp.c_str());

	tmp = ini_file.read ("misc", "CHECK_TIMEOUT");
	if (tmp.empty ())
		ini.check_timeout = 1;
	else
		ini.check_timeout = atoi (tmp.c_str());

	tmp = ini_file.read ("misc", "SESSION_TIMEOUT");
	if (tmp.empty ())
		return -1;
	ini.session_timeout = atoi (tmp.c_str());

	tmp = ini_file.read ("misc", "SOCKET_BUFSIZE");
	if (tmp.empty ())
		return -1;
	ini.socket_bufsize = atoi (tmp.c_str());

	tmp = ini_file.read ("misc", "ACCEPT_BACKLOG");
	if (tmp.empty ())
		ini.backlog = 10;
	else
		ini.backlog = atoi (tmp.c_str());

	tmp = ini_file.read ("misc", "MONITOR_TIME");
	if (tmp.empty ())
		ini.monitor_time = 10;
	else
		ini.monitor_time = atoi (tmp.c_str());

	tmp = ini_file.read ("misc", "WORKER_PROC_NUM");
	if (tmp.empty ())
		return -1;
	ini.worker_proc_num = atoi (tmp.c_str());

	tmp = ini_file.read ("misc", "SERVER_MODE");
	if (tmp.empty ())
		return -1;
	ini.server_mode = atoi (tmp.c_str());

	tmp = ini_file.read ("misc", "CPU_ID");
	int cid = sysconf(_SC_NPROCESSORS_CONF);
	if (tmp.empty () || (ini.cpu_id = atoi (tmp.c_str())) >= cid || ini.cpu_id < 0)
	{
		ini.cpu_id = cid - 1;	//分配在倒数第一个，因为倒数第一个和第一个CPU通常不是同一个物理CPU
	}

	tmp = ini_file.read ("path", "LOG_DIR");
	strncpy (ini.log_dir, tmp.c_str(), sizeof ini.log_dir);

	tmp = ini_file.read ("path", "BIND_FILE");
	strncpy (ini.bind_file, tmp.c_str(), sizeof ini.bind_file);
	if (strlen (ini.log_dir) == 0 || strlen(ini.bind_file) == 0)
	{
		printf("config file LOG_DIR:%s,BIND_FILE:%s error\n" ,ini.log_dir, ini.bind_file);
		return -1;
	}

#if 0
	//msg svr
	tmp = ini_file.read ("ext", "MSG_NUM");
	if (tmp.empty ())
		ini.msg_num = 0;
	else
	{
		ini.msg_num = atoi (tmp.c_str());
		if (ini.msg_num <= 0 || ini.msg_num > MSG_NUM)
			ini.msg_num = 0;
	}
	if (ini.msg_num > 0)
	{
		//Binding request time
		tmp = ini_file.read ("ext", "BIND_TIMEOUT");
		if (tmp.empty ())
			ini.bind_timeout = 100;
		else
		{
			ini.bind_timeout = atoi (tmp.c_str());
			if (ini.bind_timeout <= 0 || ini.bind_timeout > 1000)
				ini.bind_timeout = 100;
		}
	
		char tt[32];
		for (int i =0; i< ini.msg_num; i++)
		{
			memset(tt, 0, 32);
			sprintf(tt, "MSG_%d_ID", i);
			tmp = ini_file.read ("ext", tt);
			if (tmp.empty ())
				return -1;
			else
				strncpy (ini.msg_svr[i].id, tmp.c_str(), 7);
			
			sprintf(tt, "MSG_%d_SVR", i);
			tmp = ini_file.read ("ext", tt);
			if (tmp.empty ())
				return -1;
			else
				strncpy (ini.msg_svr[i].svr, tmp.c_str(), 23);

			sprintf(tt, "MSG_%d_BIND_IP", i);
			tmp = ini_file.read ("ext", tt);
			if (tmp.empty ())
				return -1;
			else
				strncpy (ini.msg_svr[i].bindip, tmp.c_str(), 23);

			sprintf(tt, "MSG_%d_BIND_PORT", i);
			tmp = ini_file.read ("ext", tt);
			if (tmp.empty ())
				return -1;
			else
				strncpy (ini.msg_svr[i].bindport, tmp.c_str(), 11);
			
			//Binding table
			sprintf(tt, "MSG_%d_BIND_TABLE", i);
			tmp = ini_file.read ("ext", tt);
			if (tmp.empty ())
				return -1;
			strncpy (ini.msg_svr[i].bindtable, tmp.c_str(), 31);

			ini.msg_svr[i].id[7]=0;
			ini.msg_svr[i].svr[23]=0;
			ini.msg_svr[i].bindip[23]=0;
			ini.msg_svr[i].bindport[11]=0;
			ini.msg_svr[i].bindtable[31] = 0;

		}
	}
#endif
	return 0;
}

int	Option::parse_network(const char* config_file)
{
	IniFile net_file;
	if (!net_file.open (config_file))
	{
		printf ("open config file:%s failed\n", config_file);
		return -1;
	}

	string	section_str;
	section_str = net_file.read ("main", "BIND_COUNT");
	if (section_str.empty ())
		return -1;
	bind_count = atoi (section_str.c_str());

	if (bind_count <= 0 || bind_count > 16)
	{
		printf ("config file:%s BIND_COUNT error\n",section_str.c_str());
		return -1;
	}

	for (int i=0; i<bind_count; i++)
	{
		char sect_head[32];
		sprintf( sect_head, "port%d", i);

		section_str = net_file.read (sect_head, "IP");
		if (section_str.empty ())
		{
			printf ("config file:%s ip error\n", config_file);
			return -1;
		}
		else
			strncpy (bind_port[i].ip, section_str.c_str(), sizeof (bind_port[i].ip));

		section_str = net_file.read (sect_head, "PORT");
		if (section_str.empty ())
		{
			printf ("config file:%s port error\n", config_file);
			return -1;
		}
		else
			bind_port[i].port = atoi(section_str.c_str());

		section_str = net_file.read (sect_head, "TYPE");
		bind_port[i].type = (section_str == "UDP" ? 0:1);

		section_str = net_file.read (sect_head, "PROTOCOL");
		bind_port[i].protocol = (unsigned char) atoi(section_str.c_str());
	}

	return 0;
}

void Option::print ()
{
	printf ("LOG_DIR:%s\n", ini.log_dir);
	printf ("BIND_FILE:%s\n\n", ini.bind_file);

	printf ("SEND_SEM_KEY:%d\n", ini.send_sem_key);
	printf ("RECV_SEM_KEY:%d\n\n", ini.recv_sem_key);
	printf ("SHM_QUEUE_LENGTH:%d\n\n", ini.shm_queue_length);

	printf ("LOG_PRIORITY:%d\n", ini.log_priority);
	printf ("LOG_PRENAME:%s\n\n", ini.log_prename);

	printf ("SOCKET_TIMEOUT:%d\n", ini.socket_timeout);
	printf ("SOCKET_BUFSIZE:%d\n", ini.socket_bufsize);
	printf ("WORKER_PROC_NUM:%d\n\n", ini.worker_proc_num);

	for (int i=0; i<bind_count; i++)
	{
		printf ("IP:%s\n", bind_port[i].ip);
		printf ("PORT:%d\n", bind_port[i].port);
		printf ("TYPE:%d\n", bind_port[i].type);
	}
}
