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

#include <stdio.h>     /* for printf() */
#include <stdlib.h>    /* for exit() */
#include "ircodec.h"

#ifdef UNITARY_TEST

void nec()
{
	sample_t test[2] = { 0xAD, 0x13 };
	char*    bits;
	lirc_t*  ir;
	lirc_t*  ir2;
	size_t   nir;
	int      cir;

	/* 1 - create a bit string */
	bits = bytes2string(test, 15);
	printf("%s\n",bits);

	/* 2 - encode it into nec */
	nir = nec_encode(bits, &ir);
	free(bits);

	/* 3 - simulate this nec ir code reception */
	cir = nir / sizeof(lirc_t);
	ir2 = malloc((cir+1)*sizeof(lirc_t));
	for (int i = 0; i < cir; i++) {
		ir2[i] = ir[i];
		if (i % 2 == 0) ir2[i] |= PULSE_BIT;
	}
	ir2[cir] = 77000;
	free(ir);

	/* 4 - decode nec ir code reception */
	{
		nec_decode_reset();
		lirc_t*  in = ir2;
		size_t   len = (cir + 1)*sizeof(lirc_t);
		do {
			if (nec_decode(&in, &len))
			{
				sample_t* decoded = NULL;
				size_t n,i;
				
				printf("%s\n", nec_DECODE_BUFF);

				/* 5 - convert bit string to bytes */
				n = string2bytes(nec_DECODE_BUFF, &decoded);
				for (i = 0; i<(n / sizeof(sample_t)); i++) {
					printf("0x%X\n", decoded[i]);
				}

				free(decoded);
				nec_decode_reset();
			}
		} while (len > 0);
	}
	free(ir2);
}

void unkn0()
{
	sample_t test[2] = { 0xAD, 0x13 };
	char*    bits;
	lirc_t*  ir;
	lirc_t*  ir2;
	size_t   nir;
	int      cir;

	/* 1 - create a bit string */
	bits = bytes2string(test, 15);
	printf("%s\n",bits);

	/* 2 - encode it into unkn0 */
	nir = unkn0_encode(bits, &ir);
	free(bits);

	/* 3 - simulate this unkn0 ir code reception */
	cir = nir / sizeof(lirc_t);
	ir2 = malloc((cir+1)*sizeof(lirc_t));
	for (int i = 0; i < cir; i++) {
		ir2[i] = ir[i];
		if (i % 2 == 0) ir2[i] |= PULSE_BIT;
	}
	ir2[cir] = 77000;
	free(ir);

	/* 4 - decode unkn0 ir code reception */
	{
		unkn0_decode_reset();
		lirc_t*  in = ir2;
		size_t   len = (cir + 1)*sizeof(lirc_t);
		do {
			if (unkn0_decode(&in, &len))
			{
				sample_t* decoded = NULL;
				size_t n,i;
				
				printf("%s\n", unkn0_DECODE_BUFF);

				/* 5 - convert bit string to bytes */
				n = string2bytes(unkn0_DECODE_BUFF, &decoded);
				for (i = 0; i<(n / sizeof(sample_t)); i++) {
					printf("0x%X\n", decoded[i]);
				}

				free(decoded);
				unkn0_decode_reset();
			}
		} while (len > 0);
	}
	free(ir2);
}


int main()
{
	UTEST_bitsops();
	
	UTEST_nec();
	nec();
	
	UTEST_unkn0();
	unkn0();

	return 0;
}

#else
int main()
{
	// TODO
}
#endif