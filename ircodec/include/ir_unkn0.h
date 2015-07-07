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

#ifndef UNKNOW0CODEC
#define UNKNOW0CODEC

#include "lirc.h"
#include <stdlib.h>

/* UNKN0
carrier 38kHz?
pulse time T=500us

# bit '1'
Defined by a pulse of T follows by a space of 2xT

  <-T-> <- 2xT ->
  |----|
  |    |----|----|

# bit '0'
Defined by a pulse of T follows by a space of T


  <-T-> <-T->
  |----|
  |    |----|


Timming
worst case =  1.5ms/bit => 666 bits/s
*/


/* The following values are experiently found with a Raspberry Pie + librc */

/* Min Space needed to separate frames */
#define UNKN0_TS 10000
/* A threshold time between 3T and T, to disctinguish '1' and '0' */
#define UNKN0_TD 800
/* Time to define 0 */
#define UNKN0_T0  500
/* Time to define 1 */
#define UNKN0_T1  1000


/* API */

/* DECODE_BUFF */
#define unkn0_DECODE_BUFF_SIZE 1024 /* nb max of bits stored */
#ifdef UNKN0_DEFINE
char unkn0_DECODE_BUFF[unkn0_DECODE_BUFF_SIZE];
int  unkn0_DECODE_BUF_IDX = 0;
#else
extern char unkn0_DECODE_BUFF[unkn0_DECODE_BUFF_SIZE];
extern int  unkn0_DECODE_BUF_IDX;
#endif

/* reset UNKN0 decoder */
void unkn0_decode_reset();


/* DECODE a 'lirc_t' stream into 'unkn0_DECODE_BUF' char stream (ex: "10001100")
*  @pIn  : input lirc_t stream pointer*
*  @pLen : input lirc_t stream size (in bytes) pointer*
*  @return : 1 if a frame is available in unkn0_DECODE_BUF
*
*  (*) pointer will be updated on remaining part after decoder processing
*/
int unkn0_decode(lirc_t ** pIn, size_t * pLen);

/* ENCODE a input char stream (ex: "10001100") into a lirc_t stream
*  @in : a char input stream
*  @pOut : the ouput (malloc) lirc_t stream (need to be free)
*  @return : Number of allocated bytes  in pOut
*/
size_t unkn0_encode(char* in, lirc_t** pOut);

#ifdef UNITARY_TEST
void UTEST_unkn0();
#endif

#endif