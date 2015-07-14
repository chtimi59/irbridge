#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <syslog.h>

#define RUNNING_DIR	"/etc/irbrgd"
#define LOCK_FILE	"irbrgd.lock"
#define LOG_FILE	"irbrgd.log"
#define CONF_FILE	"irbrgd.conf"

#include "server.h"
#include "lirc.h"

void usage() {
	fprintf(stderr, "usage: irbdg start|stop\n");
	exit(1);
}

struct {
	int  port;
	int  starfrit;
	char starfrit_url[256];
} conf = {
	8000,
	0,
	{0}
};

int trim(char*s);
void readconf() {
	FILE* file =fopen(CONF_FILE, "r");
	char line[256];
	if (file==NULL) {
		fprintf(stderr, "warning: "CONF_FILE" not found (default values used)\n");
		return;
	}
	while (fgets(line, sizeof(line), file))
	{		
		char * key=NULL;
		char * p = strtok (line, " \t\n\r");
		while(p)
		{ 	
			/* tocken travels */
			int end=0;
			if (trim(p)) do
			{
				if (strcmp(p,"#")==0) { end=1; key = NULL; break; }
				if (!key) { key = p; break; }
				do {
					if (strcmp(key,"port")==0) { 
						conf.port=atoi(p);
						break;
					}
					if (strcmp(key,"starfrit")==0) { 
						conf.starfrit = (strcmp(p,"ON")==0);
						break;
					}
					if (strcmp(key,"starfrit_url")==0) { 
						strcpy(conf.starfrit_url, p);
						break;
					}
				} while(0);
				end=1; key = NULL;
			} while(0);
			
			p = end?NULL:strtok (NULL, " ");
		}		
	}
	fclose(file);
}

void signal_handler(int sig) {
	switch(sig) {
		case SIGHUP:
			syslog(LOG_NOTICE,"hangup signal catched");
			break;
		case SIGTERM:
			syslog(LOG_NOTICE,"terminate signal catched");
			exit(0);
			break;
	}
}

void daemonize()
{
	int i,lfp;
	char str[10];
	if(getppid()==1) return; /* already a daemon */
	
	/* fork! */
	i=fork();	
	if (i<0) {
		syslog(LOG_ERR,"error: fork error");
		exit(1);
	}	
	if (i>0) {
		syslog(LOG_ERR,"error: parent exits");
		exit(0);
	}
	
	/* child (daemon) continues */
	setsid(); /* obtain a new process group */

	/* close all descriptors */
	for (i=getdtablesize();i>=0;--i) close(i);

	/* handle standart I/O */
	i=open("/dev/null",O_RDWR); dup(i); dup(i); 

	/* set newly created file permissions */
	umask(027);

	/* create lock file */
	lfp=open(LOCK_FILE,O_RDWR|O_CREAT,0640);
	if (lfp<0) {
		syslog(LOG_ERR,"error: can't open lock file "LOCK_FILE);
		exit(1);
	}
	if (lockf(lfp,F_TLOCK,0)<0) {
		syslog(LOG_ERR,"error: can not lock lock file "LOCK_FILE);
		exit(0);
	}
	
	/* record pid to lockfile */
	sprintf(str,"%d\n",getpid());
	syslog(LOG_NOTICE,"daemon started on PID: %d",getpid());
	write(lfp,str,strlen(str)); 
	
	/* catch signals */
	signal(SIGCHLD,SIG_IGN); /* ignore child */
	signal(SIGTSTP,SIG_IGN); /* ignore tty signals */
	signal(SIGTTOU,SIG_IGN); /* ignore stdin */
	signal(SIGTTIN,SIG_IGN); /* ignore stdout */
	signal(SIGHUP,signal_handler); /* catch hangup signal */
	signal(SIGTERM,signal_handler); /* catch kill signal */
}

int getPID() {
	int i;
	struct flock fl = { 0,0,0,0,0 };
	int lfp=open(LOCK_FILE,O_RDONLY);
	if (lfp<0) return -1;
	i=fcntl(lfp, F_GETLK, &fl);
	close(lfp);
	
	if (i<0) return -1;
	if (fl.l_type == F_UNLCK) return -1;
	return fl.l_pid;
}

int trim(char*s) {
	int i,len;
	if (!s) return 0;
	len=strlen(s);
	for(i=0;i<len;i++) {
		int b = 0;
		b = b || (s[i]==' ');
		b = b || (s[i]=='\t');
		b = b || (s[i]=='\n');
		b = b || (s[i]=='\r');
		if(b) s[i]='\0';
	}
	return strlen(s);
}

