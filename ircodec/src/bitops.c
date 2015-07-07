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

#include "bitops.h"
#include <string.h>

size_t string2bytes(char* in, sample_t** pOut)
{ 
	int nbbits = strlen(in);
	int nbBytes = (nbbits / 8) + ((nbbits % 8) ? 1 : 0);
	sample_t data = 0;
	int i, j = 0, b = 8;

	/* No bits ? */
	if (nbbits <= 0) {
		*pOut = NULL;
		return 0;
	}
	
	/* Allocation */
	*pOut = malloc(nbBytes);

	/* fullfill */
	for (i = 0; i < nbbits; i++)
	{
		b--;
		if (in[i] == '1') data += 1 << b;

		if (b == 0) {			
			(*pOut)[j++] = data;
			data = 0; b = 8;
		}
	}
	
	if (b != 8) (*pOut)[j] = data;
	return (size_t)nbBytes;
}

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
char* bytes2string(sample_t* in, int nbbits)
{
	char* out = NULL;
	int nbBytes = (nbbits / 8) + ((nbbits % 8) ? 1 : 0);
	int b, i;
	
	/* No bits ? */
	if (nbbits <= 0) return out;

	/* Allocation */
	out = malloc(nbbits +1);

	/* fullfill */
	for (i = 0; i < nbBytes; i++) {
		int base = 8 * i;
		for (b = 0; b < 8 && ((base + b) < nbbits); b++)
			out[base + b] = (in[i] & (1 << (7-b))) ? '1' : '0';
	}

	out[nbbits] = '\0';
	return out;
}


#ifdef UNITARY_TEST
#include <stdio.h>
#include <assert.h>
void UTEST_bitsops() {
	{
		sample_t* out = NULL;
		int n = 0;

		n = string2bytes("10001000", &out);
		assert(n == 1);
		assert(out[0] == 0x88);
		free(out);

		n = string2bytes("1", &out);
		assert(n == 1);
		assert(out[0] == 0x80);
		free(out);

		n = string2bytes("", &out);
		assert(out == NULL);
		assert(n == 0);

		n = string2bytes("100001001", &out);
		assert(n == 2);
		assert(out[0] == 0x84);
		assert(out[1] == 0x80);
		free(out);

		fprintf(stderr, "string2bytes() -- tests OK\n");
	}

	{
		sample_t in[2];
		char* out = NULL;

		out = bytes2string(in, 0);
		assert(out == NULL);

		in[0] = 0x01;
		out = bytes2string(in, 8);
		assert(strncmp(out, "00000001",8) == 0);
		free(out);

		in[0] = 0x01;
		out = bytes2string(in, 7);
		assert(strncmp(out, "0000000",7) == 0);
		free(out);

		in[0] = 0x84;
		in[1] = 0x8F;
		out = bytes2string(in, 9);
		assert(strncmp(out, "100001001",9) == 0);
		free(out);

		in[0] = 0x85;
		in[1] = 0x8F;
		out = bytes2string(in, 16);
		assert(strncmp(out, "1000010110001111", 16) == 0);
		free(out);

		fprintf(stderr, "bytes2string() -- tests OK\n");
	}
}
#endif