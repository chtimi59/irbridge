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

#define DECODE   unkn0_decode
#define RESET    unkn0_decode_reset
#define ENCODE   unkn0_encode
#define BUFF     unkn0_DECODE_BUFF
#define BUFF_IDX unkn0_DECODE_BUF_IDX

int main()
{	
	char buffer[BUF_SIZE];	
	int fd = open("/dev/lirc0", O_RDWR);

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
	RESET();
	while (1)
	{
		int c = read(fd, (void *)&buffer, BUF_SIZE);
		if (kbhit())
			break;;

		if (c>0)
		{
			size_t len = c;
			lirc_t* in = (lirc_t*)buffer;
			do {
				if (DECODE(&in, &len)) {
					unsigned int i;		
					sample_t* decoded = NULL;
					size_t n = string2bytes(BUFF, &decoded);
					if (BUFF_IDX==39)
					{
						/* invert */
						for (i = 0; i<(n / sizeof(sample_t)); i++)
							decoded[i] = (~decoded[i]) & 0xFF;			
						
						#if 0
						/* show */
						for (i = 0; i<(n / sizeof(sample_t)); i++)
							fprintf(stderr,"0x%X ", decoded[i]);							
						fprintf(stderr,"\r\n");
						#endif
						
						#if 0
						if (decoded[0]==0xAB) {
							fprintf(stderr,"0x%X ", decoded[1]);							
							fprintf(stderr,"%0.1f ", (float)(decoded[2]<<8 | decoded[3])/10);							
							fprintf(stderr,"%i %i", (decoded[0]+decoded[1]+decoded[2]+decoded[3]) & 0xFF, decoded[4]);
							fprintf(stderr,"\r\n");
						}
						#endif
					}
					free(decoded);
					RESET();
				}
			} while (len > 0);
		}		
	}
	
	reset_terminal_mode();
	printf("quit\r\n");
	if (fd) close(fd);
	return 0;
}


int testlirc(int fd) 
{
	struct stat s;
	__u32 mode;
	
	if ((fstat(fd, &s) != -1) && (S_ISFIFO(s.st_mode))) {
		/* can't do ioctls on a pipe */
	}
	else if ((fstat(fd, &s) != -1) && (!S_ISCHR(s.st_mode))) {
		fprintf(stderr, "error device is not a character device\r\n");
		close(fd);
		return 1;
	}
	else if (ioctl(fd, LIRC_GET_REC_MODE, &mode) == -1) {
		fprintf(stderr, "error device do not supports rec mode\r\n");
		close(fd);
		return 1;
	}
	else if (mode != LIRC_MODE_MODE2) {
		fprintf(stderr, "error device 'mode2' type expected\r\n");
		close(fd);
		return 1;
	}

	{
		__u32 t;
		if (ioctl(fd, LIRC_GET_FEATURES, &t) == -1) {
			fprintf(stderr, "error\r\n");
		}
		else {
			if (t & LIRC_CAN_SEND_RAW) fprintf(stderr, "LIRC_CAN_SEND_RAW\r\n");
			if (t & LIRC_CAN_SEND_PULSE) fprintf(stderr, "LIRC_CAN_SEND_PULSE\r\n");
			if (t & LIRC_CAN_SEND_MODE2) fprintf(stderr, "LIRC_CAN_SEND_MODE2\r\n");
			if (t & LIRC_CAN_SEND_LIRCCODE) fprintf(stderr, "LIRC_CAN_SEND_LIRCCODE\r\n");
			if (t & LIRC_CAN_SET_SEND_CARRIER) fprintf(stderr, "LIRC_CAN_SET_SEND_CARRIER\r\n");
			if (t & LIRC_CAN_SET_SEND_DUTY_CYCLE) fprintf(stderr, "LIRC_CAN_SET_SEND_DUTY_CYCLE\r\n");
			if (t & LIRC_CAN_REC_RAW) fprintf(stderr, "LIRC_CAN_REC_RAW\r\n");
			if (t & LIRC_CAN_REC_PULSE) fprintf(stderr, "LIRC_CAN_REC_PULSE\r\n");
			if (t & LIRC_CAN_REC_MODE2) fprintf(stderr, "LIRC_CAN_REC_MODE2\r\n");
			if (t & LIRC_CAN_REC_LIRCCODE) fprintf(stderr, "LIRC_CAN_REC_LIRCCODE\r\n");
			fprintf(stderr, "%08X\r\n", t);
		}
	}
	
	return 0;
}
