#ifndef SERVER_H
#define SERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> /* close */
#include <netdb.h> /* gethostbyname */
#include <sys/select.h>
#include <errno.h>
#include <fcntl.h>
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;
typedef struct timeval TIMEVAL;

#define BUF_SIZE	 1024

int init_connection(char*,int);
void end_connection(int);

/* FD methods */
#ifdef SERVER_DEFINE
int _select_max_fd;
#else
extern int _select_max_fd;
#endif
#define MAX_FD (_select_max_fd+1)
#define ADD_MAX_FD(_fd_) { _select_max_fd = ((_select_max_fd)>(_fd_))?(_select_max_fd):(_fd_); } while(0)
#define CLR_MAX_FD() { _select_max_fd = 0; } while(0)


/* Server IO */
int read_server(SOCKET, char *);
void write_server(int , const char *, size_t);

#endif