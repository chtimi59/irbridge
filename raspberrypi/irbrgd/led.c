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
*  Raspberry PI Model2 GPIO Settings
*  based on /sys/class/gpio driver
*/

#include <stdlib.h>
#include <stdio.h>
#include <syslog.h>
#include <unistd.h>

#include "led.h"


/* echo 4 > /sys/class/gpio/export
   echo out > /sys/class/gpio/gpio4/direction */
int register_led(int gpio)
{	
	FILE* file = NULL;
	char path[255] = {0};
	if (gpio==0) return 1; /* no gpio */
	syslog(LOG_ERR,"register_led %d",gpio);
	
	sprintf(path,"/sys/class/gpio/export");
	file=fopen(path, "w");
	if (NULL==file) {
		syslog(LOG_ERR,"no gpio driver");
		return 1;
	}
	if (fprintf(file,"%d", gpio)<0) {
		syslog(LOG_ERR,"invalid gpio%d", gpio);
		fclose(file);
		return 1;
	}
	fclose(file);
	
	sprintf(path,"/sys/class/gpio/gpio%d/direction", gpio);
	syslog(LOG_ERR,path);
	file=fopen(path, "w");
	if (NULL==file) {
		syslog(LOG_ERR,"invalid gpio driver");
		unregister_led(gpio);
		return 1;
	}
	if (fprintf(file,"out")<0) {
		syslog(LOG_ERR,"couldn't set gpio%d direction", gpio);
		fclose(file);
		unregister_led(gpio);
		return 1;
	}
	fclose(file);
	return 0;
}

/* echo 4 > /sys/class/gpio/unexport */
void unregister_led(int gpio) {
	FILE* file = NULL;
	char path[255] = {0};
	if (gpio==0) return; /* no gpio */
	syslog(LOG_ERR,"unregister_led %d",gpio);
	
	sprintf(path,"/sys/class/gpio/unexport");	
	file=fopen(path, "w");
	if (NULL==file) {
		syslog(LOG_ERR,"no gpio driver");
		return;
	}
	if (fprintf(file,"%d", gpio)<0) {
		syslog(LOG_ERR,"invalid gpio%d", gpio);
		fclose(file);
		return;
	}
	fclose(file);
}

/* echo 1 > /sys/class/gpio/gpio4/value */
void set_led(int gpio, int v) {
	FILE* file = NULL;
	char path[255] = {0};
	if (gpio==0) return; /* no gpio */
	
	sprintf(path,"/sys/class/gpio/gpio%d/value", gpio);
	file=fopen(path, "w");
	if (NULL==file) return;
	fprintf(file,"%d", v);
	fclose(file);
}

void blink_led(int gpio) {
	set_led(gpio,1);
	sleep(1);
	set_led(gpio,0);
	sleep(1);
}