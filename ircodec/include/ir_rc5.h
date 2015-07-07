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
#ifndef RC5CODEC
#define RC5CODEC

#include "lirc.h"


/* RC-5
carrier 36kHz
pulse time T=889us

# bit '1'
Defined by a falling egde

  <-T-> <-T->
  |----|
  |    |----|

# bit '0'
Defined by a raising egde


  <-T-> <-T->
       |----|
  |----|    |

Timming
1.778ms/bit ; 500 bits/s
*/

/* TODO */

#ifdef UNITARY_TEST
void UTEST_rc5();
#endif
#endif