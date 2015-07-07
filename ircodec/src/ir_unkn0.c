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


#define  UNKN0_DEFINE
#include "ir_unkn0.h"
#include "string.h"

void unkn0_decode_reset() {
	int i;
	unkn0_DECODE_BUF_IDX = 0;
	for (i = 0; i < unkn0_DECODE_BUFF_SIZE; i++)
		unkn0_DECODE_BUFF[i] = '\0';
}

int unkn0_decode(lirc_t ** pIn, size_t * pLen)
{	
	while (*pLen)
	{
		lirc_t data = **pIn;
		int isPulse = (data & PULSE_BIT);
		int lenght = (int)(data & PULSE_MASK);
		
		/* input stream progression */
		(*pIn)++;
		(*pLen) -= sizeof(lirc_t);

		do {

			/* is end of frame ? */
			if ((!isPulse) && (lenght > UNKN0_TS)) {
				return (unkn0_DECODE_BUF_IDX > 0)?1:0;
			}

			/* deal with data (drop if overflow!)*/
			if (unkn0_DECODE_BUF_IDX<(unkn0_DECODE_BUFF_SIZE + 2)) {
				if (!isPulse) unkn0_DECODE_BUFF[unkn0_DECODE_BUF_IDX++] = (lenght>UNKN0_TD) ? '1' : '0';
			}
			
		} while (0);
	}
	return 0;
}

size_t unkn0_encode(char* in, lirc_t** pOut)
{
	int i,n=0;
	int nbbits = strlen(in);
	int nAlloc = (nbbits * 2 +1) * sizeof(lirc_t);

	/* No bits ? */
	if (nbbits <= 0) {
		*pOut = NULL;
		return 0;
	}

	/* Allocation */
	*pOut = malloc(nAlloc);

	/* fullfill */
	for (i = 0; i < nbbits; i++) {
		(*pOut)[n++] = UNKN0_T0;
		(*pOut)[n++] = (in[i] == '1') ? UNKN0_T1 : UNKN0_T0;
	}
	
	/* last pulse for lirc */
	(*pOut)[n++] = UNKN0_T0;	
	return nAlloc;
}


