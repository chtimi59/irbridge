#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define SERVER_DEFINE
#include "client.h"

int init_connection(char* host, int port)
{
	SOCKET sock;
	SOCKADDR_IN address;
		
	/* create socket */
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	address.sin_addr.s_addr = inet_addr(host);
	address.sin_port = htons(port);
	address.sin_family = AF_INET;
	
	if (connect(sock, (struct sockaddr *)&address, sizeof(address))!=0) {
		perror("connect()");
		closesocket(sock);
		return -1;
	} else {
		return sock;
	}
}

void end_connection(int sock)
{
	closesocket(sock);
}

int read_server(SOCKET sock, char *buffer)
{
	int n = 0;
	if ((n = recv(sock, buffer, BUF_SIZE, 0)) < 0)
	{
		perror("recv()");
		/* if recv error we disonnect the client */
		return 0;
	}
	return n;
}

void write_server(int socket, const char *buffer, size_t len)
{
	size_t ret = send(socket, buffer, len, 0);
	if (ret!=len) {
		perror("send()");
	}
}
