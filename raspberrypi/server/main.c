#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <limits.h>
#include <syslog.h>
#include <fcntl.h>

#include "server.h"
#include "lirc.h"


/* http://stackoverflow.com/questions/17954432/creating-a-daemon-in-linux */


int main(int argc, char *argv[])
{
	
	SOCKET sock;
	int port;
	int fd;
	struct stat s;
	__u32 mode;
	char buffer[BUF_SIZE];	
	fd_set rdfs; /* synchronize FileDescriptors (file, socket, stdin...) */
	
	if (argc!=2) {
		fprintf(stderr,"usage: server <port>\n");
		exit(1);
	}
	
	/* connect to IR driver */
	fd = open("/dev/lirc0", O_RDWR);
	if (!fd) {
		fprintf(stderr,"no driver\n");
		exit(1);
	}

	/* some checks about IR driver (lirc) */
	if ((fstat(fd, &s) != -1) && (S_ISFIFO(s.st_mode))) {
		/* can't do ioctls on a pipe */
	}
	else if ((fstat(fd, &s) != -1) && (!S_ISCHR(s.st_mode))) {
		fprintf(stderr, "error IR device is not a character device\n");
		close(fd);
		exit(1);
	}
	else if (ioctl(fd, LIRC_GET_REC_MODE, &mode) == -1) {
		fprintf(stderr, "error IR device do not supports rec mode\n");
		close(fd);
		exit(1);
	}
	else if (mode != LIRC_MODE_MODE2) {
		fprintf(stderr, "error IR device 'mode2' type expected\n");
		close(fd);
		exit(1);
	}
	
	/* create server */
	port = atoi(argv[1]);	
	sock = init_connection(port);
	if (sock<=0) {
		fprintf(stderr,"couldn't create socket (port %d)\n", port);
		close(fd);
		exit(1);
	}
	
	/* main loop */
	while (1)
	{
		int idx;
		FD_ZERO(&rdfs);
		CLR_MAX_FD();
		
		/* Add STDIN_FILENO */
		FD_SET(STDIN_FILENO, &rdfs);
		ADD_MAX_FD(STDIN_FILENO);
		
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
			perror("select()");
			exit(errno);
		}

		/* 1- Something from STDIN ? */
		if (FD_ISSET(STDIN_FILENO, &rdfs))
		{			
			int end = 0; 

			/* only keep last character */
			char c,t;
			while ((read(0, &t, 1)>0) && t!='\n') c=t;			
			
			switch(c) {
				case 'q':
					end=1;
					break;
				case 's':
					fprintf(stderr, "send greeting!\n");
					write_toallclients("Hello world!\n", 13+1);
					break;
				default:
					fprintf(stderr, "unknown command '%c'\n",c);
				case 'h':					
					fprintf(stderr, "h - help\n");
					fprintf(stderr, "s - send greeting to all client'\n");
					fprintf(stderr, "q - quit\n");
					break;
			}
			if (end)
				break;
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

				/* do it 10 times ! */
				{ 
				  int y=0;
				  for(y=0;y<10;y++) {
					usleep(100*1000);
					if (c!=write(fd, (void *)buffer,c)) {
						perror("write()");
					}
				  }
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
	printf("proper end\n");
    return 0; 
}