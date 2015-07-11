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
		int c;
		
		if (kbhit()) {
			end=1;
		}
		
		c = read(fd, (void *)&buffer, BUF_SIZE);
		if (c>0)
		{
			/* NEC - DECODE */
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
	reset_terminal_mode();

	if (fd) close(fd);
	return 0;
}