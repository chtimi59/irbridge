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
#ifndef LED_H
#define LED_H

int register_led(int gpio);
void unregister_led(int gpio);
void set_led(int gpio, int v);
void blink_led(int gpio);

#endif