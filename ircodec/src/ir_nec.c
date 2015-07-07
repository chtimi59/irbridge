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


#define  DEFINE
#include "ir_nec.h"
#include "string.h"

void nec_decode_reset() {
	int i;
	nec_DECODE_BUF_IDX = 0;
	for (i = 0; i < nec_DECODE_BUFF_SIZE; i++)
		nec_DECODE_BUFF[i] = '\0';
}

int nec_decode(lirc_t ** pIn, size_t * pLen)
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
			if (lenght > NEC_TS) {
				return (nec_DECODE_BUF_IDX > 0)?1:0;
			}

			/* deal with data (drop if overflow!)*/
			if (nec_DECODE_BUF_IDX<(nec_DECODE_BUFF_SIZE + 2)) {
				if (!isPulse) nec_DECODE_BUFF[nec_DECODE_BUF_IDX++] = (lenght>NEC_TD) ? '1' : '0';
			}
			
		} while (0);
	}
	return 0;
}

size_t nec_encode(char* in, lirc_t** pOut)
{
	int i,n=0;
	int nbbits = strlen(in);
	int nAlloc = (nbbits * 2 + 2 + 1)* sizeof(lirc_t);

	/* No bits ? */
	if (nbbits <= 0) {
		*pOut = NULL;
		return 0;
	}

	/* Allocation */
	*pOut = malloc(nAlloc);

	/* fullfill */
	(*pOut)[n++] = NEC_THEAD_PULSE;
	(*pOut)[n++] = NEC_THEAD_SPACE;
	for (i = 0; i < nbbits; i++) {
		(*pOut)[n++] = NEC_T0;
		(*pOut)[n++] = (in[i] == '1') ? NEC_T1 : NEC_T0;
	}
	
	/* last pulse for lirc */
	(*pOut)[n++] = NEC_T0;	
	return nAlloc;
}