int main_demonized();
int main(int argc, char**argv)
{
	int ret=0;
	
	/* setpup syslog */
	setlogmask (LOG_UPTO (LOG_NOTICE));
	
	/* change running directory */
	chdir(RUNNING_DIR); 
	
	/* parse argument */
	if (argc!=2) usage();
	do {
		if (strcmp(argv[1],"start")==0) {
			int pid = getPID();
			openlog ("irbrgd", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
			
			if (pid>=1) {
				syslog (LOG_ERR, "irbrgd daemon [%d] is already running\n",pid);
				fprintf(stderr, "irbrgd daemon [%d] is already running\n",pid);
				closelog();
				exit(1);
			}
			
			readconf();
			syslog(LOG_NOTICE,"port %d", conf.port);
			syslog(LOG_NOTICE,"starfrit %d", conf.starfrit);
			syslog(LOG_NOTICE,"starfrit_url %s", conf.starfrit_url);
			daemonize();
			break;
		}
		if (strcmp(argv[1],"stop")==0) {
			int ret;
			int pid = getPID();
			if (pid<1) {
				fprintf(stderr, "nothing to stop\n");
				exit(1);
			}
			fprintf(stderr, "stopping [%d]\n", pid);
			ret = kill(pid,SIGHUP);
			exit(ret);
		}
		usage();
		exit(0);
	} while(0);

	ret=main_demonized();
	closelog();
	return ret;
}





int main_demonized()
{
	
	SOCKET sock;
	int fd;
	struct stat s;
	__u32 mode;
	char buffer[BUF_SIZE];	
	fd_set rdfs; /* synchronize FileDescriptors (file, socket, stdin...) */

	/* connect to IR driver */
	fd = open("/dev/lirc0", O_RDWR);
	if (!fd) {
		syslog(LOG_ERR,"no driver");
		return 1;
	}

	/* some checks about IR driver (lirc) */
	if ((fstat(fd, &s) != -1) && (S_ISFIFO(s.st_mode))) {
		/* can't do ioctls on a pipe */
	}
	else if ((fstat(fd, &s) != -1) && (!S_ISCHR(s.st_mode))) {
		syslog(LOG_ERR,"error IR device is not a character device");
		close(fd);
		return 1;
	}
	else if (ioctl(fd, LIRC_GET_REC_MODE, &mode) == -1) {
		syslog(LOG_ERR,"error IR device do not supports rec mode");
		close(fd);
		return 1;
	}
	else if (mode != LIRC_MODE_MODE2) {
		syslog(LOG_ERR,"error IR device 'mode2' type expected");
		close(fd);
		return 1;
	}
	
	/* create server */
	sock = init_connection(conf.port);
	if (sock<=0) {
		syslog(LOG_ERR,"couldn't create socket (port %d)", conf.port);
		close(fd);
		return 1;
	}
	
	/* main loop */
	while (1)
	{
		int idx;
		FD_ZERO(&rdfs);
		CLR_MAX_FD();
		
		/* Add LIRC */
		FD_SET(fd, &rdfs);
		ADD_MAX_FD(fd);		
		
		/* Add the connection socket */
		FD_SET(sock, &rdfs);
		ADD_MAX_FD(sock);
		
		/* Add sockets of each client */
		for (idx = 0; idx < CLIENTS_COUNT; idx++) {
			SOCKET csocket = client(idx);
			if (csocket>0) {
				FD_SET(csocket, &rdfs);
				ADD_MAX_FD(csocket);
			}
		}
		
		/* blocking FD select */
		if (select(MAX_FD, &rdfs, NULL, NULL, NULL) == -1) {
			if (errno == EINTR) {
				break;
			} else {
				syslog(LOG_ERR,"select() %s", strerror(errno));
				return errno;
			}
		}

		/* 2- Something from IR ? */
		else if (FD_ISSET(fd, &rdfs))
		{
			int c = read(fd, (void *)&buffer, BUF_SIZE);
			if (c>0) write_toallclients(buffer, c);
		}		
		/* 2- Something from connection socket ? */
		else if (FD_ISSET(sock, &rdfs))
		{
			/* new client */
			add_client(sock);
		}
		/* 4- else Something from client sockets... */
		else
		{
			int idx = 0;
			for (idx = 0; idx < CLIENTS_COUNT; idx++)
			{
				SOCKET csocket = client(idx);
				int c;
				if (csocket<0) continue;
				if (!FD_ISSET(client(idx), &rdfs)) continue;
				
				c = read_client(csocket, buffer);
				if (c == 0) {
					/* client disconnected */
					remove_client(csocket);
					continue;
				}

				if (c!=write(fd, (void *)buffer,c)) {
					syslog(LOG_ERR,"write() %s", strerror(errno));
				}
				
				/* flush read (to avoid loopback) */
				{
					int t, flags = fcntl(fd, F_GETFL, 0);
					usleep(100*1000);
					fcntl(fd, F_SETFL, flags | O_NONBLOCK);
					while (read(fd, &t, 1)>0);
					fcntl(fd, F_SETFL, flags);
				}
				break;
			}
		}
	}

	close(fd);
	clear_clients();
	end_connection(sock);
	syslog(LOG_NOTICE,"proper end");
    return 0; 
}

/* EOF */