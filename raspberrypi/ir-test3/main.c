/*
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

Version
2015/07/02

Jan d'Orgeville
jan_dorgeville@hotmail.com

*/


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
					fprintf(stderr, "1 - send test 1'\r\n");
					break;					
				case 0x3:
				case 0x4:
				case 'q':
					end = 1;
					break;
				
				case '1': {
					sample_t TEST[5] = { 0x54, 0x7F, 0xFC, 0x87, 0x58 };
					char * bits = bytes2string(TEST, 39);
					lirc_t* ir = NULL;
					size_t  nir = 0;
					nir = ENCODE(bits, &ir);
					fprintf(stderr,"send 88.8 -- %i : %s\r\n",nir,bits);
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
						/* for (i = 0; i<(n / sizeof(sample_t)); i++)
							fprintf(stderr,"0x%X ", decoded[i]); */
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
