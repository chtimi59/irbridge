#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define SERVER_DEFINE
#include "server.h"

int init_connection(int port)
{
	SOCKET sock;
	SOCKADDR_IN sin;
	memset(&sin, 0, sizeof(sin));

	if ((port<1)||(port>65000)) {
		fprintf(stderr,"invalid port\n");
		return -1;
	}
	
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		perror("connect()");
		return errno;
	}

	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_port = htons(port);
	sin.sin_family = AF_INET;

	if (bind(sock, (SOCKADDR *)&sin, sizeof sin) == SOCKET_ERROR)
	{
		perror("connect()");
		closesocket(sock);
		return errno;
	}

	if (listen(sock, MAX_CLIENTS) == SOCKET_ERROR)
	{
		perror("connect()");
		closesocket(sock);
		return errno;
	}
	
	fprintf(stderr,"socket created on port %d\n",port);
	return sock;
}

void end_connection(int sock)
{
	closesocket(sock);
}

void add_client(int sock)
{
	int idx;
	char *ip;
	SOCKADDR_IN csin;
	size_t sinsize = sizeof csin;	
	int csock;
	
	memset(&csin, 0, sinsize);
	
	csock = accept(sock, (SOCKADDR *)&csin, &sinsize);
	if (csock == SOCKET_ERROR) {
		perror("connect()");
		return;
	}	
	ip = inet_ntoa(csin.sin_addr);
	idx = _clients_count;
	
	_clients_count++;	
	_clients[idx] = csock;	
	
	printf("new client[%d]: %s\n", idx, ip);	
	
	debug_clients_display();
}

void remove_client(int csock)
{
	int idx=0;
	int moveup=0;
	closesocket(csock);
	
	printf("remove client\n");	
	
	for (idx=0; idx<_clients_count ;idx++) {
		if (_clients[idx]==csock)
			moveup = 1;
		if(moveup && ((idx+1)<_clients_count))
			_clients[idx]=_clients[idx+1];
	}	
	_clients_count--;	
	if (_clients_count<0) _clients_count=0;
	
	debug_clients_display();
}
		
void debug_clients_display() {
	int idx;
	for (idx=0;idx<_clients_count;idx++) {
		printf("%d : %d\n", idx, _clients[idx]);	
	}	
}	

SOCKET client(int idx) {
	if (idx>=_clients_count) return -1;
	return _clients[idx];
}

void clear_clients() {
	int idx;
	for (idx=0;idx<_clients_count;idx++) {
		closesocket(_clients[idx]);	
	}	
}

int read_client(SOCKET sock, char *buffer)
{
	int n = 0;
	if ((n = recv(sock, buffer, BUF_SIZE, 0)) < 0)
	{
		fprintf(stderr,"recv()\n");
		/* if recv error we disonnect the client */
		return 0;
	}
	return n;
}

void write_toallclients(const char *buffer, size_t len)
{
	int idx;
	for (idx=0;idx<_clients_count;idx++) {
		SOCKET csock = client(idx);
		size_t ret = send(csock, buffer, len, 0);
		if (ret!=len) {
			perror("send()");
		}
	}	
}