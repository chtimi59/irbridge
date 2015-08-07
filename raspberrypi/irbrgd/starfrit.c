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
#include <string.h>
#include <syslog.h>
#include <unistd.h>
#include <time.h>
#include "trim.h"

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

/* startfit configuration */
#define MAX_CONF 128
int starfritcount = 0;
struct {
	float min;
	float max;
	char* url;
} starfritconf[MAX_CONF];
int starfrit_led = 0;

void starfrit_setup(int led, char* args)
{
	char * p = NULL;
	int n=0;
	
	starfrit_led = led;
	if (args==NULL) return;
	p = strtok (args, ";");
	while(p)
	{ 	
		trim(p, NULL);
		switch(n%3) {
			case 0:
				starfritconf[starfritcount].min = atof(p);
				if (starfritconf[starfritcount].min<0) starfritconf[starfritcount].min=0;
				break;
			case 1:
				starfritconf[starfritcount].max = atof(p);
				if (starfritconf[starfritcount].max<0) starfritconf[starfritcount].max=0;				
				break;				
			case 2:
				if (p && strncmp(p,"http://", 7)==0) {
					starfritconf[starfritcount].url = p;
					starfritcount++;	
				}
				break;									
		}
		n++;
		p = strtok(NULL, ";");
	}
	if (starfritcount==0) {
		syslog(LOG_NOTICE,"starfrit: No url clients!");	
	} else {
		syslog(LOG_NOTICE,"starfrit: %d client(s)", starfritcount);	
	}
	for(n=0; n<starfritcount; n++) {
		syslog(LOG_NOTICE,"  [%d] min>=%0.2fKg max<%0.2fKg [%s]", n, starfritconf[n].min, starfritconf[n].max, starfritconf[n].url);
	}
}	

void push_measurement(float weight);
int starfrit_process(char* buffer, size_t len)
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
							
							/* push ! */
							if (samevalue_count==SAMEVALUE_CNT && weight!=0)
								push_measurement(weight);								
								
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

void push_measurement(float weight)
{
	int n=0;
	char path[255]={0};
	blink_led(starfrit_led);	
	
	/* for each clients */
	syslog(LOG_NOTICE,"startfrit pushed start");
	for(n=0; n<starfritcount; n++)
	{
		int test = 1;
		test = test && (starfritconf[n].min <= 0.0f || starfritconf[n].min >= weight);
		test = test && (starfritconf[n].max <= 0.0f || starfritconf[n].max <  weight);
		if (test) {
			sprintf(path,"curl -d \"\" -X POST \"%s%f\"", starfritconf[n].url, weight);
			syslog(LOG_NOTICE,"startfrit run '%s'", path);
			system(path);			
		}
	}	
	syslog(LOG_NOTICE,"startfrit pushed end");
}