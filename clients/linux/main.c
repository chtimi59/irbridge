#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "client.h"
#include "ircodec.h"

#if 0
#define DECODE   nec_decode
#define RESET    nec_decode_reset
#define ENCODE   nec_encode
#define BUFF     nec_DECODE_BUFF
#define BUFF_IDX nec_DECODE_BUF_IDX
#else
#define DECODE   unkn0_decode
#define RESET    unkn0_decode_reset
#define ENCODE   unkn0_encode
#define BUFF     unkn0_DECODE_BUFF
#define BUFF_IDX unkn0_DECODE_BUF_IDX
#endif
  
int main(int argc, char *argv[])
{
	SOCKET sock;
	int port = 0;
	char* host = "";
	char buffer[BUF_SIZE];
	
	fd_set rdfs; /* synchronize FileDescriptors (socket, stdin...) */
	
	if (argc!=3) {
		fprintf(stderr,"usage: client <ip> <port>\n");
		exit(1);
	}
	
	/* connect to server */
	host = argv[1];	
	port = atoi(argv[2]);	
	fprintf(stderr,"Connecting  to %s:%d...\n", host, port);	
	sock = init_connection(host, port);	
	if (sock<=0) {
		fprintf(stderr,"couldn't connect to %s:%d\n", host, port);
		exit(1);
	}
	fprintf(stderr,"connected!\n");
	
		
	/* main loop */
	while (1)
	{
		FD_ZERO(&rdfs);
		CLR_MAX_FD();

		/* Add STDIN_FILENO */
		FD_SET(STDIN_FILENO, &rdfs);
		ADD_MAX_FD(STDIN_FILENO);

		/* Add the connection socket */
		FD_SET(sock, &rdfs);
		ADD_MAX_FD(sock);

		/* blocking FD select */
		if (select(MAX_FD, &rdfs, NULL, NULL, NULL) == -1) {
			fprintf(stderr,"select()");
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
				default:
					fprintf(stderr, "unknown command '%c'\r\n",c);
				case 'h':					
					fprintf(stderr, "h - help\r\n");
					fprintf(stderr, "q - quit\r\n");
					fprintf(stderr, "1 - send light ON'\r\n");
					fprintf(stderr, "2 - send light OFF'\r\n");
					fprintf(stderr, "3 - send 88.3Kg'\r\n");
					break;
				case 'q':
					end=1;
					break;
					
					
				case '1':
				{
					sample_t TEST[4] = { 0x0, 0xF7, 0xC0, 0x3F};
					char * bits = bytes2string(TEST, 32);
					lirc_t* ir = NULL;
					size_t  nir = 0;
					nir = nec_encode(bits, &ir);
					fprintf(stderr,"send ON -- %i : %s\r\n",nir,bits);
					write_server(sock, (void *)ir, nir);
					free(ir);
					free(bits);
					break;
				}

				case '2':
				{
					sample_t TEST[4] = { 0x0, 0xF7, 0x40, 0xBF};
					char * bits = bytes2string(TEST, 32);
					lirc_t* ir = NULL;
					size_t  nir = 0;
					nir = nec_encode(bits, &ir);
					fprintf(stderr,"send OFF -- %i : %s\r\n",nir,bits);
					write_server(sock, (void *)ir, nir);
					free(ir);
					free(bits);
					break;
				}
				
				case '3':
				{
					sample_t TEST[5] = { 0x54, 0x7F, 0xFC, 0x8C, 0x5C };
					char * bits = bytes2string(TEST, 39);
					lirc_t* ir = NULL;
					size_t  nir = 0;
					nir = unkn0_encode(bits, &ir);
					fprintf(stderr,"send 88.3 -- %i : %s\r\n",nir,bits);
					write_server(sock, (void *)ir, nir);
					free(ir);
					free(bits);
					break;
				}
				
			}
			if (end)
				break;
		}
		
		/* 2- Something from connection socket ? */
		else if (FD_ISSET(sock, &rdfs))
		{
			size_t len;
			lirc_t*  in = NULL;
			
			len = read_server(sock, buffer);
			if (len == 0) {
				printf("server disconnected!\n");
				break;
			}
			
			in = (lirc_t*)buffer;
			do {
				if (DECODE(&in, &len))
				{
					sample_t* decoded = NULL;
					size_t n = string2bytes(BUFF, &decoded);
					if (BUFF_IDX==39)
					{
						/* invert */
						unsigned int i;
						for (i = 0; i<(n / sizeof(sample_t)); i++)
							decoded[i] = (~decoded[i]) & 0xFF;			
						if (decoded[0]==0xAB)
							fprintf(stderr,"%0.1f Kg", (float)(decoded[2]<<8 | decoded[3])/10);
					}
					free(decoded);
					RESET();
				}
			} while (len > 0);
		}
	}
	
	end_connection(sock);
	printf("proper end\n");	
    return 0; 
}