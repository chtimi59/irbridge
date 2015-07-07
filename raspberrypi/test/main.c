#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "lirc.h"
#include "iokey.h"

#define BUF_SIZE 1024

int testlirc(int) ;


int main()
{	
	char buffer[BUF_SIZE];	
	int fd = open("/dev/lirc0", O_RDWR);
	int end=0;
	
	if (fd == -1) {
		fprintf(stderr, "error opening device\n");
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
					fprintf(stderr, "unknown command '%c'\n",c);
				case 'h':					
					fprintf(stderr, "h - help\n");
					fprintf(stderr, "s - send greeting to server'\n");
					fprintf(stderr, "q - quit\n");
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
			if (c>0) {
				int p;
				for (p=0; p<c; p=p+(int)(sizeof(lirc_t))) {
					lirc_t data = *(lirc_t*)&buffer[p];
					int isPulse = (data & PULSE_BIT);
					__u32 lenght = (__u32)(data & PULSE_MASK);
					fprintf(stderr, "%s %u\n", (isPulse) ? "pulse" : "space", lenght);
				}
			}
		}		
	}
	
	reset_terminal_mode();
	printf("quit\n");
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
		fprintf(stderr, "error device is not a character device\n");
		close(fd);
		return 1;
	}
	else if (ioctl(fd, LIRC_GET_REC_MODE, &mode) == -1) {
		fprintf(stderr, "error device do not supports rec mode\n");
		close(fd);
		return 1;
	}
	else if (mode != LIRC_MODE_MODE2) {
		fprintf(stderr, "error device 'mode2' type expected\n");
		close(fd);
		return 1;
	}

	{
		__u32 t;
		if (ioctl(fd, LIRC_GET_FEATURES, &t) == -1) {
			fprintf(stderr, "error\n");
		}
		else {
			if (t & LIRC_CAN_SEND_RAW) fprintf(stderr, "LIRC_CAN_SEND_RAW\n");
			if (t & LIRC_CAN_SEND_PULSE) fprintf(stderr, "LIRC_CAN_SEND_PULSE\n");
			if (t & LIRC_CAN_SEND_MODE2) fprintf(stderr, "LIRC_CAN_SEND_MODE2\n");
			if (t & LIRC_CAN_SEND_LIRCCODE) fprintf(stderr, "LIRC_CAN_SEND_LIRCCODE\n");
			if (t & LIRC_CAN_SET_SEND_CARRIER) fprintf(stderr, "LIRC_CAN_SET_SEND_CARRIER\n");
			if (t & LIRC_CAN_SET_SEND_DUTY_CYCLE) fprintf(stderr, "LIRC_CAN_SET_SEND_DUTY_CYCLE\n");
			if (t & LIRC_CAN_REC_RAW) fprintf(stderr, "LIRC_CAN_REC_RAW\n");
			if (t & LIRC_CAN_REC_PULSE) fprintf(stderr, "LIRC_CAN_REC_PULSE\n");
			if (t & LIRC_CAN_REC_MODE2) fprintf(stderr, "LIRC_CAN_REC_MODE2\n");
			if (t & LIRC_CAN_REC_LIRCCODE) fprintf(stderr, "LIRC_CAN_REC_LIRCCODE\n");
			fprintf(stderr, "%08X\n\r", t);
		}
	}
	
	return 0;
}
