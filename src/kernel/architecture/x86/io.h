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

#ifndef IO_H
#define IO_H
#include "../../defines/types.h"
class Cio {
public:
    Cio();
    void outb(u32 addr,u8 value);
    void outw(u32 addr,u16 value);
    void outl(u32 addr,u32 value);
    u8 inb(u32 addr);
    u16 inw(u32 addr);
    u32 inl(u32 addr);
private:

};

#endif

