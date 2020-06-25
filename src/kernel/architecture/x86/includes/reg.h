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

#ifndef REG_H
#define REG_H
#include "../../../defines/types.h"

struct SregX86 {
    u32 eax, ecx, edx, ebx;
    u32 esp, ebp, esi, edi;
    u32 eip, eflags;
    u32 cs, ss, ds, es, fs, gs;
    u32 cr3;
};

struct SstackX86 {
    u32 esp0;
    u16 ss0;
} __attribute__((packed));

#endif

