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

#ifndef SIRCCODEC
#define SIRCODEC

#include "lirc.h"

/*  Sony SIRC
carrier 40kHz
pulse time T=600us

# bit '1'
Defined by a pulse of 2xT follows by a space of T

  <---2xT--> <-T->
  |----|----|
  |         |----|

# bit '0'
Defined by a pulse of T follows by a space of T


  <-T-> <-T->
  |----|
  |    |----|


Timming
worst case =  1.8ms/bit ; 500 bits/s
*/

// TODO 


#ifdef UNITARY_TEST
void UTEST_sirc();
#endif
#endif