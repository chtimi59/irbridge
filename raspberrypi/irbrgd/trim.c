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

/* can't believe this string trim() doesn't exist in C... */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "trim.h"

int trim(char*s, int* multiline) {
	int len;
	char* out = NULL;
	if (!s) return 0;
	len=strlen(s);
	if (len==0) return 0;
	out = malloc(len+1);
	if (!out) return 0;
	out[0] = '\0';
	if (multiline) *multiline = 0;
		
	/* 1. trim beginning */
	{
		int b=0;int j=0; int i=0;
		for(i=0;i<len;i++)
		{
			/* regular char meet ? */
			if (!b) {
				b = 1 && (s[i]!='\n');
				b = b && (s[i]!='\r');
				b = b && (s[i]!='\t');
				b = b && (s[i]!=' ');
			}
			
			if (b) {
				/* starts with a comment '#' ?*/
				if (j==0 && s[i]=='#') {
					strcpy(s, "");
					return 0;
				}
				/* regular copy */
				out[j++] = s[i];
			}
		}
		out[j++] = '\0';
	}
	strcpy(s, out);
	len=strlen(s);
	free(out);
	
	/* 2. crop the tail */
	{
		int b=0; int bnl=0; int i=0;
		int multl = 0;
		for(i=len-1;i>=0;i--) {
		
			/* regular char meet ? */
			if (!b) {
				b = 1 && (s[i]!='\n');
				b = b && (s[i]!='\r');
				b = b && (s[i]!='\t');
				b = b && (s[i]!=' ');
			}
			
			/* multiline detection */
			if (!bnl) {
				bnl = 1 && (s[i]!='\n');
				bnl = bnl && (s[i]!='\r');
				bnl = bnl && (s[i]!='\\');
				if (!bnl && s[i]=='\\') {
					multl = 1;
					s[i]=' ';
				}
			}
			
			/* crop */
			if(!b) s[i]='\0';
			/* ends with a comment ' #' ? */
			if(b && s[i]=='#' && i>0 && (s[i-1]==' '||s[i-1]=='\t') ) {
				/* crop it from here */
				s[i]='\0'; 
				/* and forget this char */
				b = 0;
				/* invalidate multiline */
				multl = 0;
			}
		}
		
		if (multiline) *multiline = multl;
	}	
	
	return strlen(s);
}


/* test 
hello world __ trival case
  		hello world __ various head an tail  	   
  		hello world __ various head an tail with comment  	   # comment
  		hello world __ various head an tail with comment  	   # comment here	   		
un
hello world 1__ comment#not valid comment here
hello world 2__ comment# not valid comment here
hello world 3__ comment # comment here

emptylines:
# empty line 	
	   # empty line 	  
		 
		 
empty lines 	
	   #empty line 	

valid new lines\
invalid new lines\ a b t	 
invalid new lines\ 
invalid new lines\ #sdfsdf
valid \n\e\w lines	   \
	valid \n\e\w lines	   \
	 valid new lines	  \
#	 invalid new lines	  \	 
	 valid new lines #sd fsf  \
*/


void splitKey(char* s, char** pkey, char** pargs) {
	int i,len;
	int b1 = 0;
	int b2 = 0;
	int b3 = 0;
	if (s==NULL) return;
	len = strlen(s);
	for(i=0; i<len; i++)
	{
		int c = s[i];
		/* 1- search for the first non white char*/
		if (!b1) {
			b1 = 1 && (s[i]!='\t');
			b1 = b1 && (s[i]!=' ');
			if (b1 && pkey) *pkey=&s[i];
		}	
				
		/* 2- search for the first white char*/
		if (b1 && !b2) {
			b2 = b2 || (s[i]=='\t');
			b2 = b2 || (s[i]==' ');
			if (b2) c='\0';
		}
		
		/* 3- then search for a non white char*/
		if (b1 && b2 && !b3) {
			b3 = 1 && (s[i]!='\t');
			b3 = b3 && (s[i]!=' ');
			if (b3 && pargs) *pargs=&s[i];
		}
		
		s[i] = c;
	}	
}

/*test
"hello world" should be [hello][world]
"hello"       should be [hello][(null)]
"  	hello  	" should be [hello][(null)]
""            should be [(null)][(null)]
"    "        should be [(null)][(null)]
"  	hello  	cou dfsdf 	 " should be [hello][cou dfsdf 	 ]
*/