#ifdef UNITARY_TEST
#include <stdio.h>
#include <assert.h>
void UTEST_nec() {
	{
		int n;
		int p, i;
		size_t len;
		lirc_t  BUFF[1024];
		lirc_t* in;
		
		nec_decode_reset();
		len = 0;
		in = BUFF;		
		n=nec_decode(&in, &len);
		assert(n == 0);
		assert(in == BUFF);
		
		nec_decode_reset();
		p = 0;
		BUFF[p++] = 2672565;
		len = p*sizeof(lirc_t);
		in = BUFF;		
		n = nec_decode(&in, &len);
		assert(n == 0);
		assert(in == (BUFF+p));

		nec_decode_reset();
		p = 0;
		BUFF[p++] = PULSE_BIT | 576;
		BUFF[p++] = 514; /*0*/
		BUFF[p++] = PULSE_BIT | 583;
		BUFF[p++] = 1624; /*1*/
		BUFF[p++] = 2672565;
		len = p*sizeof(lirc_t);
		in = BUFF;		
		n = nec_decode(&in, &len);
		assert(n == 1);
		assert(in == (BUFF + p));
		assert(strncmp(nec_DECODE_BUFF, "01", nec_DECODE_BUF_IDX) == 0);

		nec_decode_reset();
		p = 0;
		BUFF[p++] = PULSE_BIT | 576;
		BUFF[p++] = 514; /*0*/
		BUFF[p++] = PULSE_BIT | 583;
		BUFF[p++] = 1624;/*1*/
		BUFF[p++] = PULSE_BIT | 568;
		BUFF[p++] = 531;/*0*/
		BUFF[p++] = PULSE_BIT | 525;
		BUFF[p++] = 1680;/*1*/
		len = p*sizeof(lirc_t);
		in = BUFF;
		n = nec_decode(&in, &len);
		assert(n == 0);
		assert(in == (BUFF + p));
		p = 0;
		BUFF[p++] = PULSE_BIT | 569;
		BUFF[p++] = 1634; /*1*/
		BUFF[p++] = PULSE_BIT | 598;
		BUFF[p++] = 1603; /*1*/
		BUFF[p++] = PULSE_BIT | 593;
		BUFF[p++] = 510; /*0*/
		BUFF[p++] = 77000;
		len = p*sizeof(lirc_t);
		in = BUFF;
		n = nec_decode(&in, &len);
		assert(n == 1);
		assert(in == (BUFF + p));
		assert(strncmp(nec_DECODE_BUFF, "0101110", nec_DECODE_BUF_IDX) == 0);


		nec_decode_reset();
		p = 0;
		BUFF[p++] = 2672565;
		BUFF[p++] = PULSE_BIT | 8825;
		BUFF[p++] = 4407;
		BUFF[p++] = PULSE_BIT | 576;
		BUFF[p++] = 514;
		BUFF[p++] = PULSE_BIT | 583;
		BUFF[p++] = 1624;
		BUFF[p++] = PULSE_BIT | 568;
		BUFF[p++] = 531;
		BUFF[p++] = PULSE_BIT | 525;
		BUFF[p++] = 1680;
		BUFF[p++] = PULSE_BIT | 569;
		BUFF[p++] = 1634;
		BUFF[p++] = PULSE_BIT | 598;
		BUFF[p++] = 1603;
		BUFF[p++] = PULSE_BIT | 593;
		BUFF[p++] = 510;
		BUFF[p++] = PULSE_BIT | 558;
		BUFF[p++] = 1646;
		BUFF[p++] = PULSE_BIT | 587;
		BUFF[p++] = 511;
		BUFF[p++] = PULSE_BIT | 562;
		BUFF[p++] = 539;
		BUFF[p++] = PULSE_BIT | 522;
		BUFF[p++] = 579;
		BUFF[p++] = PULSE_BIT | 519;
		BUFF[p++] = 591;
		BUFF[p++] = PULSE_BIT | 564;
		BUFF[p++] = 525;
		BUFF[p++] = PULSE_BIT | 527;
		BUFF[p++] = 1682;
		BUFF[p++] = PULSE_BIT | 584;
		BUFF[p++] = 514;
		BUFF[p++] = PULSE_BIT | 568;
		BUFF[p++] = 1639;
		BUFF[p++] = PULSE_BIT | 592;
		BUFF[p++] = 1612;
		BUFF[p++] = PULSE_BIT | 571;
		BUFF[p++] = 1632;
		BUFF[p++] = PULSE_BIT | 583;
		BUFF[p++] = 514;
		BUFF[p++] = PULSE_BIT | 570;
		BUFF[p++] = 533;
		BUFF[p++] = PULSE_BIT | 571;
		BUFF[p++] = 553;
		BUFF[p++] = PULSE_BIT | 496;
		BUFF[p++] = 585;
		BUFF[p++] = PULSE_BIT | 521;
		BUFF[p++] = 579;
		BUFF[p++] = PULSE_BIT | 563;
		BUFF[p++] = 553;
		BUFF[p++] = PULSE_BIT | 549;
		BUFF[p++] = 536;
		BUFF[p++] = PULSE_BIT | 587;
		BUFF[p++] = 513;
		BUFF[p++] = PULSE_BIT | 521;
		BUFF[p++] = 1686;
		BUFF[p++] = PULSE_BIT | 567;
		BUFF[p++] = 1637;
		BUFF[p++] = PULSE_BIT | 586;
		BUFF[p++] = 1617;
		BUFF[p++] = PULSE_BIT | 582;
		BUFF[p++] = 1621;
		BUFF[p++] = PULSE_BIT | 657;
		BUFF[p++] = 1545;
		BUFF[p++] = PULSE_BIT | 600;
		BUFF[p++] = 1604;
		BUFF[p++] = PULSE_BIT | 600;
		BUFF[p++] = 2672565;
		BUFF[p++] = PULSE_BIT | 8825;
		BUFF[p++] = 4407;
		BUFF[p++] = PULSE_BIT | 576;
		BUFF[p++] = 514;
		BUFF[p++] = PULSE_BIT | 583;
		BUFF[p++] = 1624;
		BUFF[p++] = PULSE_BIT | 568;
		BUFF[p++] = 531;
		BUFF[p++] = PULSE_BIT | 525;
		BUFF[p++] = 1680;
		BUFF[p++] = PULSE_BIT | 569;
		BUFF[p++] = 1634;
		BUFF[p++] = PULSE_BIT | 598;
		BUFF[p++] = 1603;
		BUFF[p++] = PULSE_BIT | 593;
		BUFF[p++] = 510;
		BUFF[p++] = PULSE_BIT | 558;
		BUFF[p++] = 1646;
		BUFF[p++] = PULSE_BIT | 587;
		BUFF[p++] = 511;
		BUFF[p++] = PULSE_BIT | 562;
		BUFF[p++] = 539;
		BUFF[p++] = PULSE_BIT | 522;
		BUFF[p++] = 579;
		BUFF[p++] = PULSE_BIT | 519;
		BUFF[p++] = 591;
		BUFF[p++] = PULSE_BIT | 564;
		BUFF[p++] = 525;
		BUFF[p++] = PULSE_BIT | 527;
		BUFF[p++] = 1682;
		BUFF[p++] = PULSE_BIT | 584;
		BUFF[p++] = 514;
		BUFF[p++] = PULSE_BIT | 568;
		BUFF[p++] = 1639;
		BUFF[p++] = PULSE_BIT | 592;
		BUFF[p++] = 1612;
		BUFF[p++] = PULSE_BIT | 571;
		BUFF[p++] = 1632;
		BUFF[p++] = PULSE_BIT | 583;
		BUFF[p++] = 514;
		BUFF[p++] = PULSE_BIT | 570;
		BUFF[p++] = 533;
		BUFF[p++] = PULSE_BIT | 571;
		BUFF[p++] = 553;
		BUFF[p++] = PULSE_BIT | 496;
		BUFF[p++] = 585;
		BUFF[p++] = PULSE_BIT | 521;
		BUFF[p++] = 579;
		BUFF[p++] = PULSE_BIT | 563;
		BUFF[p++] = 553;
		BUFF[p++] = PULSE_BIT | 549;
		BUFF[p++] = 536;
		BUFF[p++] = PULSE_BIT | 587;
		BUFF[p++] = 513;
		BUFF[p++] = PULSE_BIT | 521;
		BUFF[p++] = 1686;
		BUFF[p++] = PULSE_BIT | 567;
		BUFF[p++] = 1637;
		BUFF[p++] = PULSE_BIT | 586;
		BUFF[p++] = 1617;
		BUFF[p++] = PULSE_BIT | 582;
		BUFF[p++] = 1621;
		BUFF[p++] = PULSE_BIT | 657;
		BUFF[p++] = 1545;
		BUFF[p++] = PULSE_BIT | 600;
		BUFF[p++] = 1604;
		BUFF[p++] = PULSE_BIT | 600;
		BUFF[p++] = 2672565;
		BUFF[p++] = PULSE_BIT | 8825;
		len = p*sizeof(lirc_t);
		in = BUFF;
		i = 0;
		do {
			i++; // test no
			n = nec_decode(&in, &len);
			switch (i) {
				case 1:
					assert(n == 0);
					assert(in == (BUFF + 1));
					break;
				case 2:
					assert(n == 0);
					assert(in == (BUFF + 2));
					break;
				case 3:
					assert(n == 0);
					assert(in == (BUFF + 3));
					break;
				case 4:
					assert(n == 1);
					assert(strncmp(nec_DECODE_BUFF, "01011101000001011100000000111111", nec_DECODE_BUF_IDX) == 0);
					break;
				case 5:
				case 6:
					assert(n == 0);
					break;
				case 7:
					assert(n == 1);
					assert(strncmp(nec_DECODE_BUFF, "01011101000001011100000000111111", nec_DECODE_BUF_IDX) == 0);
					break;
			}
			if (n == 1) nec_decode_reset();
		} while (len>0);
		
		fprintf(stderr, "nec_decode() -- tests OK\n");
	}

	{
		lirc_t* out = NULL;
		size_t n = 0;

		n = nec_encode("", &out);
		assert(n == 0);
		assert(out==NULL);

		n = nec_encode("1", &out);
		assert(n == sizeof(lirc_t)*(2*1+2+1));
		assert((out[0] == NEC_THEAD_PULSE) && (out[1] == NEC_THEAD_SPACE));
		assert((out[2] == NEC_T0) && (out[3] == NEC_T1));
		assert(out[4] == NEC_T0);
		free(out);

		n = nec_encode("0", &out);
		assert(n == sizeof(lirc_t)*(2*1+2+1));
		assert((out[0] == NEC_THEAD_PULSE) && (out[1] == NEC_THEAD_SPACE));
		assert((out[2] == NEC_T0) && (out[3] == NEC_T0));
		free(out);

		n = nec_encode("110", &out);
		assert(n == sizeof(lirc_t)*(2*3+2+1));
		assert((out[0] == NEC_THEAD_PULSE) && (out[1] == NEC_THEAD_SPACE));
		assert((out[2] == NEC_T0) && (out[3] == NEC_T1));
		assert((out[4] == NEC_T0) && (out[5] == NEC_T1));
		assert((out[6] == NEC_T0) && (out[7] == NEC_T0));
		free(out);

		fprintf(stderr, "nec_encode() -- tests OK\n");
	}
}
#endif
