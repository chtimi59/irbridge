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

#ifndef BITOPS
#define BITOPS

#if defined(WIN32)
#include <windows.h>
#define sample_t  BYTE
#else
#include <stdlib.h>
#include <linux/types.h>
#define sample_t  __u8
#endif

/* Convert an input char stream into a 'sample_t' array
*  example:
*       sample_t* out = NULL;
*       n = string2bytes("10001000", &out);  => n=1 ; out[0] = 0x88
*       n = string2bytes("1", &out);         => n=1 ; out[0] = 0x80
*       n = string2bytes("100001001", &out); => n=2 ; out[0] = 0x84 out[1] = 0x80
*
*  @in : a char input stream
*  @pOut : the ouput 'sample_t' 8-bits array
*  @return : Number of allocated bytes  in pOut
*
*/
size_t string2bytes(char* in, sample_t** pOut);

/* Convert a 'sample_t' array into a char stream
*  example:
*       sample_t* in = { 0x42,  0x8F };
*       char* out = NULL;
*       out = bytes2string(in, 8) => out = "100001001"
*
*  @in : a 'sample_t' array
*  @nbbits : nb of bits in input array
*  @return : an allocated  char stream
*/
char* bytes2string(sample_t* in, int nbbits);

#ifdef UNITARY_TEST
void UTEST_bitsops();
#endif

#endif
