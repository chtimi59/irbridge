#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "iokey.h"
#include "lirc.h"
#include "ircodec.h"

#define BUF_SIZE 1024

#define DECODE   nec_decode
#define RESET    nec_decode_reset
#define ENCODE   nec_encode
#define BUFF     nec_DECODE_BUFF
#define BUFF_IDX nec_DECODE_BUF_IDX

int testlirc(int) ;

int main()
{	
	char buffer[BUF_SIZE];	
	int fd = open("/dev/lirc0", O_RDWR);
	int end=0;
	
	if (fd == -1) {
		fprintf(stderr, "error opening device\r\n");
		return 1;
	};

	/* used non blocking read */
	{
		int flags = fcntl(fd, F_GETFL, 0);
		fcntl(fd, F_SETFL, flags | O_NONBLOCK);
	}
	
	set_conio_terminal_mode();
	while (!end)
	{
		if (kbhit()) {
			int c = getch();
			switch (c)
			{
				default:
					fprintf(stderr, "unknown command '%c'\r\n",c);
				case 'h':					
					fprintf(stderr, "h - help\r\n");
					fprintf(stderr, "q - quit\r\n");
					fprintf(stderr, "1 - LIGHT ON'\r\n");
					fprintf(stderr, "2 - LIGHT OFF'\r\n");
					break;					
				case 0x3:
				case 0x4:
				case 'q':
					end = 1;
					break;
				
				case '1': {
					sample_t TEST[4] = { 0x0, 0xF7, 0xC0, 0x3F};
					char * bits = bytes2string(TEST, 32);
					lirc_t* ir = NULL;
					size_t  nir = 0;
					nir = ENCODE(bits, &ir);
					fprintf(stderr,"send ON -- %i : %s\r\n",nir,bits);
					write(fd, (void *)ir, nir);
					free(ir);
					free(bits);
					break;
				}
				case '2': {
					sample_t TEST[4] = { 0x0, 0xF7, 0x40, 0xBF};
					char * bits = bytes2string(TEST, 32);
					lirc_t* ir = NULL;
					size_t  nir = 0;
					nir = ENCODE(bits, &ir);
					fprintf(stderr,"send OFF -- %i : %s\r\n",nir,bits);
					write(fd, (void *)ir, nir);
					free(ir);
					free(bits);
					break;
				}
			}
		}
		
		
		{
			int c = read(fd, (void *)&buffer, BUF_SIZE);
			if (c>0)
			{
				size_t len = c;
				lirc_t* in = (lirc_t*)buffer;
				do {
					if (DECODE(&in, &len)) {
						unsigned int i;		
						sample_t* decoded = NULL;
						size_t n = string2bytes(BUFF, &decoded);
						fprintf(stderr,"%i:%s -> ", BUFF_IDX, BUFF);
						for (i = 0; i<(n / sizeof(sample_t)); i++)
							fprintf(stderr,"0x%X ", decoded[i]);							
						fprintf(stderr,"\r\n");
						free(decoded);
						RESET();
					}
				} while (len > 0);
			}
		}		
	}
	
	reset_terminal_mode();
	printf("quit\r\n");
	if (fd) close(fd);
	return 0;
}
