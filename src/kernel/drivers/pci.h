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

#ifndef PCI_H
#define PCI_H
#include "../defines/types.h"

#define PCI_CONFIG          0xCF8
#define PCI_DATA            0xCFC
#define PCI_VENDOR_DEVICE   0x00
#define PCI_CLASS_SUBCLASS  0x08

class Cpci {
public:
    Cpci();
    u32 read(u32 bus, u32 device, u32 function, u32 offset);
    void write(u32 bus, u32 device, u32 function, u32 offset, u32 data);
    u8 find(u32 vendor, u32 device, u32* bus, u32* dev, u32* function);
    void test();
private:

};

#endif

