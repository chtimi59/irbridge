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
#define UNKNOW0


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

	#ifdef RAW
	fprintf(stderr, "Test RAW\r\n");
	#endif
	#ifdef NEC
	fprintf(stderr, "Test NEC\r\n");
	RESET();
	#endif
	#ifdef UNKNOW0
	fprintf(stderr, "Test UNKNOW0\r\n");
	RESET();
	#endif
	
	if (fd == -1) {
		fprintf(stderr, "error opening device\r\n");
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
					fprintf(stderr, "unknown command '%c'\r\n",c);
				case 'h':					
					fprintf(stderr, "h - help\r\n");
					fprintf(stderr, "q - quit\r\n");
					fprintf(stderr, "1 - send test 1'\r\n");
					fprintf(stderr, "2 - send test 2'\r\n");
					break;					
				case 0x3:
				case 0x4:
				case 'q':
					end = 1;
					break;
					
				#if defined(NEC)
				case '1': {
					sample_t TEST[4] = { 0x0, 0xF7, 0xC0, 0x3F};
					char * bits = bytes2string(TEST, 32);
					lirc_t* ir = NULL;
					size_t  nir = 0;
					nir = ENCODE(bits, &ir);
					fprintf(stderr,"send TEST1 -- %i : %s\r\n",nir,bits);
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
					fprintf(stderr,"send TEST2 -- %i : %s\r\n",nir,bits);
					write(fd, (void *)ir, nir);
					free(ir);
					free(bits);
					break;
				}
				#endif
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
					fprintf(stderr, "%s %u\r\n", (isPulse) ? "pulse" : "space", lenght);
				}
				#endif
			
				#if defined(NEC) || defined(UNKNOW0)
				size_t len = c;
				lirc_t* in = (lirc_t*)buffer;
				do {
					if (DECODE(&in, &len)) {
						unsigned int i;		
						sample_t* decoded = NULL;
						size_t n = string2bytes(BUFF, &decoded);
						printf("%i:%s -> ", BUFF_IDX, BUFF);
						for (i = 0; i<(n / sizeof(sample_t)); i++)
							fprintf(stderr,"0x%X ", decoded[i]);							
						fprintf(stderr,"\r\n");
						free(decoded);
						RESET();
					}
				} while (len > 0);
				#endif
			}
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
