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

/* TEST OPTIONS...
RAW : lirc raw output
NEC : NEC ir codec
UNKNOW0 : UNKNOW-0 ir propriatary codec
*/
#define NEC

#ifdef NEC
#define DECODE   nec_decode
#define RESET    nec_decode_reset
#define ENCODE   nec_encode
#define BUFF     nec_DECODE_BUFF
#define BUFF_IDX nec_DECODE_BUF_IDX
#endif

#ifdef UNKNOW0
#define DECODE   unkn0_decode
#define RESET    unkn0_decode_reset
#define ENCODE   unkn0_encode
#define BUFF     unkn0_DECODE_BUFF
#define BUFF_IDX unkn0_DECODE_BUF_IDX
#endif

int testlirc(int) ;

int main()
{	
	char buffer[BUF_SIZE];	
	int fd = open("/dev/lirc0", O_RDWR);
	int end=0;
	
	if (fd == -1) {
		fprintf(stderr, "error opening device\n\r");
		return 1;
	};

	/* used non blocking read */
	{
		int flags = fcntl(fd, F_GETFL, 0);
		fcntl(fd, F_SETFL, flags | O_NONBLOCK);
	}
	
	if (testlirc(fd))
		return 1;

	set_conio_terminal_mode();
	while (!end)
	{
		if (kbhit()) {
			int c = getch();
			switch (c)
			{
				default:
					fprintf(stderr, "unknown command '%c'\n\r",c);
				case 'h':					
					fprintf(stderr, "h - help\n\r");
					fprintf(stderr, "s - send greeting to server'\n\r");
					fprintf(stderr, "q - quit\n\r");
					break;					
				case 0x3:
				case 0x4:
				case 'q':
					end = 1;
					break;
			}
		}
		
		
		{
			int c = read(fd, (void *)&buffer, BUF_SIZE);
			if (c>0)
			{
			
				#ifdef RAW
				int p;
				for (p=0; p<c; p=p+(int)(sizeof(lirc_t))) {
					lirc_t data = *(lirc_t*)&buffer[p];
					int isPulse = (data & PULSE_BIT);
					__u32 lenght = (__u32)(data & PULSE_MASK);
					fprintf(stderr, "%s %u\n\r", (isPulse) ? "pulse" : "space", lenght);
				}
				#endif
			
				#if defined(NEC) || defined(UNKNOW0)
				size_t len = c;
				lirc_t* in = (lirc_t*)buffer;
				do {
					if (DECODE(&in, &len)) {
						sample_t* decoded = NULL;
						size_t n = string2bytes(BUFF, &decoded);
						printf("%i : %s\n", BUFF_IDX, BUFF);
						{
							unsigned int i;						
							for (i = 0; i<(n / sizeof(sample_t)); i++)
								printf("0x%X ", decoded[i]);
							printf("\n\r");
						}
						free(decoded);
						RESET();
					}
				} while (len > 0);
				#endif
			}
		}		
	}
	
	reset_terminal_mode();
	printf("quit\n\r");
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
		fprintf(stderr, "error device is not a character device\n\r");
		close(fd);
		return 1;
	}
	else if (ioctl(fd, LIRC_GET_REC_MODE, &mode) == -1) {
		fprintf(stderr, "error device do not supports rec mode\n\r");
		close(fd);
		return 1;
	}
	else if (mode != LIRC_MODE_MODE2) {
		fprintf(stderr, "error device 'mode2' type expected\n\r");
		close(fd);
		return 1;
	}

	{
		__u32 t;
		if (ioctl(fd, LIRC_GET_FEATURES, &t) == -1) {
			fprintf(stderr, "error\n\r");
		}
		else {
			if (t & LIRC_CAN_SEND_RAW) fprintf(stderr, "LIRC_CAN_SEND_RAW\n\r");
			if (t & LIRC_CAN_SEND_PULSE) fprintf(stderr, "LIRC_CAN_SEND_PULSE\n\r");
			if (t & LIRC_CAN_SEND_MODE2) fprintf(stderr, "LIRC_CAN_SEND_MODE2\n\r");
			if (t & LIRC_CAN_SEND_LIRCCODE) fprintf(stderr, "LIRC_CAN_SEND_LIRCCODE\n\r");
			if (t & LIRC_CAN_SET_SEND_CARRIER) fprintf(stderr, "LIRC_CAN_SET_SEND_CARRIER\n\r");
			if (t & LIRC_CAN_SET_SEND_DUTY_CYCLE) fprintf(stderr, "LIRC_CAN_SET_SEND_DUTY_CYCLE\n\r");
			if (t & LIRC_CAN_REC_RAW) fprintf(stderr, "LIRC_CAN_REC_RAW\n\r");
			if (t & LIRC_CAN_REC_PULSE) fprintf(stderr, "LIRC_CAN_REC_PULSE\n\r");
			if (t & LIRC_CAN_REC_MODE2) fprintf(stderr, "LIRC_CAN_REC_MODE2\n\r");
			if (t & LIRC_CAN_REC_LIRCCODE) fprintf(stderr, "LIRC_CAN_REC_LIRCCODE\n\r");
			fprintf(stderr, "%08X\n\r", t);
		}
	}
	
	return 0;
}
