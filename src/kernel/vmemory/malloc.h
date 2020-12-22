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

#ifndef MALLOC_H
#define MALLOC_H
#include "../defines/types.h"

struct SmallocHeader {
    u8 pgsize : 2;
    u8 used : 1;
    u8 option : 5;
    u32 size;
} __attribute__((packed));

class Cmalloc {
public:
    Cmalloc();
    void *malloc(u32 asize);
    void *bsrk(u32 anpages);
    void free(void *amemory);
private:

};

#endif

