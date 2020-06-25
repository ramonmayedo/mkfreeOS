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

#include "../includes/string.h"

int strcmp(char* astring1, char *astring2) {
    int result;
    int asize = strlen(astring1);
    while (asize > 0) {
        result = *astring1 - *astring2;
        if (result != 0) return result;
        astring1++;
        astring2++;
        if (*astring1 == 0x0 || *astring2 == 0x0) return  *astring1 - *astring2;
        asize--;
    }
    return 0;
}

int strlen(const char* astring) {
    if (astring == 0) return 0;
    int count = 0;
    while (*astring++ != 0x0 && count < 255)
        count++;
    return count;
}

char *strtok(char *str, char *sep) {
    static char * ptrstr = 0;
    bool founded = false;

    if (str == 0 || sep == 0)  {ptrstr=0; return 0;} 
    if (ptrstr != 0) str = ptrstr;
    int count = 0;
    int i;
       
    while (str[count] != 0x0) count++;
    
    if (count == 0) {
        ptrstr = 0;
        return 0;
    }


    for (i=0; i < count; i++)
        if (str[i] == sep[0]) {
            founded = true;
            break;
        }

    if (founded) {
        str[i] = 0x0;
        while (str[i + 1] == sep[0]) i++;
    }

    ptrstr = &str[i + 1];
    return str;
}

char* strcpy(char* dest, const char *src) {
    if ((dest == (char *) 0) || (src == (char *) 0)) {
        return (dest = 0);
    }
    int maxLength = strlen(src);
    if (maxLength > 255)
        maxLength = 255;

    for (int i = 0; i < maxLength; i++) {
        dest[i] = src[i];
        if (src[i] == 0x0) break;
    }
    dest[maxLength] = 0x0;
    return (dest);
}

char *strcat(char *dest, const char *src) {
    if (dest == 0 || src == 0) return 0;
    strcpy((char*) (dest + strlen(dest)), src);
    return dest;
}