#ifdef UNITARY_TEST
#include <stdio.h>
#include <assert.h>
void UTEST_unkn0() {
	{
		int n;
		int p, i;
		size_t len;
		lirc_t  BUFF[1024];
		lirc_t* in;
		
		unkn0_decode_reset();
		len = 0;
		in = BUFF;		
		n=unkn0_decode(&in, &len);
		assert(n == 0);
		assert(in == BUFF);
		
		unkn0_decode_reset();
		p = 0;
		BUFF[p++] = 77000;
		len = p*sizeof(lirc_t);
		in = BUFF;		
		n = unkn0_decode(&in, &len);
		assert(n == 0);
		assert(in == (BUFF+p));

		unkn0_decode_reset();
		p = 0;
		BUFF[p++] = 500 | PULSE_BIT; // 1
		BUFF[p++] = 1000;
		BUFF[p++] = 500 | PULSE_BIT; // 1
		BUFF[p++] = 1000;
		BUFF[p++] = 500 | PULSE_BIT; // 0
		BUFF[p++] = 500;
		BUFF[p++] = 500 | PULSE_BIT; // 1
		BUFF[p++] = 1000;
		BUFF[p++] = 500 | PULSE_BIT; // 1
		BUFF[p++] = 1000;
		BUFF[p++] = 500 | PULSE_BIT; // 1
		BUFF[p++] = 1000;
		BUFF[p++] = 500 | PULSE_BIT; // 0
		BUFF[p++] = 500;
		BUFF[p++] = 500 | PULSE_BIT; // 0
		BUFF[p++] = 500;
		BUFF[p++] = 77000;
		len = p*sizeof(lirc_t);
		in = BUFF;		
		n = unkn0_decode(&in, &len);
		assert(n == 1);
		assert(in == (BUFF + p));
		assert(strncmp(unkn0_DECODE_BUFF, "11011100", unkn0_DECODE_BUF_IDX) == 0);

		unkn0_decode_reset();
		p = 0;
		BUFF[p++] = 500 | PULSE_BIT; // 1
		BUFF[p++] = 1000;
		BUFF[p++] = 500 | PULSE_BIT; // 1
		BUFF[p++] = 1000;
		BUFF[p++] = 500 | PULSE_BIT; // 0
		BUFF[p++] = 500;
		BUFF[p++] = 500 | PULSE_BIT; // 1
		BUFF[p++] = 1000;
		len = p*sizeof(lirc_t);
		in = BUFF;
		n = unkn0_decode(&in, &len);
		assert(n == 0);
		assert(in == (BUFF + p));
		p = 0;
		BUFF[p++] = 500 | PULSE_BIT; // 1
		BUFF[p++] = 1000;
		BUFF[p++] = 500 | PULSE_BIT; // 1
		BUFF[p++] = 1000;
		BUFF[p++] = 500 | PULSE_BIT; // 0
		BUFF[p++] = 500;
		BUFF[p++] = 500 | PULSE_BIT; // 0
		BUFF[p++] = 500;
		BUFF[p++] = 77000;
		len = p*sizeof(lirc_t);
		in = BUFF;
		n = unkn0_decode(&in, &len);
		assert(n == 1);
		assert(in == (BUFF + p));
		assert(strncmp(unkn0_DECODE_BUFF, "11011100", unkn0_DECODE_BUF_IDX) == 0);


		unkn0_decode_reset();
		p = 0;
		BUFF[p++] = 77000;
		BUFF[p++] = 500 | PULSE_BIT; // 1
		BUFF[p++] = 1000;
		BUFF[p++] = 500 | PULSE_BIT; // 1
		BUFF[p++] = 1000;
		BUFF[p++] = 500 | PULSE_BIT; // 0
		BUFF[p++] = 500;
		BUFF[p++] = 500 | PULSE_BIT; // 1
		BUFF[p++] = 1000;
		BUFF[p++] = 77000;
		BUFF[p++] = 500 | PULSE_BIT; // 1
		BUFF[p++] = 1000;
		BUFF[p++] = 500 | PULSE_BIT; // 1
		BUFF[p++] = 1000;
		BUFF[p++] = 500 | PULSE_BIT; // 0
		BUFF[p++] = 500;
		BUFF[p++] = 77000;
		BUFF[p++] = 500 | PULSE_BIT; // 0
		BUFF[p++] = 500;
		BUFF[p++] = 77000;
		BUFF[p++] = 500 | PULSE_BIT; // 0
		BUFF[p++] = 500;
		len = p*sizeof(lirc_t);
		in = BUFF;
		i = 0;
		do {
			i++; // test no
			n = unkn0_decode(&in, &len);
			switch (i) {
				case 1:
					assert(n == 0);
					assert(in == (BUFF + 1));
					break;
				case 2:
					assert(n == 1);
					assert(in == (BUFF + 10));
					assert(strncmp(unkn0_DECODE_BUFF, "1101", unkn0_DECODE_BUF_IDX) == 0);
					break;
				case 3:
					assert(n == 1);
					assert(in == (BUFF + 17));
					assert(strncmp(unkn0_DECODE_BUFF, "110", unkn0_DECODE_BUF_IDX) == 0);
					break;
				case 4:
					assert(n == 1);
					assert(in == (BUFF + 20));
					assert(strncmp(unkn0_DECODE_BUFF, "0", unkn0_DECODE_BUF_IDX) == 0);
					break;
				case 5:
					assert(n == 0);
					assert(in == (BUFF + 22));
					break;
			}
			if (n == 1) unkn0_decode_reset();
		} while (len>0);
		
		fprintf(stderr, "unkn0_decode() -- tests OK\n");
	}

	{
		lirc_t* out = NULL;
		size_t n = 0;

		n = unkn0_encode("", &out);
		assert(n == 0);
		assert(out==NULL);

		n = unkn0_encode("1", &out);
		assert(n == sizeof(lirc_t)*(2*1+1));
		assert((out[0] == UNKN0_T0) && (out[1] == UNKN0_T1));
		free(out);

		n = unkn0_encode("0", &out);
		assert(n == sizeof(lirc_t)*(2*1+1));
		assert((out[0] == UNKN0_T0) && (out[1] == UNKN0_T0));
		free(out);

		n = unkn0_encode("110", &out);
		assert(n == sizeof(lirc_t)* (2*3+1));
		assert((out[0] == UNKN0_T0) && (out[1] == UNKN0_T1));
		assert((out[2] == UNKN0_T0) && (out[3] == UNKN0_T1));
		assert((out[4] == UNKN0_T0) && (out[5] == UNKN0_T0));
		free(out);

		fprintf(stderr, "unkn0_encode() -- tests OK\n");
	}
}
#endif
