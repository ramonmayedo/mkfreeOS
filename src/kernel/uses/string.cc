/*Copyright (C) 2019  Ramón Mayedo Morales (ramonmayedo@gmail.com)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>
*/

#include "string.h"
#include "../defines/types.h"

Cstring::Cstring() {
}

char* Cstring::strCopy(char* adest ,char *asource,int amaxLength){
    if ((adest == (char *)0) || (asource == (char *)0)){
	return (adest = 0);
    }
  //  if (amaxLength > 255)
    //    amaxLength = 255;

    for (int i= 0;i<amaxLength;i++){
        adest[i] = asource[i];
        if (asource[i] == 0x0) return (adest);
    }
    
    adest[amaxLength] = 0x0;   
    return (adest);
}

 int Cstring::strCmp(char* astring1,char *astring2,int asize){
     int result;
     while (asize>0){
         result = *astring1 - *astring2;
         if (result != 0) return result;
         astring1++;astring2++;
         if (*astring1==0x0 || *astring2==0x0) return *astring1 - *astring2;
         asize--;
     }
     return 0;
 }

int Cstring::strLenght(char* astring) {
    int count = 0;
    while (*astring++ != 0x0 && count < 255) {
        count++;
    }
    return count;
}

 