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

/* 
*  starfrit bathroom scale
*/

#include <stdlib.h>
#include <stdio.h>
#include <syslog.h>
#include <unistd.h>
#include <time.h>

#include "led.h"
#include "starfrit.h"
#include "ircodec.h"

#define DECODE   unkn0_decode
#define RESET    unkn0_decode_reset
#define ENCODE   unkn0_encode
#define BUFF     unkn0_DECODE_BUFF
#define BUFF_IDX unkn0_DECODE_BUF_IDX

#define SAMEVALUE_CNT 10
float last_measurement = -1;
int   samevalue_count = 0;
int   last_measurement_date = 0;

void reset_measurements() {
	last_measurement = -1;
	samevalue_count = 0;
}

int starfrit_process(char* url, int led, char* buffer, size_t len)
{
	lirc_t* in = (lirc_t*)buffer;

	do {
		if (DECODE(&in, &len)) {
			sample_t* decoded = NULL;
			size_t n = string2bytes(BUFF, &decoded);		
			if(n==5) {
				if (decoded[0]==0x54)
				{		
					int trail  = (~decoded[0]) & 0xFF;
					int head   = (~decoded[1]) & 0xFF;
					int msb    = (~decoded[2]) & 0xFF;
					int lsb    = (~decoded[3]) & 0xFF;
					int check  = (~decoded[4]) & 0xFF;
						
					float weight = (float)((msb<<8) | lsb)/10;
						
					int sum = trail+head+msb+lsb;
					int sumFF = (sum & 0xFF) | 0x1;
					int remain = (sum - sumFF);
					if (remain>=256) sumFF += 2;			
					
					if (sumFF==check)
					{	
						/* 15s time out? reset measurements */
						int t = (int)time(NULL);
						if ((t-last_measurement_date)>(15)) reset_measurements();
						last_measurement_date = t;
						
						/* already know ? */
						if (last_measurement==weight) {
							samevalue_count++;
							
							/* log */
							if (samevalue_count<SAMEVALUE_CNT) {
								syslog(LOG_NOTICE,"startfrit %0.2fKg %i/%i %is", weight,
											samevalue_count, SAMEVALUE_CNT,
											(t-last_measurement_date));
							}
							
							if (samevalue_count==SAMEVALUE_CNT) {
								char path[255]={0};
								sprintf(path,"curl -X POST \"%s%f\"", url, weight);
								syslog(LOG_NOTICE,"startfrit run '%s'", path);
								system(path);
								blink_led(led);
							}
						} else {
							reset_measurements();
							last_measurement=weight;
						}						
					}
				}
			}
			free(decoded);
			RESET();
		}
	} while (len > 0);
	
	return 0;
}
