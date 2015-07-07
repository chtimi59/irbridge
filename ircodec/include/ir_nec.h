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

#ifndef NECCODEC
#define NECCODEC

#include "lirc.h"
#include <stdlib.h>

/* NEC
carrier 38kHz
pulse time T=560us

# bit '1'
Defined by a pulse of T follows by a space of 3xT

  <-T-> <---- 3xT ---->
  |----|
  |    |----|----|----|

# bit '0'
Defined by a pulse of T follows by a space of T


  <-T-> <-T->
  |----|
  |    |----|


Timming
worst case =  2.24ms/bit => 500 bits/s
*/


/* The following values are experiently found with a Raspberry Pie + librc */

/* Min Space needed to separate frames */
#define NEC_TS 2500
/* A threshold time between 3T and T, to disctinguish '1' and '0' */
#define NEC_TD (560 + (1680-560)/2)
#define NEC_THEAD_PULSE  9000
#define NEC_THEAD_SPACE  (NEC_THEAD_PULSE/2)
/* Time to define 0 */
#define NEC_T0  560
/* Time to define 1 */
#define NEC_T1  1680


/* API */

#define nec_DECODE_BUFF_SIZE 1024 /* nb max of bits stored */
#ifdef DEFINE
char nec_DECODE_BUFF[nec_DECODE_BUFF_SIZE];
int  nec_DECODE_BUF_IDX = 0;
#else
extern char nec_DECODE_BUFF[nec_DECODE_BUFF_SIZE];
extern int  nec_DECODE_BUF_IDX;
#endif

/* reset NEC decoder */
void nec_decode_reset();


/* DECODE a 'lirc_t' stream into 'nec_DECODE_BUF' char stream (ex: "10001100")
*  @pIn  : input lirc_t stream pointer*
*  @pLen : input lirc_t stream size (in bytes) pointer*
*  @return : 1 if a frame is available in nec_DECODE_BUF
*
*  (*) pointer will be updated on remaining part after decoder processing
*/
int nec_decode(lirc_t ** pIn, size_t * pLen);

/* ENCODE a input char stream (ex: "10001100") into a lirc_t stream
*  @in : a char input stream
*  @pOut : the ouput (malloc) lirc_t stream (need to be free)
*  @return : Number of allocated bytes  in pOut
*/
size_t nec_encode(char* in, lirc_t** pOut);

#ifdef UNITARY_TEST
void UTEST_nec();
#endif

#endif