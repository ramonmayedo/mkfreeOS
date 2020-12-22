/*Copyright (C) 2019  Ram�n Mayedo Morales (ramonmayedo@gmail.com)

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

#include "../includes/memory.h"

void* memcpy(void* des, const void* src, u32 size) {
    char *dest = (char*) des;
    char *source = (char*) src;
    while (size--) {
        *(dest++) = *(source++);
    }
    return dest;
}

void *memset(char *dest, char val, int n) {
    char *ptrdest = dest;
    while (n--)
        *ptrdest++ = val;
}

int memcmp(const void* s1, const void* s2, u32 size) {
    char* ptr1 = (char*) s1;
    char* ptr2 = (char*) s2;
    while (size-- > 0) {
        if (*ptr1++ != *ptr2++)
            return 1;
    }
    return 0;
}